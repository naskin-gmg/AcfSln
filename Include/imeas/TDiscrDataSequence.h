#ifndef imeas_TDiscrDataSequence_included
#define imeas_TDiscrDataSequence_included


// STL includes
#include <cstring>


// ACF includes
#include "istd/TChangeNotifier.h"
#include "istd/TOptDelPtr.h"
#include "istd/TSmartPtr.h"
#include "iser/IArchive.h"
#include "iser/CArchiveTag.h"

#include "imeas/IDiscrDataSequence.h"


namespace imeas
{


template <typename Element>
class TDiscrDataSequence: virtual public IDiscrDataSequence
{
public:
	TDiscrDataSequence();

	// reimplemented (imeas::IDiscrDataSequence)
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

	// reimplemented (imeas::IDataSequence)
	virtual bool CreateSequence(int samplesCount, int channelsCount = 1);
	virtual bool CreateSequenceWithInfo(
				istd::TTransPtr<const IDataSequenceInfo> infoPtr,
				int samplesCount = -1,
				int channelsCount = -1);
	virtual const IDataSequenceInfo* GetSequenceInfo() const;
	virtual bool IsEmpty() const;
	virtual void ResetSequence();
	virtual int GetSamplesCount() const;
	virtual int GetChannelsCount() const;
	virtual double GetSample(int index, int channel = 0) const;
	virtual void SetSample(int index, int channel, double value);

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);

	// reimplemented (istd::IChangeable)
	virtual bool CopyFrom(const istd::IChangeable& object);

private:
	istd::TOptDelPtr<Element, true> m_sampleBuffer;

	int m_allocatedElementsCount;
	int m_samplesCount;
	int m_channelsCount;
	int m_sampleDiff;
	int m_channelDiff;
	double m_normFactor;

	istd::TSmartPtr<const IDataSequenceInfo> m_sequenceInfoPtr;
};


// public methods

template <typename Element>
TDiscrDataSequence<Element>::TDiscrDataSequence()
:	m_allocatedElementsCount(0),
	m_samplesCount(0),
	m_channelsCount(0),
	m_sampleDiff(0),
	m_channelDiff(0)
{
	m_normFactor = ::pow(2.0, double(sizeof(Element) * 8)) - 1;
}


// reimplemented (imeas::IDiscrDataSequence)

template <typename Element>
bool TDiscrDataSequence<Element>::CreateDiscrSequence(
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
int TDiscrDataSequence<Element>::GetSampleDepth() const
{
	return sizeof(Element) * 8;
}


template <typename Element>
I_DWORD TDiscrDataSequence<Element>::GetDiscrSample(int position, int channel) const
{
	const Element& element = *(const Element*)((const I_BYTE*)m_sampleBuffer.GetPtr() + position * m_sampleDiff + channel * m_channelDiff);

	return element;
}


template <typename Element>
bool TDiscrDataSequence<Element>::SetDiscrSample(int position, int channel, I_DWORD sample)
{
	Element& element = *(Element*)((I_BYTE*)m_sampleBuffer.GetPtr() + position * m_sampleDiff + channel * m_channelDiff);

	element = Element(sample);

	return true;
}


// reimplemented (imeas::IDataSequence)

template <typename Element>
bool TDiscrDataSequence<Element>::CreateSequence(int samplesCount, int channelsCount)
{
	if (m_sequenceInfoPtr.IsValid()){
		int flags = m_sequenceInfoPtr->GetSequenceInfoFlags();

		if (samplesCount < 0){
			samplesCount = m_sequenceInfoPtr->GetDefaultSamplesCount();
		}
		else if (((flags & IDataSequenceInfo::SIF_SAMPLES_COUNT_FIXED) != 0) && (samplesCount != m_sequenceInfoPtr->GetDefaultSamplesCount())){
			return false;
		}

		if (channelsCount < 0){
			channelsCount = m_sequenceInfoPtr->GetDefaultChannelsCount();
		}
		else if (((flags & IDataSequenceInfo::SIF_CHANNELS_COUNT_FIXED) != 0) && (channelsCount != m_sequenceInfoPtr->GetDefaultChannelsCount())){
			return false;
		}
	}

	if ((samplesCount < 0) || (channelsCount < 0)){
		return false;
	}

	int elementsCount = samplesCount * channelsCount;
	if (		(elementsCount > m_allocatedElementsCount) ||
				(elementsCount * 2 < m_allocatedElementsCount)){
		m_sampleBuffer.SetPtr(new Element[elementsCount], true);
		m_allocatedElementsCount = elementsCount;
	}

	m_samplesCount = samplesCount;
	m_channelsCount = channelsCount;
	m_sampleDiff = channelsCount * sizeof(Element);
	m_channelDiff = sizeof(Element);

	return true;
}


template <typename Element>
bool TDiscrDataSequence<Element>::CreateSequenceWithInfo(
			istd::TTransPtr<const IDataSequenceInfo> infoPtr,
			int samplesCount,
			int channelsCount)
{
	m_sequenceInfoPtr = infoPtr;

	return CreateSequence(samplesCount, channelsCount);
}


template <typename Element>
const IDataSequenceInfo* TDiscrDataSequence<Element>::GetSequenceInfo() const
{
	return m_sequenceInfoPtr.GetPtr();
}


template <typename Element>
bool TDiscrDataSequence<Element>::IsEmpty() const
{
	return m_samplesCount <= 0;
}


template <typename Element>
void TDiscrDataSequence<Element>::ResetSequence()
{
	m_sampleBuffer.Reset();
	m_samplesCount = 0;
	m_channelsCount = 0;
}


template <typename Element>
int TDiscrDataSequence<Element>::GetSamplesCount() const
{
	return m_samplesCount;
}


template <typename Element>
int TDiscrDataSequence<Element>::GetChannelsCount() const
{
	return m_channelsCount;
}


template <typename Element>
double TDiscrDataSequence<Element>::GetSample(int index, int channel) const
{
	const Element& element = *(const Element*)((const I_BYTE*)m_sampleBuffer.GetPtr() + index * m_sampleDiff + channel * m_channelDiff);

	return double(element) / m_normFactor;
}


template <typename Element>
void TDiscrDataSequence<Element>::SetSample(int index, int channel, double value)
{
	Element& element = *(Element*)((I_BYTE*)m_sampleBuffer.GetPtr() + index * m_sampleDiff + channel * m_channelDiff);

	element = Element(value * m_normFactor - I_BIG_EPSILON);
}


// reimplemented (iser::ISerializable)

template <typename Element>
bool TDiscrDataSequence<Element>::Serialize(iser::IArchive& archive)
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
		if ((samplesCount < 0) || (channelsCount < 0) || !CreateSequence(samplesCount, channelsCount)){
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


// reimplemented (istd::IChangeable)

template <typename Element>
bool TDiscrDataSequence<Element>::CopyFrom(const istd::IChangeable& object)
{
	const IDataSequence* sequencePtr = dynamic_cast<const IDataSequence*>(&object);
	if (sequencePtr != NULL){
		const TDiscrDataSequence<Element>* nativeSequencePtr = dynamic_cast<const TDiscrDataSequence<Element>*>(sequencePtr);
		if (		(nativeSequencePtr != NULL) &&
					(nativeSequencePtr->m_channelDiff == sizeof(Element)) &&
					(nativeSequencePtr->m_sampleDiff == int(nativeSequencePtr->m_channelsCount * sizeof(Element)))){
			m_samplesCount = nativeSequencePtr->m_samplesCount;
			m_channelsCount = nativeSequencePtr->m_channelsCount;
			m_sampleDiff = sizeof(Element) * m_channelsCount;
			m_channelDiff = sizeof(Element);

			m_allocatedElementsCount = 0;
			m_allocatedElementsCount = m_samplesCount * m_channelsCount;
			if (m_allocatedElementsCount > 0){
				m_sampleBuffer.SetPtr(new Element[m_allocatedElementsCount], true);
				if (m_sampleBuffer.IsValid()){
					std::memcpy(		m_sampleBuffer.GetPtr(),
								nativeSequencePtr->m_sampleBuffer.GetPtr(),
								m_allocatedElementsCount * sizeof(Element));
				}
				else{
					ResetSequence();

					return false;
				}
			}
			else{
				m_sampleBuffer.Reset();
			}

		}
		else{
			int samplesCount = sequencePtr->GetSamplesCount();
			int channelsCount = sequencePtr->GetChannelsCount();

			I_ASSERT(samplesCount >= 0);
			I_ASSERT(channelsCount >= 0);

			if (!CreateSequence(samplesCount, channelsCount)){
				return false;
			}

			for (int sampleIndex = 0; sampleIndex < samplesCount; ++sampleIndex){
				for (int channelIndex = 0; channelIndex < channelsCount; ++channelIndex){
					double sample = sequencePtr->GetSample(sampleIndex, channelIndex);

					SetSample(sampleIndex, channelIndex, sample);
				}
			}
		}

		return true;
	}

	return false;
}


typedef TDiscrDataSequence<I_BYTE> CSimpleSamplesSequence8;
typedef TDiscrDataSequence<I_WORD> CSimpleSamplesSequence16;
typedef TDiscrDataSequence<I_DWORD> CSimpleSamplesSequence32;


} // namespace imeas


#endif // !imeas_TDiscrDataSequence_included


