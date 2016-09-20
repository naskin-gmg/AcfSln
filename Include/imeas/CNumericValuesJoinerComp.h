#ifndef imeas_CNumericValuesJoinerComp_included
#define imeas_CNumericValuesJoinerComp_included


// ACF includes
#include <ilog/TLoggerCompWrap.h>

#include <imeas/INumericValueProvider.h>
#include <imeas/CSimpleNumericValue.h>


namespace imeas
{


/**
	Joins several numeric values into one.
*/
class CNumericValuesJoinerComp:
			public ilog::CLoggerComponentBase,
			virtual public imeas::INumericValueProvider,
			protected imeas::CSimpleNumericValue
{
public:
	typedef ilog::CLoggerComponentBase BaseClass;

	I_BEGIN_COMPONENT(CNumericValuesJoinerComp);
		I_REGISTER_INTERFACE(imeas::INumericValueProvider);
		I_ASSIGN_MULTI_0(m_valueProvidersCompPtr, "ValueProviders", "List of numeric value providers", true);
	I_END_COMPONENT;

	// reimplemented (imeas::INumericValueProvider)
	virtual int GetValuesCount() const;
	virtual const imeas::INumericValue& GetNumericValue(int index) const;

protected:
	// reimplemented (imeas::CSimpleNumericValue)
	virtual bool IsValueTypeSupported(imeas::INumericValue::ValueTypeId valueTypeId) const;
	virtual imath::CVarVector GetComponentValue(imeas::INumericValue::ValueTypeId valueTypeId) const;

private:
	I_MULTIREF(imeas::INumericValueProvider, m_valueProvidersCompPtr);
};


} // namespace imeas


#endif // !imeas_CNumericValuesJoinerComp_included
