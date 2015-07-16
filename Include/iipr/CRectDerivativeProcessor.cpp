#include "iipr/CRectDerivativeProcessor.h"


// Qt includes
#include <QtCore/QObject>

// ACF includes
#include "istd/CChangeNotifier.h"
#include "istd/TSmartPtr.h"
#include "imath/CDoubleManip.h"
#include "iprm/TParamsPtr.h"

// ACF-Solutions includes
#include "imeas/IDataSequence.h"
#include "imeas/INumericValue.h"
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

	double halfRealLength = qMax(1.0, filterLength * 0.5);

	int sumOffset = int(halfRealLength);
	double sumLastAlpha = halfRealLength - sumOffset;
	double sumLastAlphaInv = 1 - sumLastAlpha;

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

	// buffer to speed up data access (brings ca. 10%)
	std::vector<double> channelData(samplesCount);

	if (projectionWidth > 0){
		for (int channelIndex = 0; channelIndex < channelsCount; ++channelIndex){
			for (int i = 0; i < samplesCount; ++i){
				channelData[i] = source.GetSample(i, channelIndex);
			}

			double leftSum = 0.0;
			double leftWeight = 0.0;
			double rightSum = channelData[0] * sumLastAlpha;
			double rightWeight = sumLastAlpha;
			for (int x = -sumOffset; x < projectionWidth; ++x){
				if (x < projectionWidth - sumOffset){
					rightSum +=	channelData[x + sumOffset + 1] * sumLastAlpha + channelData[x + sumOffset] * sumLastAlphaInv;

					rightWeight += 1;
				}
				else if (x == projectionWidth - sumOffset){
					rightSum += channelData[x + sumOffset] * sumLastAlphaInv;
					rightWeight += sumLastAlphaInv;
				}

				if (x >= 0){
					double diff = channelData[x];
					leftSum += diff;

					if (x > sumOffset){
						leftSum -= channelData[x - sumOffset] * sumLastAlphaInv;
						leftSum -= channelData[x - sumOffset - 1] * sumLastAlpha;
					}
					else if (x == sumOffset){
						leftSum -= channelData[x - sumOffset] * sumLastAlphaInv;
						leftWeight += sumLastAlpha;
					}
					else{
						leftWeight += 1;
					}

					rightSum -= diff;
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
			ibase::IProgressManager* /*progressManagerPtr*/)
{
	const imeas::IDataSequence* inputProjectionPtr = dynamic_cast<const imeas::IDataSequence*>(inputPtr);
	imeas::IDataSequence* outputProjectionPtr = dynamic_cast<imeas::IDataSequence*>(outputPtr);

	double filterLength = 5.0;
	if (paramsPtr != NULL){
		iprm::TParamsPtr<imeas::INumericValue> filterParamsPtr(paramsPtr, m_filterParamsId);
		if (filterParamsPtr.IsValid()){
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


QString CRectDerivativeProcessor::GetNumericValueName(int /*dimension*/) const
{
	return QObject::tr("Filter");
}


QString CRectDerivativeProcessor::GetNumericValueDescription(int /*index*/) const
{
	return QObject::tr("Smoothing filter length, higher value makes insensible to data noise");
}


const imath::IUnitInfo& CRectDerivativeProcessor::GetNumericValueUnitInfo(int /*dimension*/) const
{
	return *this;
}


// reimplemented (imath::IUnitInfo)

int CRectDerivativeProcessor::GetUnitType() const
{
	return UT_TECHNICAL;
}


QString CRectDerivativeProcessor::GetUnitName() const
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


