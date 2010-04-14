#include "imeas/CGeneralDataSequence.h"


#include "istd/TChangeNotifier.h"
#include "istd/TDelPtr.h"

#include "iser/IArchive.h"
#include "iser/CArchiveTag.h"

#include "imeas/IChannelsInfo.h"


namespace imeas
{


CGeneralDataSequence::CGeneralDataSequence()
:	m_channelsCount(0)
{
}


bool CGeneralDataSequence::CreateSequence(int samplesCount, int channelsCount)
{
	I_ASSERT(samplesCount >= 0);
	I_ASSERT(channelsCount >= 0);

	m_channelsCount = channelsCount;

	m_samples.resize(samplesCount * m_channelsCount, 0.0);

	return true;
}


// reimplemented (imeas::IDataSequence)

bool CGeneralDataSequence::CreateSequence(int samplesCount, const IChannelsInfo* infoPtr, bool releaseInfoFlag)
{
	m_channelsInfoPtr.SetPtr(infoPtr, releaseInfoFlag);

	if (infoPtr != NULL){
		return CreateSequence(samplesCount, infoPtr->GetChannelsCount());
	}
	else{
		return CreateSequence(samplesCount, 1);
	}
}


const IChannelsInfo* CGeneralDataSequence::GetChannelsInfo() const
{
	return m_channelsInfoPtr.GetPtr();
}


bool CGeneralDataSequence::IsEmpty() const
{
	return m_samples.empty();
}


void CGeneralDataSequence::ResetSequence()
{
	m_samples.clear();
	m_channelsInfoPtr.Reset();
}


int CGeneralDataSequence::GetSamplesCount() const
{
	if (m_channelsCount > 0){
		return int(m_samples.size() / m_channelsCount);
	}
	else{
		return 0;
	}
}


int CGeneralDataSequence::GetChannelsCount() const
{
	return m_channelsCount;
}


double CGeneralDataSequence::GetSample(int index, int channel) const
{
	I_ASSERT(index >= 0);
	I_ASSERT(index * m_channelsCount + channel < int(m_samples.size()));

	return m_samples[index * m_channelsCount + channel];
}


void CGeneralDataSequence::SetSample(int index, int channel, double value)
{
	I_ASSERT(index >= 0);
	I_ASSERT(index * m_channelsCount + channel < int(m_samples.size()));
	I_ASSERT(channel >= 0);
	I_ASSERT(channel < m_channelsCount);

	m_samples[index * m_channelsCount + channel] = value;
}


// reimplemented (imath::ISampledFunction2d)

bool CGeneralDataSequence::CreateFunction(double* dataPtr, const ArgumentType& sizes)
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


int CGeneralDataSequence::GetTotalSamplesCount() const
{
	return GetSamplesCount() * GetChannelsCount();
}


int CGeneralDataSequence::GetGridSize(int dimensionIndex) const
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


istd::CRange CGeneralDataSequence::GetLogicalRange(int dimensionIndex) const
{
	if (dimensionIndex == 0){
		return istd::CRange(0, GetSamplesCount());
	}
	else if (dimensionIndex == 1){
		return istd::CRange(0, 1);
	}
	else{
		return istd::CRange::GetInvalid();
	}
}


istd::CRange CGeneralDataSequence::GetResultValueRange(int dimensionIndex, int /*resultDimension*/) const
{
	if ((dimensionIndex == 0) && m_channelsInfoPtr.IsValid()){
		return m_channelsInfoPtr->GetValueRange(-1);
	}

	return istd::CRange(-1, 1);
}


// reimplemented (imath::TIMathFunction)

bool CGeneralDataSequence::GetValueAt(const ArgumentType& argument, ResultType& result) const
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


CGeneralDataSequence::ResultType CGeneralDataSequence::GetValueAt(const ArgumentType& argument) const
{
	CGeneralDataSequence::ResultType retVal;

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

bool CGeneralDataSequence::Serialize(iser::IArchive& archive)
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

int CGeneralDataSequence::GetSupportedOperations() const
{
	return SO_COPY | SO_CLONE;
}


bool CGeneralDataSequence::CopyFrom(const istd::IChangeable& object)
{
	const IDataSequence* sequencePtr = dynamic_cast<const IDataSequence*>(&object);
	if (sequencePtr != NULL){
		istd::CChangeNotifier notifier(this);

		const CGeneralDataSequence* nativeSequencePtr = dynamic_cast<const CGeneralDataSequence*>(sequencePtr);
		if (nativeSequencePtr != NULL){
			if (nativeSequencePtr->m_channelsInfoPtr.IsValid() && nativeSequencePtr->m_channelsInfoPtr.IsToRelase()){
				m_channelsInfoPtr.SetCastedOrRemove(nativeSequencePtr->m_channelsInfoPtr->CloneMe());
				if (!m_channelsInfoPtr.IsValid()){
					return false;
				}
			}
			else{
				m_channelsInfoPtr.SetPtr(nativeSequencePtr->m_channelsInfoPtr.GetPtr(), false);
			}

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

		return true;
	}

	return false;
}


istd::IChangeable* CGeneralDataSequence::CloneMe() const
{
	istd::TDelPtr<CGeneralDataSequence> clonePtr(new CGeneralDataSequence);

	if (clonePtr->CopyFrom(*this)){
		return clonePtr.PopPtr();
	}

	return NULL;
}


} // namespace imeas


