#ifndef iipr_CSearchParamsComp_included
#define iipr_CSearchParamsComp_included


// ACF includes
#include <icomp/CComponentBase.h>

// ACF-Solutions includes
#include <iipr/ISearchConstraints.h>
#include <iipr/CSearchParams.h>


namespace iipr
{


class CSearchParamsComp: 
			public icomp::CComponentBase,
			public CSearchParams
{
public:
	typedef icomp::CComponentBase BaseClass;

	I_BEGIN_COMPONENT(CSearchParamsComp);
		I_REGISTER_INTERFACE(iipr::ISearchParams);
		I_REGISTER_INTERFACE(iser::ISerializable);
		I_ASSIGN(m_searchConstraintsCompPtr, "SearchConstaints", "Search parameters constraints", false, "SearchConstaints");
		I_ASSIGN(m_defaultModelOccuranceAttrPtr, "DefaultModelOccurance", "Default model occurance\n\t-1 - all models,\n\t0 - no models", true, -1);
	I_END_COMPONENT;

	// reimplemented (iipr::ISearchParams)
	virtual const ISearchConstraints* GetSearchConstraints() const;

protected:
	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated();

private:
	I_REF(iipr::ISearchConstraints, m_searchConstraintsCompPtr);
	I_ATTR(int, m_defaultModelOccuranceAttrPtr);
};


} // namespace iipr


#endif // !iipr_CSearchParamsComp_included


