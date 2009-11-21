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

bool CGeneralSamplesSequence::CreateSequence(int samplesCount, int channelsCount)
{
	m_channelsCount = channelsCount;
	m_samples.resize(samplesCount * m_channelsCount, 0.0);

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


int CGeneralSamplesSequence::GetSamplesCount() const
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


bool CGeneralSamplesSequence::CopySequenceFrom(const ISamplesSequence& sequence)
{
	int samplesCount = sequence.GetSamplesCount();
	int channelsCount = sequence.GetChannelsCount();
	if (!CreateSequence(samplesCount, channelsCount)){
		return false;
	}

	for (int sampleIndex = 0; sampleIndex < samplesCount; ++sampleIndex){
		for (int channelIndex = 0; channelIndex < channelsCount; ++channelIndex){
			double sample = sequence.GetSample(sampleIndex, channelIndex);

			SetSample(sampleIndex, channelIndex, sample);
		}
	}

	return true;
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

	int samplesCount = GetSamplesCount();

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


} // namespace imeas


