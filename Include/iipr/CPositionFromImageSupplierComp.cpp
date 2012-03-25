#include "iipr/CPositionFromImageSupplierComp.h"


// ACF includes
#include "i2d/CCircle.h"

#include "iipr/CSingleFeatureConsumer.h"


namespace iipr
{


// reimplemented (iproc::IValueProvider)

imath::CVarVector CPositionFromImageSupplierComp::GetValue(int /*index*/, int valueTypeId) const
{
	const imath::CVarVector* productPtr = GetWorkProduct();
	if (productPtr != NULL){
		switch (valueTypeId){
		case VTI_AUTO:
			return *productPtr;

		case VTI_POSITION:
			if (productPtr->GetElementsCount() >= 2){
				return i2d::CVector2d(productPtr->GetElement(0), productPtr->GetElement(1));
			}
			break;

		case VTI_RADIUS:
			if (productPtr->GetElementsCount() >= 3){
				return imath::CVarVector(1, productPtr->GetElement(2));
			}
			break;
		}
	}

	return imath::CVarVector();
}


// protected methods

// reimplemented (iproc::TSupplierCompWrap)

int CPositionFromImageSupplierComp::ProduceObject(imath::CVarVector& result) const
{
	if (		m_bitmapProviderCompPtr.IsValid() &&
				m_processorCompPtr.IsValid()){
		const iimg::IBitmap* bitmapPtr = m_bitmapProviderCompPtr->GetBitmap();
		if (bitmapPtr != NULL){
			iprm::IParamsSet* paramsSetPtr = GetModelParametersSet();

			CSingleFeatureConsumer consumer(CSingleFeatureConsumer::FP_HEAVIEST);
			int positionState = m_processorCompPtr->DoProcessing(
							paramsSetPtr,
							bitmapPtr,
							&consumer);

			if (positionState != iproc::IProcessor::TS_OK){
				return WS_ERROR;
			}

			const i2d::ITransformation2d* logicalTransformPtr = NULL;	
			if (m_calibrationProviderCompPtr.IsValid()){
				logicalTransformPtr = m_calibrationProviderCompPtr->GetCalibration();
			}

			const i2d::CPosition2d* positionPtr = dynamic_cast<const i2d::CPosition2d*>(consumer.GetFeature());
			if (positionPtr == NULL){
				return WS_ERROR;
			}

			if (logicalTransformPtr != NULL){
				i2d::CPosition2d transofrmedPosition = *positionPtr;
				if (!transofrmedPosition.Transform(*logicalTransformPtr)){
					return WS_ERROR;
				}
	
				result = transofrmedPosition.GetPosition();
			}
			else{
				result = positionPtr->GetPosition();
			}

			const i2d::CCircle* circlePtr = dynamic_cast<const i2d::CCircle*>(positionPtr);
			if (circlePtr != NULL){
				result.SetElementsCount(3);
				result[2] = circlePtr->GetRadius();

				if (logicalTransformPtr != NULL){
					i2d::CVector2d input = positionPtr->GetPosition();
					input.SetX(input.GetX() + circlePtr->GetRadius());

					if (!logicalTransformPtr->GetPositionAt(input, input)){
						return WS_ERROR;
					}

					result[2] = i2d::CVector2d(result[0], result[1]).GetDistance(input);
				}
			}

			return WS_OK;
		}
	}

	return WS_CRITICAL;
}


// reimplemented (icomp::CComponentBase)

void CPositionFromImageSupplierComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	if (m_bitmapProviderModelCompPtr.IsValid()){
		RegisterSupplierInput(m_bitmapProviderModelCompPtr.GetPtr());
	}
}


} // namespace iipr


