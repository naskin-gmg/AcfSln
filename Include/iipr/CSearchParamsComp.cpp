#include "iipr/CSearchParamsComp.h"


namespace iipr
{


// reimplemented (iipr::ISearchParams)

const ISearchConstraints* CSearchParamsComp::GetSearchConstraints() const
{
	if (m_searchConstraintsCompPtr.IsValid()){
		return m_searchConstraintsCompPtr.GetPtr();
	}

	return NULL;
}


} // namespace iipr

