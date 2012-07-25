#ifndef imeas_CNumericConstraintsComp_included
#define imeas_CNumericConstraintsComp_included


// ACF includes
#include "iser/ISerializable.h"
#include "icomp/CComponentBase.h"

// ACF-Solutions incldues
#include "imeas/IUnitInfo.h"

#include "imeas/INumericValue.h"
#include "imeas/INumericConstraints.h"


namespace imeas
{


/**
	Simple implementation of imeas::INumericConstraints interface.
	All values use the same units and ranges.
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
		I_ASSIGN_MULTI_0(m_namesAttrPtr, "ValueName", "Name of each single value", true);
		I_ASSIGN_MULTI_0(m_descriptionsAttrPtr, "ValueDescriptions", "Description of each single value", true);
		I_ASSIGN_MULTI_0(m_valueIdsAttrPtr, "ValueIds", "ID of each single value", true);
		I_ASSIGN(m_unitTypeAttrPtr, "UnitType", "Type of unit\n\t0 - unknown,\n\t1 - physical,\n\t2 - technical,\n\t3 - relative", true, 0);
		I_ASSIGN(m_unitNameAttrPtr, "UnitName", "Name of unit", true, "");
		I_ASSIGN(m_displayMultFactorAttrPtr, "DisplayMultFactor", "Scale factor used for display, e.g for unit name '%' it will be 100", true, 1);
		I_ASSIGN(m_minValueAttrPtr, "MinValue", "Minimal value", true, 0);
		I_ASSIGN(m_maxValueAttrPtr, "MaxValue", "Maximal value", true, 1);
	I_END_COMPONENT;

	// reimplemented (imeas::INumericConstraints)
	virtual int GetNumericValuesCount() const;
	virtual QString GetNumericValueName(int index) const;
	virtual QString GetNumericValueDescription(int index) const;
	virtual const imeas::IUnitInfo& GetNumericValueUnitInfo(int index) const;

protected:
	// reimplemented (imeas::IUnitInfo)
	virtual int GetUnitType() const;
	virtual QString GetUnitName() const;
	virtual double GetDisplayMultiplicationFactor() const;
	virtual istd::CRange GetValueRange() const;
	virtual const imath::IDoubleManip& GetValueManip() const;

private:
	I_ATTR(int, m_dimensionsCountAttrPtr);
	I_MULTIATTR(QString, m_namesAttrPtr);
	I_MULTIATTR(QString, m_descriptionsAttrPtr);
	I_MULTIATTR(QByteArray, m_valueIdsAttrPtr);
	I_ATTR(int, m_unitTypeAttrPtr);
	I_ATTR(QString, m_unitNameAttrPtr);
	I_ATTR(double, m_displayMultFactorAttrPtr);
	I_ATTR(double, m_minValueAttrPtr);
	I_ATTR(double, m_maxValueAttrPtr);
};


} // namespace imeas


#endif // !imeas_CNumericConstraintsComp_included


