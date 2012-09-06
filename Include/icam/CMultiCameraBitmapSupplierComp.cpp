#include "icam/CMultiCameraBitmapSupplierComp.h"


// ACF includes
#include "i2d/CAffineTransformation2d.h"


namespace icam
{


// public methods

CMultiCameraBitmapSupplierComp::CMultiCameraBitmapSupplierComp()
	:m_bitmapAcquisitionPtr(NULL),
	m_cameraParamsManagerPtr(NULL),
	m_logTransformPtr(NULL)
{
}


// reimplemented (iipr::IMultiBitmapProvider)

const iprm::ISelectionConstraints* CMultiCameraBitmapSupplierComp::GetBitmapSelectionContraints() const
{
	return NULL;
}


int CMultiCameraBitmapSupplierComp::GetBitmapsCount() const
{
	if (m_cameraParamsManagerPtr != NULL){
		return m_cameraParamsManagerPtr->GetParamsSetsCount();	
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
	return m_logTransformPtr;	
}


// reimplemented (i2d::ICalibrationProvider)

const i2d::ITransformation2d* CMultiCameraBitmapSupplierComp::GetCalibration() const
{
	return m_logTransformPtr;
}


// protected methods

// reimplemented (iproc::TSupplierCompWrap)

bool CMultiCameraBitmapSupplierComp::InitializeWork()
{
	if (m_cameraParamsManagerPtr != NULL){
		int camerasCount = m_cameraParamsManagerPtr->GetParamsSetsCount();

		for (int cameraIndex = 0; cameraIndex < camerasCount; cameraIndex++){
			if (m_bitmapAcquisitionPtr != NULL){
				m_bitmapAcquisitionPtr->InitProcessor(m_cameraParamsManagerPtr->GetParamsSet(cameraIndex));
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

	if (m_cameraParamsManagerPtr != NULL){
		int camerasCount = m_cameraParamsManagerPtr->GetParamsSetsCount();
		if (camerasCount <= 0){
			return WS_ERROR;
		}

		for (int cameraIndex = 0; cameraIndex < camerasCount; cameraIndex++){
			istd::TDelPtr<iimg::IBitmap> cameraBitmapPtr(m_bitmapCompFact.CreateInstance());

			if (cameraBitmapPtr.IsValid() && (m_bitmapAcquisitionPtr != NULL)){
				int status = m_bitmapAcquisitionPtr->DoProcessing(m_cameraParamsManagerPtr->GetParamsSet(cameraIndex), NULL, cameraBitmapPtr.GetPtr());

				switch (status){
					case iproc::IProcessor::TS_OK:
						result.PushBack(cameraBitmapPtr.PopPtr());
						break;

					case iproc::IProcessor::TS_CANCELED:
						result.Reset();

						return WS_CANCELED;

					default:
						result.Reset();

						return WS_ERROR;
				}
			}
		}

		return WS_OK;
	}

	return WS_CRITICAL;
}


// reimplemented (icomp::CComponentBase)

void CMultiCameraBitmapSupplierComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	m_bitmapAcquisitionPtr = m_bitmapAcquisitionCompPtr.GetPtr();

	m_cameraParamsManagerPtr = m_cameraParamsManagerCompPtr.GetPtr();

	m_logTransformPtr = m_logTransformCompPtr.GetPtr();
}


} // namespace icam

