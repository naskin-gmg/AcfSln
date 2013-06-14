#include "imeas/CGeneralNumericValue.h"


// ACF includes
#include "istd/TChangeNotifier.h"
#include "iser/CArchiveTag.h"

namespace imeas
{


void CGeneralNumericValue::SetComponentValue(imeas::INumericValue::ValueTypeId valueTypeId, imath::CVarVector value)
{
	m_supportMap.insert(valueTypeId, value);
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


bool CGeneralNumericValue::SetValues(const imath::CVarVector& /*values*/)
{
	return false;
}


bool CGeneralNumericValue::Serialize(iser::IArchive &archive)
{
	static iser::CArchiveTag supportTag("Supported", "List of supported values");
	static iser::CArchiveTag supportValueTag("SupportedValues", "supported value");

	bool retVal = true;

	int count = m_supportMap.size();
	retVal = retVal && archive.BeginMultiTag(supportTag, supportValueTag, count);

	if (archive.IsStoring()){
		for (SupportMap::const_iterator it = m_supportMap.begin(); it != m_supportMap.end(); ++it){
			int id = it.key();
			imath::CVarVector vector = it.value();

			retVal = retVal && archive.BeginTag(supportValueTag);
			retVal = retVal && archive.Process(id);
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
			retVal = retVal && archive.Process((qint8&)id);
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
		istd::CChangeNotifier notifier(this);

		m_supportMap = sourcePtr->m_supportMap;

		return true;
	}

	return false;
}

istd::IChangeable* CGeneralNumericValue::CloneMe(CompatibilityMode mode) const
{
	CGeneralNumericValue* value = new CGeneralNumericValue();

	value->CopyFrom(*this, mode);

	return value;
}


} // namespace imeas