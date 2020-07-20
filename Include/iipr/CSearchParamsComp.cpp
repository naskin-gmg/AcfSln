#include <iipr/CSearchParamsComp.h>


namespace iipr
{


// public methods

// reimplemented (iipr::ISearchParams)

const ISearchConstraints* CSearchParamsComp::GetSearchConstraints() const
{
	if (m_searchConstraintsCompPtr.IsValid()){
		return m_searchConstraintsCompPtr.GetPtr();
	}

	return NULL;
}


// protected methods

// reimplemented (icomp::CComponentBase)

void CSearchParamsComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	SetNominalModelsCount(*m_defaultModelOccuranceAttrPtr);
}


} // namespace iipr


