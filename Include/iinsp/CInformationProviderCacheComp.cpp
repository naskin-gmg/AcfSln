#include "iinsp/CInformationProviderCacheComp.h"


namespace iinsp
{


CInformationProviderCacheComp::CInformationProviderCacheComp()
:	m_category(istd::IInformationProvider::IC_NONE),
	m_flags(0),
	m_id(0)
{
}


// reimplemented (iinsp::IInformationProvider)

QDateTime CInformationProviderCacheComp::GetInformationTimeStamp() const
{
	return m_timeStamp;
}


istd::IInformationProvider::InformationCategory CInformationProviderCacheComp::GetInformationCategory() const
{
	return m_category;
}


int CInformationProviderCacheComp::GetInformationId() const
{
	return m_id;
}


QString CInformationProviderCacheComp::GetInformationDescription() const
{
	return m_description;
}


QString CInformationProviderCacheComp::GetInformationSource() const
{
	if (m_source.isEmpty() && m_defaultSourceAttrPtr.IsValid()){
		return *m_defaultSourceAttrPtr;
	}

	return m_source;
}


int CInformationProviderCacheComp::GetInformationFlags() const
{
	return m_flags;
}


// reimplemented (istd::IChangeable)

bool CInformationProviderCacheComp::CopyFrom(const IChangeable& object, CompatibilityMode /*mode*/)
{
	const istd::IInformationProvider* providerPtr = dynamic_cast<const IInformationProvider*>(&object);
	if (providerPtr != NULL){
		m_timeStamp = providerPtr->GetInformationTimeStamp();
		m_category = providerPtr->GetInformationCategory();
		m_id = providerPtr->GetInformationId();
		m_description = providerPtr->GetInformationDescription();
		m_source = providerPtr->GetInformationSource();
		m_flags = providerPtr->GetInformationFlags();

		return true;
	}

	return false;
}


} // namespace iinsp


