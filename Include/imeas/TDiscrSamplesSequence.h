#ifndef imeas_TDiscrSamplesSequence_included
#define imeas_TDiscrSamplesSequence_included


#include "istd/TChangeNotifier.h"

#include "iser/IArchive.h"
#include "iser/CArchiveTag.h"

#include "imeas/IDiscrSamplesSequence.h"


namespace imeas
{


template <typename Element>
class TDiscrSamplesSequence: virtual public IDiscrSamplesSequence
{
public:
	TDiscrSamplesSequence();

	// reimplemented (imeas::IDiscrSamplesSequence)
	virtual bool CreateDiscrSequence(
				int samplesCount,
				void* dataPtr,
				bool releaseFlag,
				int sampleDiff,
				int channelDiff,
				int sampleDepth,
				int channelsCount = 1);
	virtual int GetSampleDepth() const;
	virtual I_DWORD GetDiscrSample(int position, int channel = 0) const;
	virtual bool SetDiscrSample(int position, int channel, I_DWORD sample);

	// reimplemented (imeas::ISamplesSequence)
	virtual bool CreateSequence(int samplesCount, int channelsCount = 1);
	virtual bool IsEmpty() const;
	virtual void ResetSequence();
	virtual int GetTimeSamplesCount() const;
	virtual int GetChannelsCount() const;
	virtual double GetSamplingPeriod() const;
	virtual void SetSamplingPeriod(double value);
	virtual double GetSample(int index, int channel = 0) const;
	virtual void SetSample(int index, int channel, double value);
	virtual bool CopySequenceFrom(const ISamplesSequence& sequence);

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);

private:
	istd::TOptDelPtr<Element, true> m_sampleBuffer;

	int m_allocatedElementsCount;
	int m_samplesCount;
	int m_channelsCount;
	int m_sampleDiff;
	int m_channelDiff;

	double m_samplingPeriod;
};


// public methods

template <typename Element>
TDiscrSamplesSequence<Element>::TDiscrSamplesSequence()
:	m_allocatedElementsCount(0),
	m_samplesCount(0),
	m_channelsCount(0),
	m_sampleDiff(0),
	m_channelDiff(0),
	m_samplingPeriod(0)
{
}


// reimplemented (imeas::IDiscrSamplesSequence)

template <typename Element>
bool TDiscrSamplesSequence<Element>::CreateDiscrSequence(
			int samplesCount,
			void* dataPtr,
			bool releaseFlag,
			int sampleDiff,
			int channelDiff,
			int sampleDepth,
			int channelsCount)
{
	if (sampleDepth != sizeof(Element) * 8){
		return false;
	}

	m_sampleBuffer.SetPtr((Element*)dataPtr, releaseFlag);

	m_allocatedElementsCount = 0;
	m_samplesCount = samplesCount;
	m_channelsCount = channelsCount;
	m_sampleDiff = (sampleDiff != 0)? sampleDiff: channelsCount * sizeof(Element);
	m_channelDiff = (channelDiff != 0)? channelDiff: sizeof(Element);

	return true;
}


template <typename Element>
int TDiscrSamplesSequence<Element>::GetSampleDepth() const
{
	return sizeof(Element) * 8;
}


template <typename Element>
I_DWORD TDiscrSamplesSequence<Element>::GetDiscrSample(int position, int channel) const
{
	const Element& element = *(const Element*)((const I_BYTE*)m_sampleBuffer.GetPtr() + position * m_sampleDiff + channel * m_channelDiff);

	return element;
}


template <typename Element>
bool TDiscrSamplesSequence<Element>::SetDiscrSample(int position, int channel, I_DWORD sample)
{
	Element& element = *(Element*)((I_BYTE*)m_sampleBuffer.GetPtr() + position * m_sampleDiff + channel * m_channelDiff);

	element = Element(sample);

	return true;
}


// reimplemented (imeas::ISamplesSequence)

template <typename Element>
bool TDiscrSamplesSequence<Element>::CreateSequence(int samplesCount, int channelsCount)
{
	int elementsCount = samplesCount * channelsCount;
	if (		(elementsCount > m_allocatedElementsCount) ||
				(elementsCount * 2 < m_allocatedElementsCount)){
		m_sampleBuffer.SetPtr(new Element[elementsCount], true);
		m_allocatedElementsCount = elementsCount * sizeof(Element);
	}

	m_samplesCount = samplesCount;
	m_channelsCount = channelsCount;
	m_sampleDiff = channelsCount * sizeof(Element);
	m_channelDiff = sizeof(Element);

	return true;
}


template <typename Element>
bool TDiscrSamplesSequence<Element>::IsEmpty() const
{
	return m_samplesCount <= 0;
}


template <typename Element>
void TDiscrSamplesSequence<Element>::ResetSequence()
{
	m_sampleBuffer.Reset();
	m_samplesCount = 0;
	m_channelsCount = 0;
}


template <typename Element>
int TDiscrSamplesSequence<Element>::GetTimeSamplesCount() const
{
	return m_samplesCount;
}


template <typename Element>
int TDiscrSamplesSequence<Element>::GetChannelsCount() const
{
	return m_channelsCount;
}


template <typename Element>
double TDiscrSamplesSequence<Element>::GetSamplingPeriod() const
{
	return m_samplingPeriod;
}


template <typename Element>
void TDiscrSamplesSequence<Element>::SetSamplingPeriod(double value)
{
	istd::CChangeNotifier notifier(this);

	m_samplingPeriod = value;
}


template <typename Element>
double TDiscrSamplesSequence<Element>::GetSample(int index, int channel) const
{
	const Element& element = *(const Element*)((const I_BYTE*)m_sampleBuffer.GetPtr() + index * m_sampleDiff + channel * m_channelDiff);

	return double(element) / ::pow(2.0, double(sizeof(element) * 8));
}


template <typename Element>
void TDiscrSamplesSequence<Element>::SetSample(int index, int channel, double value)
{
	Element& element = *(Element*)((I_BYTE*)m_sampleBuffer.GetPtr() + index * m_sampleDiff + channel * m_channelDiff);

	element = Element(value * ::pow(2.0, double(sizeof(element) * 8)) - I_BIG_EPSILON);
}


template <typename Element>
bool TDiscrSamplesSequence<Element>::CopySequenceFrom(const ISamplesSequence& sequence)
{
	int samplesCount = sequence.GetTimeSamplesCount();
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

template <typename Element>
bool TDiscrSamplesSequence<Element>::Serialize(iser::IArchive& archive)
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

	if (isStoring){
		for (int i = 0; i < samplesCount; ++i){
			retVal = retVal && archive.BeginTag(channelsTag);
			for (int channelIndex = 0; channelIndex < channelsCount; ++channelIndex){
				Element sample = Element(GetDiscrSample(i, channelIndex));

				retVal = retVal && archive.Process(sample);
			}
			retVal = retVal && archive.EndTag(channelsTag);
		}
	}
	else{
		notifier.SetPtr(this);
		if (!CreateSequence(samplesCount, channelsCount)){
			return false;
		}

		for (int i = 0; i < samplesCount; ++i){
			retVal = retVal && archive.BeginTag(channelsTag);
			for (int channelIndex = 0; channelIndex < channelsCount; ++channelIndex){
				Element sample = 0;

				retVal = retVal && archive.Process(sample);

				SetDiscrSample(i, channelIndex, sample);
			}
			retVal = retVal && archive.EndTag(channelsTag);
		}
	}

	retVal = retVal && archive.EndTag(samplesListTag);

	return retVal;
}


typedef TDiscrSamplesSequence<I_BYTE> CSimpleSamplesSequence8;
typedef TDiscrSamplesSequence<I_WORD> CSimpleSamplesSequence16;
typedef TDiscrSamplesSequence<I_DWORD> CSimpleSamplesSequence32;


} // namespace imeas


#endif // !imeas_TDiscrSamplesSequence_included


