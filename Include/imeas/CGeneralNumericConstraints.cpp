
#include "imeas/CGeneralNumericConstraints.h"


#include "istd/CChangeNotifier.h"


namespace imeas
{


void CGeneralNumericConstraints::Reset()
{
	istd::CChangeNotifier notifier(this);
	Q_UNUSED(notifier);

	m_valueInfos.clear();
}


void CGeneralNumericConstraints::InsertValueInfo(const QString& name, const QString& description, const imath::CGeneralUnitInfo& unitInfo, int index)
{
	istd::CChangeNotifier notifier(this);
	Q_UNUSED(notifier);

	Info info;
	info.name = name;
	info.description = description;
	info.unitInfo = unitInfo;

	if (index >= 0){
		m_valueInfos.insert(index, info);
	}
	else{
		m_valueInfos.append(info);
	}
}


void CGeneralNumericConstraints::RemoveValueInfo(int index)
{
	m_valueInfos.remove(index);
}


// reimplemented (imeas::INumericConstraints)

int CGeneralNumericConstraints::GetNumericValuesCount() const
{
	return m_valueInfos.size();
}


QString CGeneralNumericConstraints::GetNumericValueName(int index) const
{
	return m_valueInfos[index].name;
}


QString CGeneralNumericConstraints::GetNumericValueDescription(int index) const
{
	return m_valueInfos[index].description;
}


const imath::IUnitInfo* CGeneralNumericConstraints::GetNumericValueUnitInfo(int index) const
{
	return &m_valueInfos[index].unitInfo;
}


} // namespace imeas

