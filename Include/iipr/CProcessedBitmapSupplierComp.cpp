#include "iipr/CProcessedBitmapSupplierComp.h"


namespace iipr
{


// reimplemented (iimg::IBitmapProvider)

const iimg::IBitmap* CProcessedBitmapSupplierComp::GetBitmap() const
{
	const ProductType* productPtr = GetWorkProduct();
	if (productPtr != NULL){
		return productPtr->GetPtr();
	}

	return NULL;
}


//protected methods

bool CProcessedBitmapSupplierComp::EnsureBitmapCreated(ProductType& result) const
{
	if (!m_bitmapCompFact.IsValid()){
		return false;
	}

	if (!result.IsValid()){
		result.SetPtr(m_bitmapCompFact.CreateInstance());
	}

	return result.IsValid();
}


// reimplemented (iinsp::TSupplierCompWrap)

int CProcessedBitmapSupplierComp::ProduceObject(ProductType& result) const
{
	if (!m_bitmapProviderCompPtr.IsValid() || !m_imageProcessorCompPtr.IsValid()){
		return WS_CRITICAL;
	}

	if (!EnsureBitmapCreated(result)){
		return WS_CRITICAL;
	}
	Q_ASSERT(result.IsValid());

	const iimg::IBitmap* bitmapPtr = m_bitmapProviderCompPtr->GetBitmap();
	if (bitmapPtr == NULL){
		return WS_ERROR;
	}

	Timer performanceTimer(this, "Bitmap processing");

	int status = m_imageProcessorCompPtr->DoProcessing(GetModelParametersSet(), bitmapPtr, result.GetPtr());
	switch (status){
		case iproc::IProcessor::TS_OK:
			return WS_OK;

		case iproc::IProcessor::TS_CANCELED:
			return WS_CANCELED;

		default:
			return WS_ERROR;
	}
}


// reimplemented (icomp::CComponentBase)

void CProcessedBitmapSupplierComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	if (m_bitmapProviderModelCompPtr.IsValid()){
		RegisterSupplierInput(m_bitmapProviderModelCompPtr.GetPtr(), m_bitmapSupplierCompPtr.GetPtr());
	}
}


} // namespace iipr


