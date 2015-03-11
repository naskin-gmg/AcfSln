#include "iipr/CProcessedBitmapSupplierComp.h"


namespace iipr
{


// reimplemented (iimg::IBitmapProvider)

const iimg::IBitmap* CProcessedBitmapSupplierBase::GetBitmap() const
{
	const ProductType* productPtr = GetWorkProduct();
	if (productPtr != NULL){
		return productPtr->GetPtr();
	}

	return NULL;
}


//protected methods

bool CProcessedBitmapSupplierBase::EnsureBitmapCreated(ProductType& result) const
{
	if (!result.IsValid()){
		result.SetPtr(CreateBitmap());
	}

	return result.IsValid();
}


// reimplemented (iinsp::TSupplierCompWrap)

int CProcessedBitmapSupplierBase::ProduceObject(ProductType& result) const
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

void CProcessedBitmapSupplierBase::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	if (m_bitmapProviderModelCompPtr.IsValid() && m_bitmapSupplierCompPtr.IsValid()){
		RegisterSupplierInput(m_bitmapProviderModelCompPtr.GetPtr(), m_bitmapSupplierCompPtr.GetPtr());
	}

	m_bitmapProviderCompPtr.EnsureInitialized();
	m_imageProcessorCompPtr.EnsureInitialized();
	m_defaultCalibrationCompPtr.EnsureInitialized();
}


} // namespace iipr


