// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#include <iinsp/CIdProviderCacheComp.h>


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

bool CIdProviderCacheComp::CopyFrom(const IChangeable& object, CompatibilityMode /*mode*/)
{
	const IIdProvider* providerPtr = dynamic_cast<const IIdProvider*>(&object);
	if (providerPtr != NULL){
		m_currentId = providerPtr->GetCurrentId();

		return true;
	}

	return false;
}


} // namespace iinsp


