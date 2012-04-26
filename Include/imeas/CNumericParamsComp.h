#ifndef imeas_CNumericParamsComp_included
#define imeas_CNumericParamsComp_included


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
	Component implementating imeas::INumericParams interface.
	All parameter constraints can be defined directly or can use external implementation.
*/
class CNumericParamsComp:
			public icomp::CComponentBase,
			virtual public INumericParams,
			virtual protected INumericConstraints,
			virtual protected imeas::IUnitInfo
{
public:
	typedef icomp::CComponentBase BaseClass;

	I_BEGIN_COMPONENT(CNumericParamsComp);
		I_REGISTER_INTERFACE(istd::IChangeable);
		I_REGISTER_INTERFACE(iser::ISerializable);
		I_REGISTER_INTERFACE(INumericParams);
		I_ASSIGN(m_dimensionsCountAttrPtr, "ValuesCount", "Default number of numeric values (will be used if no constraints set)", true, 1);
		I_ASSIGN(m_minValueAttrPtr, "MinValues", "Minimal value (will be used if no constraints set)", true, 1);
		I_ASSIGN(m_maxValueAttrPtr, "MaxValues", "Maximal value (will be used if no constraints set)", true, 10);
		I_ASSIGN_MULTI_0(m_defaultValuesAttrPtr, "Values", "Default values", false);
		I_ASSIGN(m_constraintsCompPtr, "Constraints", "Constraints object describing possible parameter values", false, "Constraints");
	I_END_COMPONENT;

	// reimplemented (imeas::INumericParams)
	virtual const INumericConstraints* GetNumericConstraints() const;
	virtual imath::CVarVector GetValues() const;
	virtual bool SetValues(const imath::CVarVector& values);

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);

	// reimplemented (istd::IChangeable)
	virtual bool CopyFrom(const IChangeable& object);

protected:
	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated();

	// reimplemented (imeas::INumericConstraints)
	virtual int GetNumericValuesCount() const;
	virtual QString GetNumericValueName(int index) const;
	virtual QString GetNumericValueDescription(int index) const;
	virtual const imeas::IUnitInfo& GetNumericValueUnitInfo(int index) const;

	// reimplemented (imeas::IUnitInfo)
	virtual int GetUnitType() const;
	virtual QString GetUnitName() const;
	virtual double GetDisplayMultiplicationFactor() const;
	virtual istd::CRange GetValueRange() const;
	virtual const imath::IDoubleManip& GetValueManip() const;

private:
	imath::CVarVector m_values;

	I_ATTR(int, m_dimensionsCountAttrPtr);
	I_ATTR(double, m_minValueAttrPtr);
	I_ATTR(double, m_maxValueAttrPtr);
	I_MULTIATTR(double, m_defaultValuesAttrPtr);
	I_REF(INumericConstraints, m_constraintsCompPtr);
};


} // namespace imeas


#endif // !imeas_CNumericParamsComp_included


