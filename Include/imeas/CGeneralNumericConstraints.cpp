// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial

#include <imeas/CGeneralNumericConstraints.h>


#include <istd/CChangeNotifier.h>


namespace imeas
{


void CGeneralNumericConstraints::Reset()
{
	istd::CChangeNotifier notifier(this);
	Q_UNUSED(notifier);

	m_valueInfos.clear();
}


bool CGeneralNumericConstraints::InsertValueInfo(
			const QString& name,
			const QString& description,
			const QByteArray& valueId,
			const imath::CGeneralUnitInfo& unitInfo,
			int index)
{
	istd::CChangeNotifier notifier(this);
	Q_UNUSED(notifier);

	Info info;
	info.name = name;
	info.description = description;
	info.id = valueId;
	info.unitInfo = unitInfo;

	if (index >= 0){
		m_valueInfos.insert(index, info);
	}
	else{
		m_valueInfos.append(info);
	}

	return true;
}


void CGeneralNumericConstraints::RemoveValueInfo(int index)
{
	m_valueInfos.remove(index);
}


const imath::CGeneralUnitInfo& CGeneralNumericConstraints::GetGeneralUnitInfo(int index) const
{
	return m_valueInfos[index].unitInfo;
}


// reimplemented (imeas::INumericConstraints)

const iprm::IOptionsList& CGeneralNumericConstraints::GetValueListInfo() const
{
	return *this;
}


const imath::IUnitInfo* CGeneralNumericConstraints::GetNumericValueUnitInfo(int index) const
{
	return &m_valueInfos[index].unitInfo;
}


// reimplemented (iprm::IOptionsList)

int CGeneralNumericConstraints::GetOptionsFlags() const
{
	return SCF_SUPPORT_UNIQUE_ID;
}


int CGeneralNumericConstraints::GetOptionsCount() const
{
	return m_valueInfos.count();
}


QString CGeneralNumericConstraints::GetOptionName(int index) const
{
	return m_valueInfos[index].name;
}


QString CGeneralNumericConstraints::GetOptionDescription(int index) const
{
	return m_valueInfos[index].description;
}


QByteArray CGeneralNumericConstraints::GetOptionId(int index) const
{
	return m_valueInfos[index].id;
}


bool CGeneralNumericConstraints::IsOptionEnabled(int /*index*/) const
{
	return true;
}


} // namespace imeas

