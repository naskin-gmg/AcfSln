#include "imeas/CGeneralSamplesSequence.h"


#include "istd/TChangeNotifier.h"

#include "iser/IArchive.h"
#include "iser/CArchiveTag.h"

#include "imeas/IChannelsInfo.h"
#include "imeas/CSamplingChannelsInfo.h"


namespace imeas
{


CGeneralSamplesSequence::CGeneralSamplesSequence()
:	m_channelsCount(0)
{
}


bool CGeneralSamplesSequence::CreateSequence(int samplesCount, int channelsCount)
{
	I_ASSERT(samplesCount >= 0);
	I_ASSERT(channelsCount >= 0);

	m_channelsCount = channelsCount;

	m_samples.resize(samplesCount * m_channelsCount, 0.0);

	return true;
}


// reimplemented (imeas::IDataSequence)

bool CGeneralSamplesSequence::CreateSequence(int samplesCount, const IChannelsInfo* infoPtr, bool releaseInfoFlag)
{
	m_channelsInfoPtr.SetPtr(infoPtr, releaseInfoFlag);

	if (infoPtr != NULL){
		return CreateSequence(samplesCount, infoPtr->GetChannelsCount());
	}
	else{
		return CreateSequence(samplesCount, 1);
	}
}


const IChannelsInfo* CGeneralSamplesSequence::GetChannelsInfo() const
{
	return m_channelsInfoPtr.GetPtr();
}


bool CGeneralSamplesSequence::IsEmpty() const
{
	return m_samples.empty();
}


void CGeneralSamplesSequence::ResetSequence()
{
	m_samples.clear();
	m_channelsInfoPtr.Reset();
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
	I_ASSERT(channel >= 0);
	I_ASSERT(channel < m_channelsCount);

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


int CGeneralSamplesSequence::GetTotalSamplesCount() const
{
	return GetSamplesCount() * GetChannelsCount();
}


int CGeneralSamplesSequence::GetGridSize(int dimensionIndex) const
{
	if (dimensionIndex == 0){
		return GetSamplesCount();
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
		const CSamplingChannelsInfo* infoPtr = dynamic_cast<const CSamplingChannelsInfo*>(m_channelsInfoPtr.GetPtr());
		return istd::CRange(0, GetSamplesCount() * (infoPtr != NULL)? infoPtr->GetSamplingPeriod(): 1);
	}
	else if (dimensionIndex == 1){
		return istd::CRange(0, 1);
	}
	else{
		return istd::CRange::GetInvalid();
	}
}


istd::CRange CGeneralSamplesSequence::GetResultValueRange(int dimensionIndex, int /*resultDimension*/) const
{
	if ((dimensionIndex == 0) && m_channelsInfoPtr.IsValid()){
		return m_channelsInfoPtr->GetValueRange(-1);
	}

	return istd::CRange(-1, 1);
}


// reimplemented (imath::TIMathFunction)

bool CGeneralSamplesSequence::GetValueAt(const ArgumentType& argument, ResultType& result) const
{
	int sampleIndex = argument[0];
	int channelIndex = argument[1];
	if ((sampleIndex >= 0) && (sampleIndex < GetSamplesCount()) && (channelIndex >= 0) && (channelIndex < 2)){
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
	if ((sampleIndex >= 0) && (sampleIndex < GetSamplesCount()) && (channelIndex >= 0) && (channelIndex < 2)){
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


// reimplemented (istd::IChangeable)

bool CGeneralSamplesSequence::CopyFrom(const istd::IChangeable& object)
{
	const IDataSequence* sequencePtr = dynamic_cast<const IDataSequence*>(&object);
	if (sequencePtr != NULL){
		istd::CChangeNotifier notifier(this);

		const CGeneralSamplesSequence* nativeSequencePtr = dynamic_cast<const CGeneralSamplesSequence*>(sequencePtr);
		if (nativeSequencePtr != NULL){
			m_samples = nativeSequencePtr->m_samples;
			m_channelsCount = nativeSequencePtr->m_channelsCount;
		}
		else{
			int samplesCount = sequencePtr->GetSamplesCount();
			int channelsCount = sequencePtr->GetChannelsCount();

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

		const CSamplingChannelsInfo* infoPtr = dynamic_cast<const CSamplingChannelsInfo*>(sequencePtr->GetChannelsInfo());
		if (infoPtr != NULL){
			m_channelsInfoPtr.SetPtr(new CSamplingChannelsInfo(m_channelsCount, infoPtr->GetSamplingPeriod()), true);
		}
		else{
			m_channelsInfoPtr.Reset();
		}

		return true;
	}

	return false;
}


} // namespace imeas


