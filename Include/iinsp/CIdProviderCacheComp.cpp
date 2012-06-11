#include "iinsp/CIdProviderCacheComp.h"


namespace iinsp
{


CIdProviderCacheComp::CIdProviderCacheComp()
:	m_currentId(0)
{
}


// reimplemented (iinsp::IIdProvider)

quint32 CIdProviderCacheComp::GetCurrentId() const
{
	return m_currentId;
}


// reimplemented (istd::IChangeable)

bool CIdProviderCacheComp::CopyFrom(const IChangeable& object)
{
	const IIdProvider* providerPtr = dynamic_cast<const IIdProvider*>(&object);
	if (providerPtr != NULL){
		m_currentId = providerPtr->GetCurrentId();

		return true;
	}

	return false;
}


} // namespace iinsp


