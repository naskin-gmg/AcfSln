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
			int caliperState = m_processorCompPtr->DoProcessing(
							paramsSetPtr,
							bitmapPtr,
							&consumer);

			if (caliperState != iproc::IProcessor::TS_OK){
				return WS_ERROR;
			}

			const i2d::CPosition2d* positionPtr = dynamic_cast<const i2d::CPosition2d*>(consumer.GetFeature());
			if (positionPtr == NULL){
				return WS_ERROR;
			}

			result = positionPtr->GetPosition();

			const i2d::CCircle* circlePtr = dynamic_cast<const i2d::CCircle*>(positionPtr);
			if (circlePtr != NULL){
				result.SetElementsCount(3);
				result[2] = circlePtr->GetRadius();
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


