#include "imeas/CHistogramStatisticsProcessorComp.h"


 // ACF includes
#include "istd/TChangeNotifier.h"


// ACF-Solutions includes
#include "imeas/IDataSequence.h"
#include "imeas/IDataSequenceStatistics.h"
#include "imeas/CDataStatistics.h"


namespace imeas
{


// reimplemented (iproc::IProcessor)

int CHistogramStatisticsProcessorComp::DoProcessing(
				const iprm::IParamsSet* /*paramsPtr*/,
				const istd::IPolymorphic* inputPtr,
				istd::IChangeable* outputPtr,
				iproc::IProgressManager* /*progressManagerPtr*/)
{
	const imeas::IDataSequence* dataSequencePtr = dynamic_cast<const imeas::IDataSequence*>(inputPtr);
	if (dataSequencePtr == NULL){
		SendErrorMessage(0, "Data sequence for statistics calculation not set");

		return TS_INVALID;
	}

	imeas::IDataSequenceStatistics* histogramStatisticsPtr = dynamic_cast<imeas::IDataSequenceStatistics*>(outputPtr);
	if (histogramStatisticsPtr == NULL){
		SendErrorMessage(0, "Statistics data object not set");

		return TS_INVALID;
	}

	return CalculateHistogramStatistics(*dataSequencePtr, *histogramStatisticsPtr) ? TS_OK : TS_INVALID;
}


// private methods

bool CHistogramStatisticsProcessorComp::CalculateHistogramStatistics(
			const imeas::IDataSequence& input,
			imeas::IDataSequenceStatistics& histogramStatistics) const
{
	istd::CChangeNotifier changePtr(&histogramStatistics);

	if (input.IsEmpty()){
		histogramStatistics.ResetStatistics();
	
		return true;
	}

	if (input.GetChannelsCount() != histogramStatistics.GetChannelsCount()){
		histogramStatistics.ResetStatistics();
	}

	bool retVal = true;
	int channelsCount = input.GetChannelsCount();
	for (int channelIndex = 0; channelIndex < channelsCount; channelIndex++){
		imeas::CDataStatistics channelStatistics;

		retVal = retVal && CalculateChannelStatistics(input, channelIndex, channelStatistics);

		retVal = retVal && histogramStatistics.SetChannelStatistics(channelStatistics, channelIndex);
	}

	return retVal;
}


bool CHistogramStatisticsProcessorComp::CalculateChannelStatistics(
			const imeas::IDataSequence& input,
			int channelIndex,
			imeas::IDataStatistics& dataStatistics) const
{
	int channleSamplesCount = input.GetSamplesCount() / input.GetChannelsCount();

	double average = 0.0;
	double median = -1.0;
	double standardDeviation = 0.0;
	istd::CRange boundaries;

	for (int sampleIndex = 0; sampleIndex < channleSamplesCount; sampleIndex++){
		double sample = input.GetSample(sampleIndex, channelIndex);

		if (sample != 0){
			boundaries.SetMinValue(sampleIndex);
			break;
		}
	}

	for (int sampleIndex = channleSamplesCount - 1; sampleIndex >= 0; sampleIndex--){
		double sample = input.GetSample(sampleIndex, channelIndex);

		if (sample != 0){
			boundaries.SetMaxValue(sampleIndex);
			break;
		}
	}

	double weights = 0.0;

	for (int sampleIndex = 0; sampleIndex < channleSamplesCount; sampleIndex++){
		double sample = input.GetSample(sampleIndex, channelIndex);

		average += sampleIndex * sample;

		weights += sample;

		if (weights >= 0.5 && median < 0){	
			median = sampleIndex;
		}
	}

	for (int sampleIndex = 0; sampleIndex < channleSamplesCount; sampleIndex++){
		double sample = input.GetSample(sampleIndex, channelIndex);

		standardDeviation += sample * (sampleIndex - average) * (sampleIndex - average);
	}

	standardDeviation = sqrt(standardDeviation);

	dataStatistics.CreateStatistics(average, median, standardDeviation, boundaries);

	return true;
}


} // namespace imeas


