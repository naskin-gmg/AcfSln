// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#include <imeas/CGeneralNumericValue.h>


// ACF includes
#include <istd/CChangeNotifier.h>
#include <iser/CArchiveTag.h>


namespace imeas
{


void CGeneralNumericValue::SetComponentValue(imeas::INumericValue::ValueTypeId valueTypeId, imath::CVarVector value)
{
	BeginChanges(GetAnyChange());

	m_supportMap[valueTypeId] = value;

	EndChanges(GetAnyChange());
}


bool CGeneralNumericValue::IsValueTypeSupported(imeas::INumericValue::ValueTypeId valueTypeId) const
{
	return m_supportMap.contains(valueTypeId);
}


const imeas::INumericConstraints* CGeneralNumericValue::GetNumericConstraints() const
{
	return NULL;
}


imath::CVarVector CGeneralNumericValue::GetComponentValue(imeas::INumericValue::ValueTypeId valueTypeId) const
{
	return m_supportMap.value(valueTypeId, imath::CVarVector());
}


imath::CVarVector CGeneralNumericValue::GetValues() const
{
	QList<imath::CVarVector> values = m_supportMap.values();
	int size = 0;

	for (int i = 0; i < values.size(); i++){
		size += values[0].GetElementsCount();
	}

	imath::CVarVector retVal(size);

	for (int i = 0; i < values.size(); i++){
		for (int j = 0; j < values[i].GetElementsCount(); j++){
			retVal[(i*j)+j] = values[i].GetElement(j);
		}
	}

	return retVal;
}


bool CGeneralNumericValue::SetValues(const imath::CVarVector& values)
{
	SetComponentValue(VTI_AUTO, values);

	return true;
}


bool CGeneralNumericValue::Serialize(iser::IArchive &archive)
{
	static iser::CArchiveTag supportTag("Supported", "List of supported values", iser::CArchiveTag::TT_MULTIPLE);
	static iser::CArchiveTag supportValueTag("SupportedValues", "supported value", iser::CArchiveTag::TT_GROUP, &supportTag);
	static iser::CArchiveTag idTag("ID", "ID of this component", iser::CArchiveTag::TT_LEAF, &supportValueTag);

	bool retVal = true;

	int count = m_supportMap.size();
	retVal = retVal && archive.BeginMultiTag(supportTag, supportValueTag, count);

	const iser::IVersionInfo& versionInfo = archive.GetVersionInfo();

	quint32 libraryVersion = 0xffffffff;
	versionInfo.GetVersionNumber(1, libraryVersion);

	if (archive.IsStoring()){
		for (SupportMap::const_iterator it = m_supportMap.begin(); it != m_supportMap.end(); ++it){
			int id = it.key();
			imath::CVarVector vector = it.value();

			retVal = retVal && archive.BeginTag(supportValueTag);

			if (libraryVersion > 1177){
				retVal = retVal && archive.BeginTag(idTag);
				retVal = retVal && archive.Process(id);
				retVal = retVal && archive.EndTag(idTag);
			}
			else{
				retVal = retVal && archive.Process(id);
			}

			retVal = retVal && vector.Serialize(archive);

			retVal = retVal && archive.EndTag(supportValueTag);
		}
	}
	else{
		m_supportMap.clear();

		for (int i = 0; i < count; i++){
			imeas::INumericValue::ValueTypeId id = imeas::INumericValue::VTI_AUTO;
			imath::CVarVector value;

			retVal = retVal && archive.BeginTag(supportValueTag);

			if (libraryVersion > 1177){
				retVal = retVal && archive.BeginTag(idTag);
				retVal = retVal && archive.Process((qint8&)id);
				retVal = retVal && archive.EndTag(idTag);
			}
			else{
				retVal = retVal && archive.Process((qint8&)id);
			}

			retVal = retVal && value.Serialize(archive);

			retVal = retVal && archive.EndTag(supportValueTag);

			m_supportMap.insert(id, value);
		}
	}

	retVal = retVal && archive.EndTag(supportTag);

	return retVal;
}


bool CGeneralNumericValue::CopyFrom(const istd::IChangeable& object, CompatibilityMode /*mode*/)
{
	const imeas::CGeneralNumericValue* sourcePtr = dynamic_cast<const imeas::CGeneralNumericValue*>(&object);

	if (sourcePtr != NULL){
		BeginChanges(GetAnyChange());

		m_supportMap = sourcePtr->m_supportMap;

		EndChanges(GetAnyChange());

		return true;
	}

	return false;
}

istd::IChangeableUniquePtr CGeneralNumericValue::CloneMe(CompatibilityMode mode) const
{
	CGeneralNumericValue* value = new CGeneralNumericValue();

	value->CopyFrom(*this, mode);

	return value;
}


} // namespace imeas
