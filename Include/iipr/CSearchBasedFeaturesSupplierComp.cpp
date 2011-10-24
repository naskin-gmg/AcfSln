#include "iipr/CSearchBasedFeaturesSupplierComp.h"


namespace iipr
{


// reimplemented (iipr::IFeaturesProvider)

iipr::IFeaturesProvider::Features CSearchBasedFeaturesSupplierComp::GetFeatures() const
{
	const CFeaturesContainer* containerPtr = GetWorkProduct();
	if (containerPtr != NULL){
		return containerPtr->GetFeatures();
	}

	return iipr::IFeaturesProvider::Features();
}


// protected methods

// reimplemented (iproc::TSupplierCompWrap)

int CSearchBasedFeaturesSupplierComp::ProduceObject(CFeaturesContainer& result) const
{
	if (		m_bitmapProviderCompPtr.IsValid() &&
				m_searchProcessorCompPtr.IsValid()){
		const iimg::IBitmap* bitmapPtr = m_bitmapProviderCompPtr->GetBitmap();
		if (bitmapPtr != NULL){
			iprm::IParamsSet* paramsSetPtr = GetModelParametersSet();

			int searchState = m_searchProcessorCompPtr->DoProcessing(
							paramsSetPtr,
							bitmapPtr,
							&result);

			if (searchState != iproc::IProcessor::TS_OK){
				return WS_ERROR;
			}

			return WS_OK;
		}
	}

	return WS_CRITICAL;
}


// reimplemented (icomp::CComponentBase)

void CSearchBasedFeaturesSupplierComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	if (m_bitmapProviderModelCompPtr.IsValid()){
		RegisterSupplierInput(m_bitmapProviderModelCompPtr.GetPtr());
	}
}


} // namespace iipr


