#include "icam/CSnapBitmapSupplierComp.h"


// ACF includes
#include "i2d/CAffineTransformation2d.h"


namespace icam
{


// reimplemented (iipr::IBitmapProvider)

const iimg::IBitmap* CSnapBitmapSupplierComp::GetBitmap() const
{
	const ProductType* productPtr = GetWorkProduct();
	if (productPtr != NULL){
		return productPtr->second.GetPtr();
	}
	else{
		return NULL;
	}
}


const i2d::ITransformation2d* CSnapBitmapSupplierComp::GetLogTransform() const
{
	const ProductType* productPtr = GetWorkProduct();
	if (productPtr != NULL){
		return productPtr->first.GetPtr();
	}
	else{
		return NULL;
	}
}


// protected methods

// reimplemented (iproc::TSupplierCompWrap)

int CSnapBitmapSupplierComp::ProduceObject(ProductType& result) const
{
	if (!m_bitmapCompFact.IsValid()){
		return WS_CRITICAL;
	}

	result.first.Reset();

	if (!result.second.IsValid()){
		result.second.SetPtr(m_bitmapCompFact.CreateInstance());
		if (!result.second.IsValid()){
			return WS_CRITICAL;
		}
	}

	if (result.second.IsValid() && m_bitmapAcquisitionCompPtr.IsValid()){
		int status = m_bitmapAcquisitionCompPtr->DoProcessing(GetModelParametersSet(), NULL, result.second.GetPtr());
		switch (status){
		case iproc::IProcessor::TS_OK:
			{
				istd::CIndex2d bitmapSize = result.second->GetImageSize();
				i2d::CVector2d center(bitmapSize.GetX() * 0.5, bitmapSize.GetY() * 0.5);
				if (m_calibrationCompPtr.IsValid()){
					i2d::CAffineTransformation2d transform;
					transform.Reset(-center);
					result.first.SetPtr(m_calibrationCompPtr->CreateCombinedTransformation(transform));
				}
				else{
					i2d::CAffineTransformation2d* transformPtr = new i2d::CAffineTransformation2d();
					transformPtr->Reset(-center);
					result.first.SetPtr(transformPtr);
				}
			}
			return WS_OK;

		case iproc::IProcessor::TS_CANCELED:
			return WS_CANCELED;

		default:
			return WS_ERROR;
		}
	}

	return WS_CRITICAL;
}


} // namespace icam

