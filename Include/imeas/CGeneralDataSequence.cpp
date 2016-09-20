#include <imeas/CGeneralDataSequence.h>


// STL includes
#include <cstring>

// ACF includes
#include <istd/CChangeNotifier.h>
#include <istd/TDelPtr.h>
#include <iser/IArchive.h>
#include <iser/CArchiveTag.h>

// ACF-Solutions
#include <imeas/CSamplesInfo.h>


namespace imeas
{


CGeneralDataSequence::CGeneralDataSequence()
:	m_channelsCount(0)
{
}


// reimplemented (imeas::IDataSequence)

bool CGeneralDataSequence::CreateSequence(int samplesCount, int channelsCount)
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

	m_channelsCount = channelsCount;

	if (m_sequenceInfoPtr.IsValid()){
		if (samplesCount < 0){
			samplesCount = m_sequenceInfoPtr->GetDefaultSamplesCount();
		}
		if (channelsCount < 0){
			channelsCount = m_sequenceInfoPtr->GetDefaultChannelsCount();
		}
	}

	if ((samplesCount <= 0) || (channelsCount <= 0)){
		return false;
	}

	m_samples.resize(samplesCount * m_channelsCount);

	return true;
}


bool CGeneralDataSequence::CreateSequenceWithInfo(
			const istd::TTransPtr<const IDataSequenceInfo>& infoPtr,
			int samplesCount,
			int channelsCount)
{
	m_sequenceInfoPtr = infoPtr;

	return CreateSequence(samplesCount, channelsCount);
}


const IDataSequenceInfo* CGeneralDataSequence::GetSequenceInfo() const
{
	return m_sequenceInfoPtr.GetPtr();
}


bool CGeneralDataSequence::IsEmpty() const
{
	return m_samples.empty();
}


void CGeneralDataSequence::ResetSequence()
{
	m_samples.clear();
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


// reimplemented (imath::ISampledFunction2d)

bool CGeneralDataSequence::CreateFunction(double* dataPtr, const ArgumentType& sizes)
{
	if (!sizes.IsSizeEmpty()){
		int elementsCount = sizes.GetProductVolume();

		m_samples.resize(elementsCount);

		std::memcpy(&m_samples[0], dataPtr, elementsCount * sizeof(double));

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
		const CSamplesInfo* infoPtr = m_sequenceInfoPtr.Cast<const CSamplesInfo*>();
		if (infoPtr != NULL){
			return infoPtr->GetLogicalSamplesRange();
		}
		else{
			return istd::CRange(0, GetSamplesCount());
		}
	}
	else if (dimensionIndex == 1){
		return istd::CRange(0, 1);
	}
	else{
		return istd::CRange::GetInvalid();
	}
}


istd::CRange CGeneralDataSequence::GetResultValueRange(int /*dimensionIndex*/, int /*resultDimension*/) const
{
	return istd::CRange::GetInvalid();
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

	return false;
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
	static iser::CArchiveTag channelsCountTag("ChannelsCount", "Number of channels", iser::CArchiveTag::TT_LEAF);
	static iser::CArchiveTag samplesListTag("SampleList", "List of sample values", iser::CArchiveTag::TT_MULTIPLE);
	static iser::CArchiveTag channelsTag("Channels", "List of sample values", iser::CArchiveTag::TT_LEAF, &samplesListTag);

	bool retVal = true;

	int channelsCount = GetChannelsCount();
	retVal = retVal && archive.BeginTag(channelsCountTag);
	retVal = retVal && archive.Process(channelsCount);
	retVal = retVal && archive.EndTag(channelsCountTag);

	int samplesCount = GetSamplesCount();

	retVal = retVal && archive.BeginMultiTag(samplesListTag, channelsTag, samplesCount);

	if (!retVal){
		return false;
	}

	bool isStoring = archive.IsStoring();

	istd::CChangeNotifier notifier(isStoring? NULL: this);

	if (!isStoring){
		if ((samplesCount < 0) || (channelsCount < 0) || !CreateSequence(samplesCount, channelsCount)){
			return false;
		}
	}

	int index = 0;
	for (int sampleIndex = 0; sampleIndex < samplesCount; ++sampleIndex){
		retVal = retVal && archive.BeginTag(channelsTag);
		for (int channelIndex = 0; channelIndex < channelsCount; ++channelIndex){
			retVal = retVal && archive.Process(m_samples[index++]);
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


bool CGeneralDataSequence::CopyFrom(const istd::IChangeable& object, CompatibilityMode /*mode*/)
{
	const IDataSequence* sequencePtr = dynamic_cast<const IDataSequence*>(&object);
	if (sequencePtr != NULL){
		istd::CChangeNotifier notifier(this);

		const CGeneralDataSequence* nativeSequencePtr = dynamic_cast<const CGeneralDataSequence*>(sequencePtr);
		if (nativeSequencePtr != NULL){
			m_samples = nativeSequencePtr->m_samples;
			m_channelsCount = nativeSequencePtr->m_channelsCount;
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


istd::IChangeable* CGeneralDataSequence::CloneMe(CompatibilityMode mode) const
{
	istd::TDelPtr<CGeneralDataSequence> clonePtr(new CGeneralDataSequence);

	if (clonePtr->CopyFrom(*this, mode)){
		return clonePtr.PopPtr();
	}

	return NULL;
}


} // namespace imeas


