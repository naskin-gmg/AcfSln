#ifndef imeas_CNumericParamsComp_included
#define imeas_CNumericParamsComp_included


// ACF includes
#include <icomp/CComponentBase.h>

// ACF-Solutions includes
#include <imath/IUnitInfo.h>
#include <imeas/INumericConstraints.h>
#include <imeas/CSimpleNumericValue.h>


namespace imeas
{


/**
	Component implementating imeas::INumericValue interface.
	All parameter constraints can be defined directly or can use external implementation.
*/
class CNumericParamsComp:
			public icomp::CComponentBase,
			public CSimpleNumericValue
{
public:
	typedef icomp::CComponentBase BaseClass;
	typedef CSimpleNumericValue BaseClass2;

	I_BEGIN_COMPONENT(CNumericParamsComp);
		I_REGISTER_INTERFACE(istd::IChangeable);
		I_REGISTER_INTERFACE(iser::ISerializable);
		I_REGISTER_INTERFACE(INumericValue);
		I_ASSIGN_MULTI_0(m_defaultValuesAttrPtr, "Values", "Default values", false);
		I_ASSIGN(m_constraintsCompPtr, "Constraints", "Constraints object describing possible parameter values", false, "Constraints");
	I_END_COMPONENT;

	// reimplemented (imeas::INumericValue)
	virtual const INumericConstraints* GetNumericConstraints() const;

protected:
	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated();

private:
	I_MULTIATTR(double, m_defaultValuesAttrPtr);
	I_REF(INumericConstraints, m_constraintsCompPtr);
};


} // namespace imeas


#endif // !imeas_CNumericParamsComp_included


