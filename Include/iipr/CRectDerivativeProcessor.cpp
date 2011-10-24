#include "iipr/CRectDerivativeProcessor.h"


// ACF includes
#include "istd/TChangeNotifier.h"
#include "istd/TSmartPtr.h"
#include "imath/CDoubleManip.h"

// ACF-Solutions includes
#include "imeas/IDataSequence.h"
#include "imeas/INumericParams.h"
#include "imeas/CSamplesInfo.h"


namespace iipr
{


bool CRectDerivativeProcessor::DoDerivativeProcessing(const imeas::IDataSequence& source, double filterLength, imeas::IDataSequence& results)
{
	int samplesCount = source.GetSamplesCount();
	int channelsCount = source.GetChannelsCount();
	if ((samplesCount < 2) || (channelsCount < 1)){
		return false;
	}

	istd::CChangeNotifier notifier(&results);

	istd::CRange sourceProportionRange(0, 1);

	const imeas::CSamplesInfo* sourceInfoPtr = dynamic_cast<const imeas::CSamplesInfo*>(source.GetSequenceInfo());
	if (sourceInfoPtr != NULL){
		const istd::CRange& logicalRange = sourceInfoPtr->GetLogicalSamplesRange();
		if (logicalRange.IsValid()){
			sourceProportionRange = logicalRange;
		}
	}

	double halfRealLength = istd::Max(1.0, filterLength * 0.5);

	int sumOffset = int(halfRealLength);
	double sumLastAlpha = halfRealLength - sumOffset;

	int projectionWidth = samplesCount - 1;

	double proportionXAlpha = 0.5 / samplesCount;
	istd::CRange resultProportionRange(
				sourceProportionRange.GetValueFromAlpha(proportionXAlpha),
				sourceProportionRange.GetValueFromAlpha(1.0 - proportionXAlpha));
	if (!results.CreateSequenceWithInfo(
				istd::TSmartPtr<const imeas::IDataSequenceInfo>(new imeas::CSamplesInfo(resultProportionRange)),
				projectionWidth,
				channelsCount)){
		return false;
	}

	if (projectionWidth > 0){
		for (int channelIndex = 0; channelIndex < channelsCount; ++channelIndex){
			double leftSum = 0.0;
			double leftWeight = 0.0;
			double rightSum = source.GetSample(0, channelIndex) * sumLastAlpha;
			double rightWeight = sumLastAlpha;
			for (int x = -sumOffset; x < projectionWidth; ++x){
				if (x <= projectionWidth - sumOffset){
					if (x < projectionWidth - sumOffset){
						rightSum +=	source.GetSample(x + sumOffset + 1, channelIndex) * sumLastAlpha +
									source.GetSample(x + sumOffset, channelIndex) * (1 - sumLastAlpha);
						rightWeight += 1;
					}
					else{
						rightSum += source.GetSample(x + sumOffset, channelIndex) * (1 - sumLastAlpha);
						rightWeight += (1 - sumLastAlpha);
					}
				}

				if (x >= 0){
					leftSum += source.GetSample(x, channelIndex);
					if (x >= sumOffset){
						leftSum -= source.GetSample(x - sumOffset, channelIndex) * (1 - sumLastAlpha);

						if (x > sumOffset){
							leftSum -= source.GetSample(x - sumOffset - 1, channelIndex) * sumLastAlpha;
						}
						else{
							leftWeight += sumLastAlpha;
						}
					}
					else{
						leftWeight += 1;
					}

					rightSum -= source.GetSample(x, channelIndex);
					rightWeight -= 1;

					results.SetSample(x, channelIndex, rightSum / rightWeight - leftSum / leftWeight);
				}
			}
		}
	}

	return true;
}


// reimplemented (iproc::IProcessor)

int CRectDerivativeProcessor::DoProcessing(
			const iprm::IParamsSet* paramsPtr,
			const istd::IPolymorphic* inputPtr,
			istd::IChangeable* outputPtr,
			iproc::IProgressManager* /*progressManagerPtr*/)
{
	const imeas::IDataSequence* inputProjectionPtr = dynamic_cast<const imeas::IDataSequence*>(inputPtr);
	imeas::IDataSequence* outputProjectionPtr = dynamic_cast<imeas::IDataSequence*>(outputPtr);

	double filterLength = 5.0;
	if (paramsPtr != NULL){
		const imeas::INumericParams* filterParamsPtr = dynamic_cast<const imeas::INumericParams*>(
					paramsPtr->GetParameter(m_filterParamsId));
		if (filterParamsPtr != NULL){
			imath::CVarVector filterLengths = filterParamsPtr->GetValues();
			if (filterLengths.GetElementsCount() > 0){
				filterLength = filterLengths[0];
			}
		}
	}

	return DoDerivativeProcessing(*inputProjectionPtr, filterLength, *outputProjectionPtr)?
				TS_OK:
				TS_INVALID;
}


// reimplemented (imeas::INumericConstraints)

int CRectDerivativeProcessor::GetNumericValuesCount() const
{
	return 1;
}


istd::CString CRectDerivativeProcessor::GetNumericValueDescription(int /*dimension*/) const
{
	return "Filter length";
}


const imeas::IUnitInfo& CRectDerivativeProcessor::GetNumericValueUnitInfo(int /*dimension*/) const
{
	return *this;
}


// reimplemented (imeas::IUnitInfo)

int CRectDerivativeProcessor::GetUnitType() const
{
	return UT_TECHNICAL;
}


istd::CString CRectDerivativeProcessor::GetUnitName() const
{
	return "px";
}


double CRectDerivativeProcessor::GetDisplayMultiplicationFactor() const
{
	return 1;
}


istd::CRange CRectDerivativeProcessor::GetValueRange() const
{
	return istd::CRange(1, 100);
}


const imath::IDoubleManip& CRectDerivativeProcessor::GetValueManip() const
{
	static imath::CDoubleManip manip;

	return manip;
}


} // namespace iipr


