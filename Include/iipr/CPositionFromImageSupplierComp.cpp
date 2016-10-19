#include <iipr/CPositionFromImageSupplierComp.h>


// ACF includes
#include <imod/TModelWrap.h>
#include <ilog/TExtMessage.h>
#include <i2d/CCircle.h>
#include <i2d/CAffineCalibration2d.h>

// ACF-Solutions includes
#include <iipr/CSingleFeatureConsumer.h>


namespace iipr
{


// reimplemented (iipr::IFeaturesProvider)

int CPositionFromImageSupplierComp::GetFeaturesCount() const
{
	const ProductType* productPtr = GetWorkProduct();
	if (productPtr != NULL){
		return productPtr->GetFeaturesCount();
	}

	return 0;
}


const imeas::INumericValue& CPositionFromImageSupplierComp::GetFeature(int index) const
{
	const ProductType* productPtr = GetWorkProduct();
	if (productPtr != NULL){
		return productPtr->GetFeature(index);
	}

	static imeas::CSimpleNumericValue empty;

	return empty;
}


// reimplemented (i2d::ICalibrationProvider)

const i2d::ICalibration2d* CPositionFromImageSupplierComp::GetCalibration() const
{
	return m_outputCalibrationPtr.GetPtr();
}


// protected methods

// reimplemented (iinsp::TSupplierCompWrap)

int CPositionFromImageSupplierComp::ProduceObject(ProductType& result) const
{
	result.ResetFeatures();

	m_outputCalibrationPtr.Reset();

	if (		m_bitmapProviderCompPtr.IsValid() &&
				m_processorCompPtr.IsValid()){
		const iimg::IBitmap* bitmapPtr = m_bitmapProviderCompPtr->GetBitmap();
		if (bitmapPtr == NULL){
			AddMessage(new ilog::CMessage(ilog::CMessage::IC_ERROR, 0, QObject::tr("No input image"), "PositionFinder"));

			return WS_ERROR;
		}

		iprm::IParamsSet* paramsSetPtr = GetModelParametersSet();

		Timer performanceTimer(this, "Calculation of position");

		int positionState = m_processorCompPtr->DoProcessing(
						paramsSetPtr,
						bitmapPtr,
						&result);

		if (positionState != iproc::IProcessor::TS_OK){
			return WS_ERROR;
		}

		const i2d::ICalibration2d* inputCalibrationPtr = NULL;
		if (m_calibrationProviderCompPtr.IsValid()){
			inputCalibrationPtr = m_calibrationProviderCompPtr->GetCalibration();
		}

		if (result.GetFeaturesCount() < 1){
			return WS_ERROR;
		}

		const i2d::CObject2dBase* positionPtr = dynamic_cast<const i2d::CObject2dBase*>(&result.GetFeature(0));
		if (positionPtr == NULL){
			return WS_ERROR;
		}

		i2d::CVector2d resultVector;

		const i2d::CCircle* circlePtr = dynamic_cast<const i2d::CCircle*>(positionPtr);
		const i2d::CLine2d* linePtr = dynamic_cast<const i2d::CLine2d*>(positionPtr);
		if (circlePtr != NULL){
			i2d::CCircle transformedCircle;
			if (!transformedCircle.CopyFrom(*circlePtr, istd::IChangeable::CM_CONVERT)){
				return WS_ERROR;
			}

			resultVector = transformedCircle.GetPosition();

			ilog::TExtMessageModel<i2d::CCircle>* messagePtr = new ilog::TExtMessageModel<i2d::CCircle>(
						istd::IInformationProvider::IC_INFO,
						iinsp::CSupplierCompBase::MI_GEOMETRICAL_RESULT,
						QString("Radius: %3, Pos.: (%1, %2)").arg(circlePtr->GetPosition().GetX()).arg(circlePtr->GetPosition().GetY()).arg(circlePtr->GetRadius()),
						"PositionFinder");
			messagePtr->i2d::CCircle::CopyFrom(*circlePtr, istd::IChangeable::CM_WITH_REFS);
			AddMessage(messagePtr);
		}
		else if (linePtr != NULL){
			i2d::CLine2d transformedLine;
			if (!transformedLine.CopyFrom(*linePtr, istd::IChangeable::CM_CONVERT)){
				return WS_ERROR;
			}

			resultVector = transformedLine.GetPoint1();

			ilog::TExtMessageModel<i2d::CLine2d>* messagePtr = new ilog::TExtMessageModel<i2d::CLine2d>(
						istd::IInformationProvider::IC_INFO,
						iinsp::CSupplierCompBase::MI_GEOMETRICAL_RESULT,
						QString("Line: (%1, %2)->(%1, %2)").arg(linePtr->GetPoint1().GetX()).arg(linePtr->GetPoint1().GetY()).arg(linePtr->GetPoint2().GetX()).arg(linePtr->GetPoint2().GetY()),
						"PositionFinder");
			messagePtr->i2d::CLine2d::CopyFrom(*linePtr, istd::IChangeable::CM_CONVERT);
			AddMessage(messagePtr);
		}
		else{
			i2d::CPosition2d transformedPosition;
			if (!transformedPosition.CopyFrom(*positionPtr, istd::IChangeable::CM_CONVERT)){
				return WS_ERROR;
			}

			resultVector = transformedPosition.GetPosition();

			ilog::TExtMessageModel<i2d::CPosition2d>* messagePtr = new ilog::TExtMessageModel<i2d::CPosition2d>(
						istd::IInformationProvider::IC_INFO,
						iinsp::CSupplierCompBase::MI_GEOMETRICAL_RESULT,
						QString("R: %3, Pos.: (%1, %2)").arg(positionPtr->GetCenter().GetX()).arg(positionPtr->GetCenter().GetY()),
						"PositionFinder");
			messagePtr->i2d::CPosition2d::CopyFrom(*positionPtr, istd::IChangeable::CM_CONVERT);
			AddMessage(messagePtr);
		}

		i2d::CVector2d originalZeroPos(0, 0);
		if (inputCalibrationPtr != NULL){
			originalZeroPos = inputCalibrationPtr->GetValueAt(i2d::CVector2d(0, 0));
		}

		i2d::CAffineCalibration2d* outputTransformPtr = new imod::TModelWrap<i2d::CAffineCalibration2d>();
		outputTransformPtr->Reset(resultVector - originalZeroPos);
		m_outputCalibrationPtr.SetPtr(outputTransformPtr);

		if (inputCalibrationPtr != NULL){
			m_outputCalibrationPtr.SetPtr(m_outputCalibrationPtr->CreateCombinedCalibration(*inputCalibrationPtr));
		}

		return WS_OK;
	}

	return WS_CRITICAL;
}


// reimplemented (icomp::CComponentBase)

void CPositionFromImageSupplierComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	if (m_bitmapProviderCompPtr.IsValid() && m_bitmapProviderModelCompPtr.IsValid()){
		RegisterSupplierInput(m_bitmapProviderModelCompPtr.GetPtr(), m_bitmapSupplierCompPtr.GetPtr());
	}

	// Initialize components
	m_calibrationProviderCompPtr.IsValid();
	m_processorCompPtr.IsValid();
}


} // namespace iipr


