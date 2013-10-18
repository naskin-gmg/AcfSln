#include "iipr/CCircleFindProcessorComp.h"


// ACF includes
#include "imath/CVarMatrix.h"
#include "ilog/TExtMessage.h"
#include "i2d/CAnnulusSegment.h"
#include "iprm/CParamsSet.h"
#include "iprm/TParamsPtr.h"

// IACF includes
#include "iipr/CCaliperFeature.h"
#include "iipr/CFeaturesContainer.h"
#include "iipr/CSingleFeatureConsumer.h"


namespace iipr
{


CCircleFindProcessorComp::CCircleFindProcessorComp()
{
}


// reimplemented (iipr::IImageToFeatureProcessor)

int CCircleFindProcessorComp::DoExtractFeatures(
			const iprm::IParamsSet* paramsPtr,
			const iimg::IBitmap& image,
			IFeaturesConsumer& results)
{
	if (!m_featuresMapperCompPtr.IsValid() || (paramsPtr == NULL)){
		return TS_INVALID;
	}

	Rays inRays;
	Rays outRays;

	iprm::TParamsPtr<istd::IChangeable> aoiObjectPtr(paramsPtr, *m_aoiParamIdAttrPtr);
	if (!aoiObjectPtr.IsValid()){
		return TS_INVALID;
	}

	iprm::TParamsPtr<iipr::ICircleFinderParams> circleFinderParamsPtr(paramsPtr, *m_circleFinderParamsIdAttrPtr);
	if (!circleFinderParamsPtr.IsValid()){
		return TS_INVALID;
	}

	i2d::CLine2d projectionLine;
	projectionLine.SetCalibration(m_resultCalibrationCompPtr.GetPtr());

	iprm::CParamsSet extendedParamsSet;
	extendedParamsSet.SetEditableParameter(*m_slaveLineIdAttrPtr, &projectionLine);
	extendedParamsSet.SetSlaveSet(paramsPtr);

	i2d::CVector2d center;

	if (!AddAoiToRays(*aoiObjectPtr, extendedParamsSet, image, *circleFinderParamsPtr, inRays, outRays, projectionLine, center)){
		return TS_INVALID;
	}

	if (*m_searchForAnnulusAttrPtr){
		istd::TDelPtr<AnnulusFeature> featurePtr(new AnnulusFeature());

		bool isOk = CalculateAnnulus(center, inRays, outRays, *featurePtr);
		AddIntermediateResults(inRays);
		AddIntermediateResults(outRays);

		if (isOk){
			results.AddFeature(featurePtr.PopPtr());

			return TS_OK;
		}
	}
	else{
		istd::TDelPtr<CircleFeature> featurePtr(new CircleFeature());
		Rays& usedRays = (inRays.size() >= outRays.size())? inRays: outRays;

		bool isOk = CalculateCircle(
								center,
								circleFinderParamsPtr->IsOutlierEliminationEnabled(),
								circleFinderParamsPtr->GetMinOutlierDistance(),
								usedRays,
								*featurePtr);

		AddIntermediateResults(usedRays);

		if (isOk){
			results.AddFeature(featurePtr.PopPtr());

			return TS_OK;
		}
	}

	return TS_INVALID;
}


// reimplemented (iproc::IProcessor)

int CCircleFindProcessorComp::DoProcessing(
			const iprm::IParamsSet* paramsPtr,
			const istd::IPolymorphic* inputPtr,
			istd::IChangeable* outputPtr,
			ibase::IProgressManager* /*progressManagerPtr*/)
{
	if (outputPtr == NULL){
		return TS_OK;
	}

	const iimg::IBitmap* imagePtr = dynamic_cast<const iimg::IBitmap*>(inputPtr);
	IFeaturesConsumer* consumerPtr = dynamic_cast<IFeaturesConsumer*>(outputPtr);

	if (		(imagePtr == NULL) ||
				(consumerPtr == NULL)){
		return TS_INVALID;
	}

	return DoExtractFeatures(paramsPtr, *imagePtr, *consumerPtr);
}


// protected methods

bool CCircleFindProcessorComp::AddAoiToRays(
			const istd::IChangeable& aoiObject,
			const iprm::IParamsSet& params,
			const iimg::IBitmap& image,
			const iipr::ICircleFinderParams& circleFinderParams,
			Rays& inRays,
			Rays& outRays,
			i2d::CLine2d& projectionLine,
			i2d::CVector2d& center)
{
	const iprm::CParamsSet* setPtr = dynamic_cast<const iprm::CParamsSet*>(&aoiObject);
	if (setPtr != NULL){
		int aoisCount = 0;

		const iprm::CParamsSet::ParameterInfos& infos = setPtr->GetParameterInfos();
		int paramsCount = infos.GetCount();
		for (int i = 0; i < paramsCount; ++i){
			const iprm::CParamsSet::ParameterInfo* infoPtr = infos.GetAt(i);
			if ((infoPtr != NULL) && infoPtr->parameterPtr.IsValid()){
				if (!AddAoiToRays(*infoPtr->parameterPtr, params, image, circleFinderParams, inRays, outRays, projectionLine, center)){
					return false;
				}

				++aoisCount;
			}
		}

		return (aoisCount > 0);
	}

	const i2d::CAnnulus* annulusAoiPtr = dynamic_cast<const i2d::CAnnulus*>(&aoiObject);
	if (annulusAoiPtr != NULL){
		const i2d::ICalibration2d* aoiCalibrationPtr = annulusAoiPtr->GetCalibration();
		i2d::CLine2d aoiLine;
		aoiLine.SetCalibration(aoiCalibrationPtr);

		double beginAngle = 0;
		double endAngle = 2 * I_PI;
		const i2d::CAnnulusSegment* segmentPtr = dynamic_cast<const i2d::CAnnulusSegment*>(annulusAoiPtr);
		if (segmentPtr != NULL){
			beginAngle = segmentPtr->GetBeginAngle();
			endAngle = segmentPtr->GetEndAngle();
		}

		double minRadius = annulusAoiPtr->GetInnerRadius();
		double maxRadius = annulusAoiPtr->GetOuterRadius();
		center = annulusAoiPtr->GetPosition();

		istd::TDelPtr<imeas::INumericValueProvider> caliperFeaturesProviderPtr;
		IFeaturesConsumer* caliperFeaturesConsumerPtr;

		if (circleFinderParams.GetCaliperMode() == ICircleFinderParams::CCM_BEST){
			CFeaturesContainer* containerPtr = new CFeaturesContainer;

			caliperFeaturesProviderPtr.SetPtr(containerPtr);
			caliperFeaturesConsumerPtr = containerPtr;
		}
		else{
			CSingleFeatureConsumer* containerPtr = new CSingleFeatureConsumer(CSingleFeatureConsumer::FP_FIRST);

			caliperFeaturesProviderPtr.SetPtr(containerPtr);
			caliperFeaturesConsumerPtr = containerPtr;
		}

		int stepsCount = int((minRadius + maxRadius) * (endAngle - beginAngle) * 0.5 + 1);
		if (circleFinderParams.GetRaysCount() >= 3){
			stepsCount = qMin(stepsCount, circleFinderParams.GetRaysCount());
		}

		for (int i = 0; i < stepsCount; ++i){
			double alpha = (i + 0.5) / stepsCount;
			double angle = alpha * (endAngle - beginAngle) + beginAngle;

			i2d::CVector2d directionVector;
			directionVector.Init(angle);

			aoiLine.SetPoint1(center + directionVector * minRadius);
			aoiLine.SetPoint2(center + directionVector * maxRadius);

			projectionLine.CopyFrom(aoiLine, istd::IChangeable::CM_CONVERT);

			caliperFeaturesConsumerPtr->ResetFeatures();
			m_slaveProcessorCompPtr->DoProcessing(&params, &image, caliperFeaturesConsumerPtr);

			AddProjectionResultsToRays(projectionLine, params, *caliperFeaturesProviderPtr, inRays, outRays);
		}

		return true;
	}

	return false;
}


bool CCircleFindProcessorComp::CalculateCircle(
			const i2d::CVector2d& center,
			bool removeOutliers,
			double minOutliersDistance,
			Rays& rays,
			CircleFeature& result)
{
	int raysCount = int(rays.size());

	imath::CVarMatrix matrix;
	imath::CVarMatrix destVector;
	matrix.SetSizes(istd::CIndex2d(3, raysCount));
	destVector.SetSizes(istd::CIndex2d(1, raysCount));

	double weightSum = 0;

	int usedRaysCount = 0;

	for (int i = 0; i < raysCount; ++i){
		Ray& ray = rays[i];
		Q_ASSERT(!ray.points.isEmpty());
		Q_ASSERT(ray.usedIndex < int(ray.points.size()));

		if (ray.usedIndex >= 0){
			const Point& rayPoint = ray.points[ray.usedIndex];

			i2d::CVector2d position = rayPoint.position - center;
			double weight = rayPoint.weight;

			matrix.SetAt(istd::CIndex2d(0, i), position.GetX() * 2.0 * weight);
			matrix.SetAt(istd::CIndex2d(1, i), position.GetY() * 2.0 * weight);
			matrix.SetAt(istd::CIndex2d(2, i), -1 * weight);

			destVector.SetAt(istd::CIndex2d(0, i), position.GetLength2() * weight);

			weightSum += weight;

			++usedRaysCount;
		}
		else{
			matrix.SetAt(istd::CIndex2d(0, i), 0);
			matrix.SetAt(istd::CIndex2d(1, i), 0);
			matrix.SetAt(istd::CIndex2d(2, i), 0);
			destVector.SetAt(istd::CIndex2d(0, i), 0);
		}
	}

	i2d::CVector2d position(0, 0);
	double radius = 0;

	bool doAgain;
	do{
		doAgain = false;

		if (usedRaysCount < 3){
			return false;	// at least 3 points needed to define circle
		}

		imath::CVarMatrix resultMatrix;
		if (!matrix.GetSolvedLSP(destVector, resultMatrix)){
			return false;
		}

		double relX = resultMatrix.GetAt(istd::CIndex2d(0, 0));
		double relY = resultMatrix.GetAt(istd::CIndex2d(0, 1));

		position = i2d::CVector2d(relX + center[0], relY + center[1]);

		double relPosNorm = relX * relX + relY * relY;
		radius = sqrt(relPosNorm - resultMatrix.GetAt(istd::CIndex2d(0, 2)));

		if (removeOutliers){
			double worseRadiusDiff = minOutliersDistance;
			int worseIndex = -1;

			for (int i = 0; i < raysCount; ++i){
				Ray& ray = rays[i];

				if (ray.usedIndex >= 0){
					Point& rayPoint = ray.points[ray.usedIndex];

					double currentRadius = rayPoint.position.GetDistance(position);

					double radiusDiff = qAbs(currentRadius - radius);
					if (radiusDiff > worseRadiusDiff){
						worseRadiusDiff = radiusDiff;

						worseIndex = i;
					}
				}
			}

			if (worseIndex >= 0){
				Ray& ray = rays[worseIndex];
				Q_ASSERT(ray.usedIndex >= 0);

				Point& rayPoint = ray.points[ray.usedIndex];

				// remove outlier entry from matrix
				matrix.SetAt(istd::CIndex2d(0, worseIndex), 0);
				matrix.SetAt(istd::CIndex2d(1, worseIndex), 0);
				matrix.SetAt(istd::CIndex2d(2, worseIndex), 0);
				destVector.SetAt(istd::CIndex2d(0, worseIndex), 0);

				weightSum -= rayPoint.weight;
				--usedRaysCount;

				ray.usedIndex = -1;

				doAgain = true;
			}
		}
	} while (doAgain);

	result.SetPosition(position);
	result.SetRadius(radius);
	result.SetWeight(weightSum / usedRaysCount);
	result.SetCalibration(m_resultCalibrationCompPtr.GetPtr());

	return true;
}


bool CCircleFindProcessorComp::CalculateAnnulus(const i2d::CVector2d& center, Rays& inRays, Rays& outRays, AnnulusFeature& result)
{
	int inRaysCount = int(inRays.size());
	if (inRaysCount < 1){
		return false;	// at least 1 point needed to define cocentric circle
	}

	int outRaysCount = int(outRays.size());
	if (outRaysCount < 1){
		return false;	// at least 1 point needed to define cocentric circle
	}

	if (inRaysCount + outRaysCount < 4){
		return false;	// at least 4 point needed to define 2 cocentric circles
	}

	imath::CVarMatrix matrix;
	imath::CVarMatrix destVector;
	matrix.SetSizes(istd::CIndex2d(4, inRaysCount + outRaysCount));
	destVector.SetSizes(istd::CIndex2d(1, inRaysCount + outRaysCount));

	double weightSum = 0;

	for (int i = 0; i < inRaysCount; ++i){
		Ray& ray = inRays[i];
		Q_ASSERT(!ray.points.isEmpty());
		Q_ASSERT(ray.usedIndex >= 0);
		Q_ASSERT(ray.usedIndex < int(ray.points.size()));

		i2d::CVector2d normPos = ray.points[ray.usedIndex].position - center;

		matrix.SetAt(istd::CIndex2d(0, i), normPos.GetX() * 2.0);
		matrix.SetAt(istd::CIndex2d(1, i), normPos.GetY() * 2.0);
		matrix.SetAt(istd::CIndex2d(2, i), -1);
		matrix.SetAt(istd::CIndex2d(3, i), 0);

		destVector.SetAt(istd::CIndex2d(0, i), normPos.GetLength2());

		weightSum += ray.points[ray.usedIndex].weight;
	}

	for (int i = 0; i < outRaysCount; ++i){
		Ray& ray = outRays[i];
		Q_ASSERT(!ray.points.isEmpty());
		Q_ASSERT(ray.usedIndex >= 0);
		Q_ASSERT(ray.usedIndex < int(ray.points.size()));

		i2d::CVector2d normPos = ray.points[ray.usedIndex].position - center;

		matrix.SetAt(istd::CIndex2d(0, i + inRaysCount), normPos.GetX() * 2.0);
		matrix.SetAt(istd::CIndex2d(1, i + inRaysCount), normPos.GetY() * 2.0);
		matrix.SetAt(istd::CIndex2d(2, i + inRaysCount), -1);
		matrix.SetAt(istd::CIndex2d(3, i + inRaysCount), 0);

		destVector.SetAt(istd::CIndex2d(0, i + inRaysCount), normPos.GetLength2());

		weightSum += ray.points[ray.usedIndex].weight;
	}

	imath::CVarMatrix resultMatrix;
	if (!matrix.GetSolvedLSP(destVector, resultMatrix)){
		return false;
	}

	double relX = resultMatrix.GetAt(istd::CIndex2d(0, 0));
	double relY = resultMatrix.GetAt(istd::CIndex2d(0, 1));
	i2d::CVector2d position(relX + center[0], relY + center[1]);
	result.SetPosition(position);

	double relPosNorm = relX * relX + relY * relY;
	double radius1 = sqrt(relPosNorm - resultMatrix.GetAt(istd::CIndex2d(0, 2)));
	double radius2 = sqrt(relPosNorm - resultMatrix.GetAt(istd::CIndex2d(0, 3)));

	result.SetInnerRadius(qMin(radius1, radius2));
	result.SetOuterRadius(qMax(radius1, radius2));
	result.SetWeight(weightSum / (inRaysCount + outRaysCount));
	result.SetCalibration(m_resultCalibrationCompPtr.GetPtr());

	return true;
}


void CCircleFindProcessorComp::AddProjectionResultsToRays(
			const i2d::CLine2d& projectionLine,
			const iprm::IParamsSet& params,
			const imeas::INumericValueProvider& container,
			Rays& inRays,
			Rays& outRays)
{
	Q_ASSERT(m_featuresMapperCompPtr.IsValid());	// validity of features mapper should be checked on the beginning

	Ray inRay;
	Ray outRay;

	double bestInWeight = -1;
	double bestOutWeight = -1;

	int featuresCount = container.GetValuesCount();
	for (int featureIndex = 0; featureIndex < featuresCount; featureIndex++){
		const CCaliperFeature* featurePtr = dynamic_cast<const CCaliperFeature*>(&container.GetNumericValue(featureIndex));
		if (featurePtr != NULL){
			Point point;
			point.weight = featurePtr->GetWeight();
			m_featuresMapperCompPtr->GetImagePosition(*featurePtr, &params, point.position);

			int edgeType = featurePtr->GetEdgeMode();
			if (edgeType == CCaliperFeature::EM_FALLING){
				if (point.weight > bestInWeight){
					bestInWeight = point.weight;
					inRay.usedIndex = int(inRay.points.size());
				}
				inRay.points.push_back(point);
			}
			else{
				if (point.weight > bestOutWeight){
					bestOutWeight = point.weight;
					outRay.usedIndex = int(outRay.points.size());
				}
				outRay.points.push_back(point);
			}
		}
	}

	if (inRay.usedIndex >= 0){
		inRay.projectionLine.CopyFrom(projectionLine);
		inRays.push_back(inRay);
	}

	if (outRay.usedIndex >= 0){
		outRay.projectionLine.CopyFrom(projectionLine);
		outRays.push_back(outRay);
	}
}


void CCircleFindProcessorComp::AddIntermediateResults(Rays& outRays)
{
	if (!m_tempConsumerCompPtr.IsValid()){
		return;
	}

	for (int rayIndex = 0; rayIndex < outRays.count(); rayIndex++){
		const Ray& ray = outRays.at(rayIndex);

		if (*m_sendUsedPointsToTempAttrPtr){
			int pointsCount = ray.points.count();
			for (int pointIndex = 0; pointIndex < pointsCount; ++pointIndex){
				const Point& rayPoint = ray.points[pointIndex];

				const i2d::CVector2d& position = rayPoint.position;

				ilog::TExtMessageModel<i2d::CPosition2d>* pointMessagePtr = new ilog::TExtMessageModel<i2d::CPosition2d>(
							(rayIndex >= 0)?
										istd::IInformationProvider::IC_INFO:
										istd::IInformationProvider::IC_WARNING,
							MI_INTERMEDIATE,
							(rayIndex >= 0)?
										QString("Point %1 at (%2, %3)").arg(rayIndex).arg(position.GetX()).arg(position.GetY()):
										QString("Unused point %1 at (%2, %3)").arg(rayIndex).arg(position.GetX()).arg(position.GetY()),
							"CircleFinder");
				pointMessagePtr->SetPosition(position);
				pointMessagePtr->SetCalibration(m_resultCalibrationCompPtr.GetPtr());

				m_tempConsumerCompPtr->AddMessage(ilog::IMessageConsumer::MessagePtr(pointMessagePtr));
			}
		}

		if (*m_sendLinesToTempAttrPtr){
			ilog::TExtMessageModel<i2d::CLine2d>* pointMessagePtr = new ilog::TExtMessageModel<i2d::CLine2d>(
						istd::IInformationProvider::IC_INFO,
						MI_INTERMEDIATE,
						QString("Line %1").arg(rayIndex),
						"CircleFinder");
			pointMessagePtr->SetPoint1(ray.projectionLine.GetPoint1());
			pointMessagePtr->SetPoint2(ray.projectionLine.GetPoint2());
			pointMessagePtr->SetCalibration(m_resultCalibrationCompPtr.GetPtr());

			m_tempConsumerCompPtr->AddMessage(ilog::IMessageConsumer::MessagePtr(pointMessagePtr));
		}
	}
}



} // namespace iipr


