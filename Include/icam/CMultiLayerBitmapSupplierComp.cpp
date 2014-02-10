#include "icam/CMultiLayerBitmapSupplierComp.h"


// ACF includes
#include "imod/TModelWrap.h"
#include "imath/CGeneralUnitInfo.h"
#include "i2d/CAffineTransformation2d.h"
#include "iprm/TParamsPtr.h"


namespace icam
{


// reimplemented (iimg::IMultiBitmapProvider)

int CMultiLayerBitmapSupplierComp::GetBitmapsCount() const
{
	const ProductType* productPtr = GetWorkProduct();
	if (productPtr != NULL){
		return productPtr->second.GetPtr()->GetBitmapsCount();
	}

	return 0;
}


const iimg::IBitmap* CMultiLayerBitmapSupplierComp::GetBitmap(int bitmapIndex) const
{
	const ProductType* productPtr = GetWorkProduct();
	if (productPtr != NULL){
		return productPtr->second.GetPtr()->GetBitmap(bitmapIndex);
	}

	return NULL;
}


const iprm::IOptionsList* CMultiLayerBitmapSupplierComp::GetBitmapListInfo() const
{
	const ProductType* productPtr = GetWorkProduct();
	if (productPtr != NULL){
		return productPtr->second.GetPtr()->GetBitmapListInfo();
	}

	return NULL;
}


// reimplemented (i2d::ICalibrationProvider)

const i2d::ICalibration2d* CMultiLayerBitmapSupplierComp::GetCalibration() const
{
	const ProductType* productPtr = GetWorkProduct();
	if (productPtr != NULL){
		return productPtr->first.GetPtr();
	}

	return NULL;
}


// protected methods

// reimplemented (iproc::TSupplierCompWrap)

bool CMultiLayerBitmapSupplierComp::InitializeWork()
{
	if (m_bitmapAcquisitionCompPtr.IsValid()){
		m_bitmapAcquisitionCompPtr->InitProcessor(GetModelParametersSet());

		return true;
	}

	return false;
}


int CMultiLayerBitmapSupplierComp::ProduceObject(ProductType& result) const
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
		Timer performanceTimer(this, "Image acquisition");

		int status = m_bitmapAcquisitionCompPtr->DoProcessing(GetModelParametersSet(), NULL, result.second.GetPtr());
		switch (status){
			case iproc::IProcessor::TS_OK:
				{
					istd::CIndex2d bitmapSize = result.second->GetBitmap(0)->GetImageSize();
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
	}

	return WS_CRITICAL;
}


} // namespace icam

