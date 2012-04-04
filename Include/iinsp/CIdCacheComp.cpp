#include "iinsp/CIdCacheComp.h"


namespace iinsp
{


CIdCacheComp::CIdCacheComp()
:	m_currentId(0)
{
}


// reimplemented (iinsp::IIdProvider)

quint32 CIdCacheComp::GetCurrentId() const
{
	return m_currentId;
}


// reimplemented (istd::IChangeable)

bool CIdCacheComp::CopyFrom(const IChangeable& object)
{
	const IIdProvider* providerPtr = dynamic_cast<const IIdProvider*>(&object);
	if (providerPtr != NULL){
		m_currentId = providerPtr->GetCurrentId();

		return true;
	}

	return false;
}


} // namespace iinsp


