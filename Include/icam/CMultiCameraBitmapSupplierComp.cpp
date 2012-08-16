#include "icam/CMultiCameraBitmapSupplierComp.h"


// ACF includes
#include "i2d/CAffineTransformation2d.h"


namespace icam
{



// reimplemented (iipr::IMultiBitmapProvider)

int CMultiCameraBitmapSupplierComp::GetBitmapsCount() const
{
	if (m_cameraParamsManagerCompPtr.IsValid()){
		return m_cameraParamsManagerCompPtr->GetParamsSetsCount();	
	}

	return 0;
}


const iimg::IBitmap* CMultiCameraBitmapSupplierComp::GetBitmap(int bitmapIndex) const
{
	const ProductType* productPtr = GetWorkProduct();
	if (productPtr != NULL){
		return productPtr->GetAt(bitmapIndex);
	}

	return NULL;
}


const i2d::ITransformation2d* CMultiCameraBitmapSupplierComp::GetLogTransform(int /*bitmapIndex*/) const
{
	return m_logTransformCompPtr.GetPtr();	
}


// reimplemented (i2d::ICalibrationProvider)

const i2d::ITransformation2d* CMultiCameraBitmapSupplierComp::GetCalibration() const
{
	return m_logTransformCompPtr.GetPtr();
}


// protected methods

// reimplemented (iproc::TSupplierCompWrap)

bool CMultiCameraBitmapSupplierComp::InitializeWork()
{
	if (m_cameraParamsManagerCompPtr.IsValid()){
		int camerasCount = m_cameraParamsManagerCompPtr->GetParamsSetsCount();

		for (int cameraIndex = 0; cameraIndex < camerasCount; cameraIndex++){
			if (m_bitmapAcquisitionCompPtr.IsValid()){
				m_bitmapAcquisitionCompPtr->InitProcessor(m_cameraParamsManagerCompPtr->GetParamsSet(cameraIndex));
			}
		}

		return true;
	}

	return false;
}


int CMultiCameraBitmapSupplierComp::ProduceObject(ProductType& result) const
{
	if (!m_bitmapCompFact.IsValid()){
		return WS_CRITICAL;
	}

	result.Reset();

	int camerasCount = m_cameraParamsManagerCompPtr->GetParamsSetsCount();

	for (int cameraIndex = 0; cameraIndex < camerasCount; cameraIndex++){
		istd::TDelPtr<iimg::IBitmap> cameraBitmapPtr(m_bitmapCompFact.CreateInstance());

		if (cameraBitmapPtr.IsValid() && m_bitmapAcquisitionCompPtr.IsValid()){
			int status = m_bitmapAcquisitionCompPtr->DoProcessing(m_cameraParamsManagerCompPtr->GetParamsSet(cameraIndex), NULL, cameraBitmapPtr.GetPtr());

			switch (status){
				case iproc::IProcessor::TS_OK:
					result.PushBack(cameraBitmapPtr.PopPtr());

					return WS_OK;

				case iproc::IProcessor::TS_CANCELED:
					return WS_CANCELED;

				default:
					return WS_ERROR;
			}
		}
	}

	return WS_CRITICAL;
}


} // namespace icam

