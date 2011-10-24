#include "iipr/CExtremumCaliperProcessorComp.h"


// ACF-Solutions includes
#include "imeas/IDataSequence.h"
#include "imeas/CSamplesInfo.h"

#include "iipr/ICaliperParams.h"
#include "iipr/IFeaturesConsumer.h"
#include "iipr/CCaliperFeature.h"


namespace iipr
{


// reimplemented (iipr::ICaliperProcessor)

bool CExtremumCaliperProcessorComp::DoCaliper(
			const imeas::IDataSequence& derivative,
			const ICaliperParams* paramsPtr,
			IFeaturesConsumer& results)
{
	if (paramsPtr == NULL){
		return false;
	}

	int samplesCount = derivative.GetSamplesCount();
	if ((samplesCount <= 0) || (derivative.GetChannelsCount() <= 0)){
		return false;
	}

	double weightThreshold = paramsPtr->GetWeightThreshold();
	I_ASSERT(weightThreshold >= 0);

	istd::CRange proportionRange(0, 1);

	const imeas::CSamplesInfo* derivativeInfoPtr = dynamic_cast<const imeas::CSamplesInfo*>(derivative.GetSequenceInfo());
	if (derivativeInfoPtr != NULL){
		const istd::CRange& logicalRange = derivativeInfoPtr->GetLogicalSamplesRange();
		if (logicalRange.IsValid()){
			proportionRange = logicalRange;
		}
	}

	int polarityMode = paramsPtr->GetPolarityMode();
	bool isRisedEdgeSupported = (polarityMode != ICaliperParams::PM_DROPPED);
	bool isDropppedEdgeSupported = (polarityMode != ICaliperParams::PM_RISED);

	results.ResetFeatures();

	bool isBackward = (paramsPtr->GetDirectionMode() == ICaliperParams::DM_BACKWARD);
	int index = -1;
	int nextIndex = isBackward? samplesCount - 1: 0;
	int indexDiff = isBackward? -1: 1;
	int endIndex = isBackward? -1: samplesCount;

	int risedCount = -1;
	int droppedCount = -1;

	double prevValue = 0;
	double value = 0;

	for (; nextIndex != endIndex; nextIndex += indexDiff){
		double nextValue = derivative.GetSample(nextIndex);
		if (nextValue < value){
			if (isRisedEdgeSupported && (risedCount > 0)){
				if (value > weightThreshold){
					double prevDelta = value - prevValue;
					I_ASSERT(prevDelta >= 0);

					double nextDelta = value - nextValue;
					I_ASSERT(nextDelta >= 0);
					I_ASSERT(nextDelta + prevDelta > 0);

					double deltaSum = (nextDelta + prevDelta);
					I_ASSERT(deltaSum >= 0);

					double shift = prevDelta / deltaSum;

					double position = proportionRange.GetValueFromAlpha((index + shift) / samplesCount);
					I_ASSERT(position >= 0 - I_EPSILON);
					I_ASSERT(position <= 1 + I_EPSILON);

					bool isReady = false;
					if (		!results.AddFeature(new CCaliperFeature(value, position, CCaliperFeature::EM_RISING), &isReady) ||
								isReady){
						return isReady;
					}
				}
			}

			risedCount = 0;
			droppedCount++;
		}
		else{
			if (isDropppedEdgeSupported && (droppedCount > 0)){
				if (value < -weightThreshold){
					double prevDelta = prevValue - value;
					I_ASSERT(prevDelta >= 0);

					double nextDelta = nextValue - value;
					I_ASSERT(nextDelta >= 0);
					I_ASSERT(nextDelta + prevDelta > 0);

					double deltaSum = (nextDelta + prevDelta);
					I_ASSERT(deltaSum >= 0);

					double shift = prevDelta / deltaSum;

					double position = proportionRange.GetValueFromAlpha((index + shift) / samplesCount);
					I_ASSERT(position >= 0 - I_EPSILON);
					I_ASSERT(position <= 1 + I_EPSILON);

					bool isReady = false;
					if (		!results.AddFeature(new CCaliperFeature(-value, position, CCaliperFeature::EM_FALLING), &isReady) ||
								isReady){
						return isReady;
					}
				}
			}

			risedCount++;
			droppedCount = 0;
		}

		prevValue = value;
		value = nextValue;

		index = nextIndex;
	}

	return true;
}


// reimplemented (iproc::IProcessor)

int CExtremumCaliperProcessorComp::DoProcessing(
			const iprm::IParamsSet* paramsPtr,
			const istd::IPolymorphic* inputPtr,
			istd::IChangeable* outputPtr,
			iproc::IProgressManager* /*progressManagerPtr*/)
{
	if (outputPtr == NULL){
		return TS_OK;
	}

	const imeas::IDataSequence* derivativePtr = dynamic_cast<const imeas::IDataSequence*>(inputPtr);
	IFeaturesConsumer* consumerPtr = dynamic_cast<IFeaturesConsumer*>(outputPtr);

	if (		(derivativePtr == NULL) ||
				(consumerPtr == NULL) ||
				(paramsPtr == NULL) ||
				(*m_caliperParamsIdAttrPtr).IsEmpty()){
		return TS_INVALID;
	}

	const ICaliperParams* caliperParamsPtr = dynamic_cast<const ICaliperParams*>(paramsPtr->GetParameter((*m_caliperParamsIdAttrPtr).ToString()));

	return DoCaliper(*derivativePtr, caliperParamsPtr, *consumerPtr)? TS_OK: TS_INVALID;
}


// reimplemented (iipr::IFeatureToProjectionMapper)

bool CExtremumCaliperProcessorComp::GetProjectionPosition(
			const IFeature& feature,
			const iprm::IParamsSet* /*paramsPtr*/,
			double& result) const
{
	const CCaliperFeature* caliperFeaturePtr = dynamic_cast<const CCaliperFeature*>(&feature);
	if (caliperFeaturePtr != NULL){
		result = caliperFeaturePtr->GetPosition();

		return true;
	}

	return false;
}


} // namespace iipr

