#include <iipr/CProcessedBitmapSupplierComp.h>


// ACF includes
#include <ilog/CMessage.h>


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


iproc::IProcessor* CProcessedBitmapSupplierBase::GetImageProcessor() const
{
	return m_imageProcessorCompPtr.GetPtr();
}


// reimplemented (iinsp::TSupplierCompWrap)

int CProcessedBitmapSupplierBase::ProduceObject(ProductType& result) const
{
	iproc::IProcessor* imageProcessorPtr = GetImageProcessor();

	if (!m_bitmapProviderCompPtr.IsValid() || (imageProcessorPtr == NULL)){
		return WS_FAILED;
	}

	if (!EnsureBitmapCreated(result)){
		return WS_FAILED;
	}
	Q_ASSERT(result.IsValid());

	const iimg::IBitmap* bitmapPtr = m_bitmapProviderCompPtr->GetBitmap();
	if (bitmapPtr == NULL){
		AddMessage(new ilog::CMessage(ilog::CMessage::IC_ERROR, 0, QObject::tr("No input image"), "BitmapProcessing"));

		return WS_FAILED;
	}

	Timer performanceTimer(this, "Bitmap processing");

	int status = imageProcessorPtr->DoProcessing(GetModelParametersSet(), bitmapPtr, result.GetPtr());
	switch (status){
		case iproc::IProcessor::TS_OK:
			return WS_OK;

		case iproc::IProcessor::TS_CANCELED:
			return WS_CANCELED;

		default:
			return WS_FAILED;
	}

	// should not get here
	return WS_FAILED;
}


// reimplemented (icomp::CComponentBase)

void CProcessedBitmapSupplierBase::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	if (m_bitmapProviderModelCompPtr.IsValid()){
		RegisterSupplierInput(m_bitmapProviderModelCompPtr.GetPtr(), m_bitmapSupplierCompPtr.GetPtr());
	}

	m_bitmapProviderCompPtr.EnsureInitialized();
	m_imageProcessorCompPtr.EnsureInitialized();
}


} // namespace iipr


