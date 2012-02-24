#include "imeas/CNumericConstraintsComp.h"


#include "istd/TChangeNotifier.h"

#include "iser/IArchive.h"
#include "iser/CArchiveTag.h"

#include "imath/CDoubleManip.h"


namespace imeas
{


// reimplemented (imeas::INumericConstraints)

int CNumericConstraintsComp::GetNumericValuesCount() const
{
	return *m_dimensionsCountAttrPtr;
}


QString CNumericConstraintsComp::GetNumericValueDescription(int index) const
{
	if (!(*m_elementDescriptionAttrPtr).isEmpty()){
		if (*m_dimensionsCountAttrPtr > 1){
			return *m_elementDescriptionAttrPtr + " " + QString().setNum(index + 1);
		}
		else{
			return *m_elementDescriptionAttrPtr;
		}
	}
	else{
		return QString().setNum(index + 1);
	}
}


const imeas::IUnitInfo& CNumericConstraintsComp::GetNumericValueUnitInfo(int /*index*/) const
{
	return *this;
}


// protected methods

// reimplemented (imeas::IUnitInfo)

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
	static imath::CDoubleManip manip;

	return manip;
}


} // namespace imeas

