#include <icam/CSnapBitmapSupplierCompBase.h>


// ACF includes
#include <imod/TModelWrap.h>
#include <imath/CGeneralUnitInfo.h>
#include <iprm/TParamsPtr.h>

// ACF-Solutions includes
#include <iproc/IProcessor.h>
#include <icalib/CAffineCalibration2d.h>


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

int CSnapBitmapSupplierCompBase::ProduceObject(ProductType& result) const
{
	result.first.Reset();

	if (!result.second.IsValid()){
		result.second.SetPtr(CreateBitmap());
		if (!result.second.IsValid()){
			SendErrorMessage(0, "Bitmap instance could not be created");

			return WS_FAILED;
		}
	}

	Timer performanceTimer(this, "Image acquisition");

	Q_ASSERT(result.second.IsValid());

	int status = DoSnap(GetModelParametersSet(), *result.second.GetPtr());
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

					if (calibrationPtr->GetTransformationFlags() & i2d::ICalibration2d::TF_AFFINE){
						result.first.SetPtr(calibrationPtr->CreateCombinedCalibration(transformation));
					}
					else{
						result.first.SetCastedOrRemove(calibrationPtr->CloneMe());
					}
				}
				else{
					if (scale != i2d::CVector2d(1, 1)){
						icalib::CAffineCalibration2d* affineCalibrationPtr = new imod::TModelWrap<icalib::CAffineCalibration2d>();
						affineCalibrationPtr->Reset(center, 0, scale);

						result.first.SetPtr(affineCalibrationPtr);
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
	m_defaultCalibrationCompPtr.EnsureInitialized();
}


} // namespace icam


