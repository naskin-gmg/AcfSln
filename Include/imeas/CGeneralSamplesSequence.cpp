#include "imeas/CGeneralSamplesSequence.h"


#include "istd/TChangeNotifier.h"


#include "iser/IArchive.h"
#include "iser/CArchiveTag.h"


namespace imeas
{


CGeneralSamplesSequence::CGeneralSamplesSequence()
:	m_channelsCount(0),
	m_samplingPeriod(-1)
{
}


// reimplemented (imeas::ISamplesSequence)

bool CGeneralSamplesSequence::CreateSequence(int timeSamplesCount, int channelsCount)
{
	m_channelsCount = channelsCount;
	m_samples.resize(timeSamplesCount * m_channelsCount, 0.0);

	return true;
}


bool CGeneralSamplesSequence::IsEmpty() const
{
	return m_samples.empty();
}


void CGeneralSamplesSequence::ResetSequence()
{
	m_samples.clear();
}


int CGeneralSamplesSequence::GetTimeSamplesCount() const
{
	if (m_channelsCount > 0){
		return int(m_samples.size() / m_channelsCount);
	}
	else{
		return 0;
	}
}


int CGeneralSamplesSequence::GetChannelsCount() const
{
	return m_channelsCount;
}


double CGeneralSamplesSequence::GetSamplingPeriod() const
{
	return m_samplingPeriod;
}


void CGeneralSamplesSequence::SetSamplingPeriod(double value)
{
	m_samplingPeriod = value;
}


double CGeneralSamplesSequence::GetSample(int index, int channel) const
{
	I_ASSERT(index >= 0);
	I_ASSERT(index * m_channelsCount + channel < int(m_samples.size()));

	return m_samples[index * m_channelsCount + channel];
}


void CGeneralSamplesSequence::SetSample(int index, int channel, double value)
{
	I_ASSERT(index >= 0);
	I_ASSERT(index * m_channelsCount + channel < int(m_samples.size()));

	m_samples[index * m_channelsCount + channel] = value;
}


// reimplemented (imath::ISampledFunction2d)

bool CGeneralSamplesSequence::CreateFunction(double* dataPtr, const ArgumentType& sizes)
{
	if (!sizes.IsSizeEmpty()){
		int elementsCount = sizes.GetProductVolume();

		m_samples.resize(elementsCount);

		memcpy(&m_samples[0], dataPtr, elementsCount * sizeof(double));

		return true;
	}
	else{
		return false;
	}
}


int CGeneralSamplesSequence::GetSamplesCount() const
{
	return GetTimeSamplesCount() * GetChannelsCount();
}


int CGeneralSamplesSequence::GetGridSize(int dimensionIndex) const
{
	if (dimensionIndex == 0){
		return GetTimeSamplesCount();
	}
	else if (dimensionIndex == 1){
		return GetChannelsCount();
	}
	else{
		return 0;
	}
}


istd::CRange CGeneralSamplesSequence::GetLogicalRange(int dimensionIndex) const
{
	if (dimensionIndex == 0){
		return istd::CRange(0, GetSamplingPeriod() * GetTimeSamplesCount());
	}
	else if (dimensionIndex == 1){
		return istd::CRange(0, 1);
	}
	else{
		return istd::CRange::GetInvalid();
	}
}


istd::CRange CGeneralSamplesSequence::GetResultValueRange(int /*dimensionIndex*/, int /*resultDimension*/) const
{
	return istd::CRange(-1, 1);
}


// reimplemented (imath::TIMathFunction)

bool CGeneralSamplesSequence::GetValueAt(const ArgumentType& argument, ResultType& result) const
{
	int sampleIndex = argument[0];
	int channelIndex = argument[1];
	if ((sampleIndex >= 0) && (sampleIndex < GetTimeSamplesCount()) && (channelIndex >= 0) && (channelIndex < 2)){
		result[0] = GetSample(sampleIndex, channelIndex);

		return true;
	}
	else{
		return false;
	}
}


CGeneralSamplesSequence::ResultType CGeneralSamplesSequence::GetValueAt(const ArgumentType& argument) const
{
	CGeneralSamplesSequence::ResultType retVal;

	int sampleIndex = argument[0];
	int channelIndex = argument[1];
	if ((sampleIndex >= 0) && (sampleIndex < GetTimeSamplesCount()) && (channelIndex >= 0) && (channelIndex < 2)){
		retVal[0] = GetSample(sampleIndex, channelIndex);
	}
	else{
		retVal[0] = 0;
	}

	return retVal;
}


// reimplemented (iser::ISerializable)

bool CGeneralSamplesSequence::Serialize(iser::IArchive& archive)
{
	bool retVal = true;

	int channelsCount = GetChannelsCount();
	static iser::CArchiveTag channelsCountTag("ChannelsCount", "Number of channels");
	retVal = retVal && archive.BeginTag(channelsCountTag);
	retVal = retVal && archive.Process(channelsCount);
	retVal = retVal && archive.EndTag(channelsCountTag);

	int samplesCount = GetTimeSamplesCount();

	static iser::CArchiveTag samplesListTag("SampleList", "List of sample values");
	static iser::CArchiveTag channelsTag("Channels", "List of sample values");
	retVal = retVal && archive.BeginMultiTag(samplesListTag, channelsTag, samplesCount);

	if (!retVal){
		return false;
	}

	bool isStoring = archive.IsStoring();

	istd::CChangeNotifier notifier(NULL);

	if (!isStoring){
		notifier.SetPtr(this);
		if (!CreateSequence(samplesCount, channelsCount)){
			return false;
		}
	}

	for (int i = 0; i < samplesCount; ++i){
		retVal = retVal && archive.BeginTag(channelsTag);
		for (int channelIndex = 0; channelIndex < channelsCount; ++channelIndex){
			retVal = retVal && archive.Process(m_samples[i]);
		}
		retVal = retVal && archive.EndTag(channelsTag);
	}

	retVal = retVal && archive.EndTag(samplesListTag);

	return retVal;
}


// reimplemented (istd::IChangeable)

bool CGeneralSamplesSequence::CopyFrom(const istd::IChangeable& object)
{
	const ISamplesSequence* sequencePtr = dynamic_cast<const ISamplesSequence*>(&object);
	if (sequencePtr != NULL){
		const CGeneralSamplesSequence* nativeSequencePtr = dynamic_cast<const CGeneralSamplesSequence*>(sequencePtr);
		if (nativeSequencePtr != NULL){
			m_samples = nativeSequencePtr->m_samples;
			m_channelsCount = nativeSequencePtr->m_channelsCount;
			m_samplingPeriod = nativeSequencePtr->m_samplingPeriod;

			return true;
		}
		else{
			int samplesCount = sequencePtr->GetTimeSamplesCount();
			int channelsCount = sequencePtr->GetChannelsCount();

			if (CreateSequence(samplesCount, channelsCount)){
				for (int sampleIndex = 0; sampleIndex < samplesCount; ++sampleIndex){
					for (int channelIndex = 0; channelIndex < channelsCount; ++channelIndex){
						double sample = sequencePtr->GetSample(sampleIndex, channelIndex);

						SetSample(sampleIndex, channelIndex, sample);
					}
				}

				SetSamplingPeriod(sequencePtr->GetSamplingPeriod());

				return true;
			}
		}
	}

	return false;
}


} // namespace imeas


