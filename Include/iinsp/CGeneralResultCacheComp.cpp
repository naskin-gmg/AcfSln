#include "iinsp/CGeneralResultCacheComp.h"


namespace iinsp
{


CGeneralResultCacheComp::CGeneralResultCacheComp()
:	m_result(GR_CRITICAL)
{
}


// reimplemented (iinsp::IGeneralResultProvider)

IGeneralResultProvider::GeneralResult CGeneralResultCacheComp::GetGeneralResult() const
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


