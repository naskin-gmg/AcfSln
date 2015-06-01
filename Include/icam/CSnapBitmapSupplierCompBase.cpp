#include "icam/CSnapBitmapSupplierCompBase.h"

// ACF includes
#include "imod/TModelWrap.h"
#include "imath/CGeneralUnitInfo.h"
#include "i2d/CAffineTransformation2d.h"
#include "iprm/TParamsPtr.h"


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

		return WS_CRITICAL;
	}

	if (!result.second.IsValid()){
		result.second.SetPtr(CreateBitmap());
		if (!result.second.IsValid()){
			SendErrorMessage(0, "Bitmap instance could not be created");

			return WS_ERROR;
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

				const imeas::INumericValue *scalePtr = NULL;

				iprm::TParamsPtr<imeas::INumericValue> scaleParamPtr(
							GetModelParametersSet(),
							m_scaleParamIdAttrPtr,
							m_defaultScaleValueCompPtr,
							false);
				if (scaleParamPtr.IsValid()){
					imath::CVarVector scaleValues = scalePtr->GetValues();
					if (scaleValues.GetElementsCount() >= 2){
						scale = i2d::CVector2d(scaleValues[0], scaleValues[1]);
					}
					else if (scaleValues.GetElementsCount() >= 1){
						scale = i2d::CVector2d(scaleValues[0], scaleValues[0]);
					}
				}

				if (m_calibrationCompPtr.IsValid()){
					i2d::CAffineTransformation2d calibration;
					calibration.Reset(center, 0, scale);
					if (m_calibratedUnitInfoCompPtr.IsValid()){
						calibration.SetArgumentUnitInfo(m_calibratedUnitInfoCompPtr.GetPtr());
					}

					result.first.SetPtr(m_calibrationCompPtr->CreateCombinedCalibration(calibration));
				}
				else{
					i2d::CAffineTransformation2d* calibrationPtr = new imod::TModelWrap<i2d::CAffineTransformation2d>();
					calibrationPtr->Reset(center, 0, scale);
					if (m_calibratedUnitInfoCompPtr.IsValid()){
						calibrationPtr->SetArgumentUnitInfo(m_calibratedUnitInfoCompPtr.GetPtr());
					}

					result.first.SetPtr(calibrationPtr);
				}
			}
			return WS_OK;

		case iproc::IProcessor::TS_CANCELED:
			return WS_CANCELED;

		default:
			return WS_ERROR;
	}

	return WS_CRITICAL;
}


// reimplemented (icomp::CComponentBase)

void CSnapBitmapSupplierCompBase::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	// initialize components
	m_bitmapAcquisitionCompPtr.EnsureInitialized();
	m_calibrationCompPtr.EnsureInitialized();
	m_defaultScaleValueCompPtr.EnsureInitialized();
	m_calibratedUnitInfoCompPtr.EnsureInitialized();
}


} // namespace icam

