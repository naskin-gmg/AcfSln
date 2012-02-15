#ifndef imeas_CNumericConstraintsComp_included
#define imeas_CNumericConstraintsComp_included


// ACF includes
#include "iser/ISerializable.h"
#include "icomp/CComponentBase.h"

// ACF-Solutions incldues
#include "imeas/IUnitInfo.h"

#include "imeas/INumericParams.h"
#include "imeas/INumericConstraints.h"


namespace imeas
{


/**
	Simple implementation of imeas::INumericConstraints interface.
	All values use the same units, ranges and descriptions.
*/
class CNumericConstraintsComp:
			public icomp::CComponentBase,
			virtual public INumericConstraints,
			virtual protected imeas::IUnitInfo
{
public:
	typedef icomp::CComponentBase BaseClass;

	I_BEGIN_COMPONENT(CNumericConstraintsComp);
		I_REGISTER_INTERFACE(INumericConstraints);
		I_ASSIGN(m_dimensionsCountAttrPtr, "ValuesCount", "Default number of numeric values", true, 1);
		I_ASSIGN(m_elementDescriptionAttrPtr, "ValueDescription", "Description of single value", true, "Value");
		I_ASSIGN(m_unitTypeAttrPtr, "UnitType", "Type of unit\n\t0 - unknown,\n\t1 - physical,\n\t2 - technical,\n\t3 - relative", true, 0);
		I_ASSIGN(m_unitNameAttrPtr, "UnitName", "Name of unit", true, "");
		I_ASSIGN(m_displayMultFactorAttrPtr, "DisplayMultFactor", "Scale factor used for display, e.g for unit name '%' it will be 100", true, 1);
		I_ASSIGN(m_minValueAttrPtr, "MinValue", "Minimal value", true, 0);
		I_ASSIGN(m_maxValueAttrPtr, "MaxValue", "Maximal value", true, 1);
	I_END_COMPONENT;

	// reimplemented (imeas::INumericConstraints)
	virtual int GetNumericValuesCount() const;
	virtual istd::CString GetNumericValueDescription(int index) const;
	virtual const imeas::IUnitInfo& GetNumericValueUnitInfo(int index) const;

protected:
	// reimplemented (imeas::IUnitInfo)
	virtual int GetUnitType() const;
	virtual istd::CString GetUnitName() const;
	virtual double GetDisplayMultiplicationFactor() const;
	virtual istd::CRange GetValueRange() const;
	virtual const imath::IDoubleManip& GetValueManip() const;

private:
	I_ATTR(int, m_dimensionsCountAttrPtr);
	I_ATTR(istd::CString, m_elementDescriptionAttrPtr);
	I_ATTR(int, m_unitTypeAttrPtr);
	I_ATTR(istd::CString, m_unitNameAttrPtr);
	I_ATTR(double, m_displayMultFactorAttrPtr);
	I_ATTR(double, m_minValueAttrPtr);
	I_ATTR(double, m_maxValueAttrPtr);
};


} // namespace imeas


#endif // !imeas_CNumericConstraintsComp_included


