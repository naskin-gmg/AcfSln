#include <icam/CSnapBitmapSupplierCompBase.h>

// ACF includes
#include <imod/TModelWrap.h>
#include <imath/CGeneralUnitInfo.h>
#include <iprm/TParamsPtr.h>


// ACF-Solutions includes
#include <icalib/CAffineCalibration2d.h>
#include <icam/CCalibratedCameraComp.h>


namespace icam
{


// public methods

// reimplemented (iimg::IBitmapProvider)

const iimg::IBitmap* CSnapBitmapSupplierCompBase::GetBitmap() const
{
	const ProductType* productPtr = GetWorkProduct();
	if (productPtr != NULL){
		return productPtr->second.GetPtr();
	}

	return NULL;
}


// reimplemented (i2d::ICalibrationProvider)

const i2d::ICalibration2d* CSnapBitmapSupplierCompBase::GetCalibration() const
{
	const ProductType* productPtr = GetWorkProduct();
	if (productPtr != NULL){
		return productPtr->first.GetPtr();
	}

	return NULL;
}


// protected methods

// reimplemented (iinsp::TSupplierCompWrap)

bool CSnapBitmapSupplierCompBase::InitializeWork()
{
	if (m_bitmapAcquisitionCompPtr.IsValid()){
		m_bitmapAcquisitionCompPtr->InitProcessor(GetModelParametersSet());

		return true;
	}

	return false;
}


int CSnapBitmapSupplierCompBase::ProduceObject(ProductType& result) const
{
	result.first.Reset();

	if (!m_bitmapAcquisitionCompPtr.IsValid()){
		SendCriticalMessage(0, "Bad component architecture, 'BitmapAcquisition' component reference is not set");

		return WS_FAILED;
	}

	if (!result.second.IsValid()){
		result.second.SetPtr(CreateBitmap());
		if (!result.second.IsValid()){
			SendErrorMessage(0, "Bitmap instance could not be created");

			return WS_FAILED;
		}
	}

	Timer performanceTimer(this, "Image acquisition");

	int status = m_bitmapAcquisitionCompPtr->DoProcessing(GetModelParametersSet(), NULL, result.second.GetPtr());
	switch (status){
		case iproc::IProcessor::TS_OK:
			{
				istd::CIndex2d bitmapSize = result.second->GetImageSize();
				i2d::CVector2d center(bitmapSize.GetX() * 0.5, bitmapSize.GetY() * 0.5);

				i2d::CVector2d scale(1, 1);

				iprm::TParamsPtr<imeas::INumericValue> scaleParamPtr(GetModelParametersSet(), m_scaleParamIdAttrPtr, m_defaultScaleValueCompPtr, false);
				if (scaleParamPtr.IsValid()){
					imath::CVarVector scaleValues = scaleParamPtr->GetValues();
					if (scaleValues.GetElementsCount() >= 2){
						scale = i2d::CVector2d(scaleValues[0], scaleValues[1]);
					}
					else if (scaleValues.GetElementsCount() >= 1){
						scale = i2d::CVector2d(scaleValues[0], scaleValues[0]);
					}
				}

				iprm::TParamsPtr<i2d::ICalibration2d> calibrationPtr(GetModelParametersSet(), m_calibrationIdAttrPtr, m_defaultCalibrationCompPtr, false);
				if (calibrationPtr.IsValid()){
					i2d::CAffineTransformation2d transformation;
					transformation.Reset(center, 0, scale);

					result.first.SetPtr(calibrationPtr->CreateCombinedCalibration(transformation));
				}
				else{
					if (scale != i2d::CVector2d(1, 1)){
						icalib::CAffineCalibration2d* calibrationPtr = new imod::TModelWrap<icalib::CAffineCalibration2d>();
						calibrationPtr->Reset(center, 0, scale);

						result.first.SetPtr(calibrationPtr);
					}
					else{
						result.first.Reset();
					}
				}
			}
			return WS_OK;

		case iproc::IProcessor::TS_CANCELED:
			return WS_CANCELED;

		default:
			return WS_FAILED;
	}

	return WS_FAILED;
}


// reimplemented (icomp::CComponentBase)

void CSnapBitmapSupplierCompBase::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	// initialize components
	m_bitmapAcquisitionCompPtr.EnsureInitialized();
	m_defaultCalibrationCompPtr.EnsureInitialized();
}


} // namespace icam


