#ifndef iipr_CSearchParamsComp_included
#define iipr_CSearchParamsComp_included


// ACF includes
#include "ibase/TLoggerCompWrap.h"

#include "iipr/ISearchConstraints.h"
#include "iipr/CSearchParams.h"


namespace iipr
{


class CSearchParamsComp: 
			public ibase::CLoggerComponentBase,
			public CSearchParams
{
public:
	typedef ibase::CLoggerComponentBase BaseClass;

	I_BEGIN_COMPONENT(CSearchParamsComp);
		I_REGISTER_INTERFACE(iipr::ISearchParams);
		I_REGISTER_INTERFACE(iser::ISerializable);
		I_ASSIGN(m_searchConstraintsCompPtr, "SearchConstaints", "Search parameters constraints", false, "SearchConstaints");
	I_END_COMPONENT;

	// reimplemented (iipr::ISearchParams)
	virtual const ISearchConstraints* GetSearchConstraints() const;

private:
	I_REF(iipr::ISearchConstraints, m_searchConstraintsCompPtr);
};


} // namespace iipr


#endif // !iipr_CSearchParamsComp_included
