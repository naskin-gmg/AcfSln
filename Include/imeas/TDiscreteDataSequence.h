#ifndef imeas_TDiscreteDataSequence_included
#define imeas_TDiscreteDataSequence_included


// Qt includes
#include <QtCore/qmath.h>

// ACF includes
#include "istd/TChangeNotifier.h"
#include "istd/TOptDelPtr.h"
#include "istd/TSmartPtr.h"
#include "iser/IArchive.h"
#include "iser/CArchiveTag.h"

#include "imeas/IDiscreteDataSequence.h"


namespace imeas
{


template <typename Element>
class TDiscreteDataSequence: virtual public IDiscreteDataSequence
{
public:
	TDiscreteDataSequence();

	// reimplemented (imeas::IDiscreteDataSequence)
	virtual bool CreateDiscreteSequence(
				int samplesCount,
				void* dataPtr,
				bool releaseFlag,
				int sampleDiff,
				int channelDiff,
				int sampleDepth,
				int channelsCount = 1);
	virtual bool CreateDiscreteSequenceWithInfo(
				const istd::TTransPtr<const IDataSequenceInfo>& infoPtr,
				int samplesCount,
				void* dataPtr,
				bool releaseFlag,
				int sampleDiff,
				int channelDiff,
				int sampleDepth,
				int channelsCount = 1);

	virtual int GetSampleDepth() const;
	virtual quint32 GetDiscreteSample(int position, int channel = 0) const;
	virtual bool SetDiscreteSample(int position, int channel, quint32 sample);

	// reimplemented (imeas::IDataSequence)
	virtual bool CreateSequence(int samplesCount, int channelsCount = 1);
	virtual bool CreateSequenceWithInfo(
				const istd::TTransPtr<const IDataSequenceInfo>& infoPtr,
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
TDiscreteDataSequence<Element>::TDiscreteDataSequence()
:	m_allocatedElementsCount(0),
	m_samplesCount(0),
	m_channelsCount(0),
	m_sampleDiff(0),
	m_channelDiff(0)
{
	m_normFactor = qPow(2.0, double(sizeof(Element) * 8)) - 1;
}


// reimplemented (imeas::IDiscreteDataSequence)

template <typename Element>
bool TDiscreteDataSequence<Element>::CreateDiscreteSequence(
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
bool TDiscreteDataSequence<Element>::CreateDiscreteSequenceWithInfo(
			const istd::TTransPtr<const IDataSequenceInfo>& infoPtr,
			int samplesCount,
			void* dataPtr,
			bool releaseFlag,
			int sampleDiff,
			int channelDiff,
			int sampleDepth,
			int channelsCount)
{
	m_sequenceInfoPtr = infoPtr;

	return CreateDiscreteSequence(samplesCount, dataPtr, releaseFlag, sampleDiff, channelDiff, sampleDepth, channelsCount);
}


template <typename Element>
int TDiscreteDataSequence<Element>::GetSampleDepth() const
{
	return sizeof(Element) * 8;
}


template <typename Element>
quint32 TDiscreteDataSequence<Element>::GetDiscreteSample(int position, int channel) const
{
	const Element& element = *(const Element*)((const quint8*)m_sampleBuffer.GetPtr() + position * m_sampleDiff + channel * m_channelDiff);

	return element;
}


template <typename Element>
bool TDiscreteDataSequence<Element>::SetDiscreteSample(int position, int channel, quint32 sample)
{
	Element& element = *(Element*)((quint8*)m_sampleBuffer.GetPtr() + position * m_sampleDiff + channel * m_channelDiff);

	element = Element(sample);

	return true;
}


// reimplemented (imeas::IDataSequence)

template <typename Element>
bool TDiscreteDataSequence<Element>::CreateSequence(int samplesCount, int channelsCount)
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
bool TDiscreteDataSequence<Element>::CreateSequenceWithInfo(
			const istd::TTransPtr<const IDataSequenceInfo>& infoPtr,
			int samplesCount,
			int channelsCount)
{
	m_sequenceInfoPtr = infoPtr;

	return CreateSequence(samplesCount, channelsCount);
}


template <typename Element>
const IDataSequenceInfo* TDiscreteDataSequence<Element>::GetSequenceInfo() const
{
	return m_sequenceInfoPtr.GetPtr();
}


template <typename Element>
bool TDiscreteDataSequence<Element>::IsEmpty() const
{
	return m_samplesCount <= 0;
}


template <typename Element>
void TDiscreteDataSequence<Element>::ResetSequence()
{
	m_sampleBuffer.Reset();
	m_samplesCount = 0;
	m_channelsCount = 0;
}


template <typename Element>
int TDiscreteDataSequence<Element>::GetSamplesCount() const
{
	return m_samplesCount;
}


template <typename Element>
int TDiscreteDataSequence<Element>::GetChannelsCount() const
{
	return m_channelsCount;
}


template <typename Element>
double TDiscreteDataSequence<Element>::GetSample(int index, int channel) const
{
	const Element& element = *(const Element*)((const quint8*)m_sampleBuffer.GetPtr() + index * m_sampleDiff + channel * m_channelDiff);

	return double(element) / m_normFactor;
}


template <typename Element>
void TDiscreteDataSequence<Element>::SetSample(int index, int channel, double value)
{
	Element& element = *(Element*)((quint8*)m_sampleBuffer.GetPtr() + index * m_sampleDiff + channel * m_channelDiff);

	element = Element(value * m_normFactor - I_BIG_EPSILON);
}


// reimplemented (iser::ISerializable)

template <typename Element>
bool TDiscreteDataSequence<Element>::Serialize(iser::IArchive& archive)
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
				Element sample = Element(GetDiscreteSample(i, channelIndex));

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

				SetDiscreteSample(i, channelIndex, sample);
			}
			retVal = retVal && archive.EndTag(channelsTag);
		}
	}

	retVal = retVal && archive.EndTag(samplesListTag);

	return retVal;
}


// reimplemented (istd::IChangeable)

template <typename Element>
bool TDiscreteDataSequence<Element>::CopyFrom(const istd::IChangeable& object)
{
	const IDataSequence* sequencePtr = dynamic_cast<const IDataSequence*>(&object);
	if (sequencePtr != NULL){
		const TDiscreteDataSequence<Element>* nativeSequencePtr = dynamic_cast<const TDiscreteDataSequence<Element>*>(sequencePtr);
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

			Q_ASSERT(samplesCount >= 0);
			Q_ASSERT(channelsCount >= 0);

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


typedef TDiscreteDataSequence<quint8> CSimpleSamplesSequence8;
typedef TDiscreteDataSequence<quint16> CSimpleSamplesSequence16;
typedef TDiscreteDataSequence<quint32> CSimpleSamplesSequence32;
typedef TDiscreteDataSequence<quint64> CSimpleSamplesSequence64;


} // namespace imeas


#endif // !imeas_TDiscreteDataSequence_included


