#include "iipr/CCircleFindProcessorComp.h"


// ACF includes
#include "imath/CVarMatrix.h"
#include "i2d/CAnnulusSegment.h"
#include "iprm/CParamsSet.h"
#include "iprm/TParamsPtr.h"

// IACF includes
#include "iipr/CCaliperFeature.h"
#include "iipr/CFeaturesContainer.h"
#include "iipr/CSingleFeatureConsumer.h"


namespace iipr
{


// reimplemented (iipr::IImageToFeatureProcessor)

int CCircleFindProcessorComp::DoExtractFeatures(
			const iprm::IParamsSet* paramsPtr,
			const iimg::IBitmap& image,
			IFeaturesConsumer& results)
{
	if (*m_introspectionOnAttrPtr){
		clear();
	}

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
	iprm::CParamsSet extendedParamsSet;
	extendedParamsSet.SetEditableParameter(*m_slaveLineIdAttrPtr, &projectionLine);
	extendedParamsSet.SetSlaveSet(paramsPtr);

	i2d::CVector2d center;

	if (!AddAoiToRays(*aoiObjectPtr, extendedParamsSet, image, *circleFinderParamsPtr, inRays, outRays, projectionLine, center)){
		return TS_INVALID;
	}

	if (*m_searchForAnnulusAttrPtr){
		istd::TDelPtr<AnnulusFeature> featurePtr(new AnnulusFeature());
		if (CalculateAnnulus(center, inRays, outRays, *featurePtr)){
			results.AddFeature(featurePtr.PopPtr());

			AddIntermediateResults(outRays);

			return TS_OK;
		}

		AddIntermediateResults(outRays);
	}
	else{
		istd::TDelPtr<CircleFeature> featurePtr(new CircleFeature());
		Rays& usedRays = (inRays.size() >= outRays.size())? inRays: outRays;
		if (		CalculateCircle(
								center,
								circleFinderParamsPtr->IsOutlierEliminationEnabled(),
								circleFinderParamsPtr->GetMinOutlierDistance(),
								usedRays,
								*featurePtr)){
			results.AddFeature(featurePtr.PopPtr());

			AddIntermediateResults(usedRays);

			return TS_OK;
		}

		AddIntermediateResults(usedRays);
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

	istd::TDelPtr<i2d::IObject2d> transformedRegionPtr;
	const i2d::IObject2d* calibratedAoiPtr = dynamic_cast<const i2d::IObject2d*>(&aoiObject);

	if (m_regionCalibrationProviderCompPtr.IsValid()){
		const i2d::ICalibration2d* logToPhysicalTransformPtr = m_regionCalibrationProviderCompPtr->GetCalibration();
		if (logToPhysicalTransformPtr != NULL){
			transformedRegionPtr.SetCastedOrRemove<istd::IChangeable>(aoiObject.CloneMe());

			if (transformedRegionPtr.IsValid()){
				if (!transformedRegionPtr->Transform(*logToPhysicalTransformPtr)){
					return false;
				}

				calibratedAoiPtr = transformedRegionPtr.GetPtr();
			}
		}
	}

	const i2d::CAnnulus* annulusAoiPtr = dynamic_cast<const i2d::CAnnulus*>(calibratedAoiPtr);
	if (annulusAoiPtr != NULL){
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

			projectionLine.SetPoint1(center + directionVector * annulusAoiPtr->GetInnerRadius());
			projectionLine.SetPoint2(center + directionVector * annulusAoiPtr->GetOuterRadius());

			caliperFeaturesConsumerPtr->ResetFeatures();
			m_slaveProcessorCompPtr->DoProcessing(&params, &image, caliperFeaturesConsumerPtr);

			AddProjectionResultsToRays(params, *caliperFeaturesProviderPtr, inRays, outRays);
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
	if (raysCount < 3){
		return false;	// at least 3 points needed to define circle
	}

	imath::CVarMatrix matrix;
	imath::CVarMatrix destVector;
	matrix.SetSizes(istd::CIndex2d(3, raysCount));
	destVector.SetSizes(istd::CIndex2d(1, raysCount));

	double weightSum = 0;

	for (int i = 0; i < raysCount; ++i){
		Ray& ray = rays[i];
		Q_ASSERT(!ray.points.isEmpty());
		Q_ASSERT(ray.usedIndex >= 0);
		Q_ASSERT(ray.usedIndex < int(ray.points.size()));

		i2d::CVector2d point = ray.points[ray.usedIndex].position - center;

		matrix.SetAt(istd::CIndex2d(0, i), point.GetX() * 2.0);
		matrix.SetAt(istd::CIndex2d(1, i), point.GetY() * 2.0);
		matrix.SetAt(istd::CIndex2d(2, i), -1);

		destVector.SetAt(istd::CIndex2d(0, i), point.GetLength2());

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
	result.SetRadius(sqrt(relPosNorm - resultMatrix.GetAt(istd::CIndex2d(0, 2))));

	result.SetWeight(weightSum / raysCount);

	if (removeOutliers){
		Rays optimizedRays;

		for (int i = 0; i < raysCount; ++i){
			Ray& ray = rays[i];
			i2d::CVector2d point = ray.points[ray.usedIndex].position - result.GetPosition();

			double currentRadius = sqrt(point.GetX() * point.GetX() + point.GetY()* point.GetY());

			double foundRadius = result.GetRadius();

			if (qAbs(currentRadius - foundRadius) < minOutliersDistance){
				optimizedRays.push_back(ray);
			}
		}

		if (optimizedRays.size() != rays.size() && optimizedRays.size() >= 3){
			return CalculateCircle(center, removeOutliers, minOutliersDistance, optimizedRays, result);
		}
	}

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

	return true;
}


void CCircleFindProcessorComp::AddProjectionResultsToRays(
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
		inRays.push_back(inRay);
	}

	if (outRay.usedIndex >= 0){
		outRays.push_back(outRay);
	}
}


void CCircleFindProcessorComp::AddIntermediateResults(Rays& outRays)
{
	if (!*m_introspectionOnAttrPtr){
		return;
	}

	for (int rayIndex = 0; rayIndex < outRays.count(); rayIndex++){
		const Ray& ray = outRays.at(rayIndex);
		if (ray.points.count() > 0){
			const i2d::CVector2d& pointVector = ray.points.at(ray.usedIndex).position;

			append(QPointF(pointVector.GetX(), pointVector.GetY()));
		}
	}
}



} // namespace iipr


