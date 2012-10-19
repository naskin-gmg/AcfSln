#include "icam/CMultiCameraBitmapSupplierComp.h"


// ACF includes
#include "i2d/CAffineTransformation2d.h"

#include "istd/CGeneralTimeStamp.h"


namespace icam
{


// public methods

// reimplemented (iipr::IMultiBitmapProvider)

const iprm::ISelectionConstraints* CMultiCameraBitmapSupplierComp::GetBitmapSelectionContraints() const
{
	return NULL;
}


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
	istd::CGeneralTimeStamp timer;
	timer.Start();

	if (!m_bitmapCompFact.IsValid()){
		return WS_CRITICAL;
	}

	result.Reset();

	if (m_cameraParamsManagerCompPtr.IsValid()){
		int camerasCount = m_cameraParamsManagerCompPtr->GetParamsSetsCount();
		if (camerasCount <= 0){
			return WS_ERROR;
		}

		int retVal = WS_OK;

		Timer performanceTimer(this, "Acquisition of image(s)");

		for (int cameraIndex = 0; cameraIndex < camerasCount; cameraIndex++){
			istd::TDelPtr<iimg::IBitmap> cameraBitmapPtr(m_bitmapCompFact.CreateInstance());

			if (cameraBitmapPtr.IsValid() && (m_bitmapAcquisitionCompPtr.IsValid())){
				int status = m_bitmapAcquisitionCompPtr->DoProcessing(
					m_cameraParamsManagerCompPtr->GetParamsSet(cameraIndex), 
					&timer, 
					cameraBitmapPtr.GetPtr());

				switch (status){
				case iproc::IProcessor::TS_OK:
					result.PushBack(cameraBitmapPtr.PopPtr());
					break;

				case iproc::IProcessor::TS_CANCELED:
					result.Reset();
					retVal = WS_CANCELED;
					break;

				default:
					result.Reset();
					retVal = WS_ERROR;
					break;
				}
			}
		}

		return retVal;
	}

	return WS_CRITICAL;
}


} // namespace icam


