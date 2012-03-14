#include "iinsp/CGeneralResultCacheComp.h"


namespace iinsp
{


CGeneralResultCacheComp::CGeneralResultCacheComp()
:	m_result(istd::IInformation::IC_CRITICAL)
{
}


// reimplemented (iinsp::IGeneralResultProvider)

istd::IInformation::InformationCategory CGeneralResultCacheComp::GetGeneralResult() const
{
	return m_result;
}


// reimplemented (istd::IChangeable)

bool CGeneralResultCacheComp::CopyFrom(const IChangeable& object)
{
	const IGeneralResultProvider* providerPtr = dynamic_cast<const IGeneralResultProvider*>(&object);
	if (providerPtr != NULL){
		m_result = providerPtr->GetGeneralResult();

		return true;
	}

	return false;
}


} // namespace iinsp


