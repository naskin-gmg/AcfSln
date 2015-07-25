
#include "imeas/CNumericConstraintsComp.h"


#include "istd/CChangeNotifier.h"

#include "iser/IArchive.h"
#include "iser/CArchiveTag.h"


namespace imeas
{


// reimplemented (imeas::INumericConstraints)

int CNumericConstraintsComp::GetNumericValuesCount() const
{
	return *m_dimensionsCountAttrPtr;
}


QString CNumericConstraintsComp::GetNumericValueName(int index) const
{
	if (index < m_namesAttrPtr.GetCount()){
		return m_namesAttrPtr[index];
	}
		
	return QString();
}


QString CNumericConstraintsComp::GetNumericValueDescription(int index) const
{
	if (index < m_descriptionsAttrPtr.GetCount()){
		return m_descriptionsAttrPtr[index];
	}

	return QString();
}


const imath::IUnitInfo* CNumericConstraintsComp::GetNumericValueUnitInfo(int /*index*/) const
{
	return this;
}


// protected methods

// reimplemented (imath::IUnitInfo)

int CNumericConstraintsComp::GetUnitType() const
{
	return *m_unitTypeAttrPtr;
}


QString CNumericConstraintsComp::GetUnitName() const
{
	return *m_unitNameAttrPtr;
}


double CNumericConstraintsComp::GetDisplayMultiplicationFactor() const
{
	return *m_displayMultFactorAttrPtr;
}


istd::CRange CNumericConstraintsComp::GetValueRange() const
{
	return istd::CRange(*m_minValueAttrPtr, *m_maxValueAttrPtr);
}


const imath::IDoubleManip& CNumericConstraintsComp::GetValueManip() const
{
	return *this;
}


// reimplemented (imath::IDoubleManip)

int CNumericConstraintsComp::GetPrecision() const
{
	return m_precision;
}


// reimplemented (ibase::CComponentBase)

void CNumericConstraintsComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	m_precision = qMax(0, *m_precisionAttrPtr);
}


} // namespace imeas

