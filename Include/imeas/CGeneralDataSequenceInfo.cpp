#include <imeas/CGeneralDataSequenceInfo.h>


#include <istd/CChangeNotifier.h>


namespace imeas
{

CGeneralDataSequenceInfo::CGeneralDataSequenceInfo(int defaultChannelsCount,
			int defaultSamplesCount,
			WeightMode mode,
			int sequenceFlags)
	:m_weightMode(mode),
	m_sequenceInfoFlags(sequenceFlags),
	m_defaultSamplesCount(defaultSamplesCount),
	m_defaultChannelsCount(defaultChannelsCount)
{
}


bool CGeneralDataSequenceInfo::InsertValueInfo(const QString& name,
			const QString& description,
			const imath::CGeneralUnitInfo& unitInfo,
			int index)
{
	if (((m_sequenceInfoFlags & IDataSequenceInfo::SIF_CHANNELS_COUNT_FIXED) != 0)
		&& (GetNumericValuesCount() + 1 > m_defaultChannelsCount)){
		// cannot insert more than fixed
		return false;
	}

	istd::CChangeNotifier changeNotifier(this);

	BaseClass::InsertValueInfo(name, description, unitInfo, index);

	return true;
}



void CGeneralDataSequenceInfo::SetSequenceInfoFlags(unsigned flags)
{
	istd::CChangeNotifier changeNotifier(this);

	m_sequenceInfoFlags = flags;
}


void CGeneralDataSequenceInfo::SetDefaultSamplesCount(unsigned samplesCount)
{
	if (samplesCount != m_defaultSamplesCount){

		istd::CChangeNotifier changeNotifier(this);

		m_defaultSamplesCount = samplesCount;
	}
}


void CGeneralDataSequenceInfo::SetDefaultChannelsCount(unsigned channelsCount)
{
	if (channelsCount != m_defaultChannelsCount){

		istd::CChangeNotifier changeNotifier(this);

		m_defaultChannelsCount = channelsCount;
	}
}


void CGeneralDataSequenceInfo::SetWeightMode(WeightMode mode)
{
	if (m_weightMode != mode){

		istd::CChangeNotifier changeNotifier(this);

		m_weightMode = mode;
	}
}


// reimplemented (imeas::IDataSequenceInfo)
int CGeneralDataSequenceInfo::GetSequenceInfoFlags() const
{
	return m_sequenceInfoFlags;
}


int CGeneralDataSequenceInfo::GetDefaultSamplesCount() const
{
	return m_defaultSamplesCount;
}


int CGeneralDataSequenceInfo::GetDefaultChannelsCount() const
{
	return m_defaultChannelsCount;
}


int CGeneralDataSequenceInfo::GetWeightMode() const
{
	return m_weightMode;
}


// reimplemented (iser::ISerializable)
bool CGeneralDataSequenceInfo::Serialize(iser::IArchive& archive)
{

	static iser::CArchiveTag channelsListTag("DefaultChannelsCount", "Number of channels", iser::CArchiveTag::TT_MULTIPLE);
	static iser::CArchiveTag samplesCountTag("DefaultSamplesCount", "Number of samples", iser::CArchiveTag::TT_LEAF);
	static iser::CArchiveTag channelsTag("Channels", "List of channel names", iser::CArchiveTag::TT_LEAF, &channelsListTag);


	bool retVal = true;

	retVal = retVal && archive.BeginTag(samplesCountTag);
	retVal = retVal && archive.Process(m_defaultSamplesCount);
	retVal = retVal && archive.EndTag(samplesCountTag);


	int numericChannelsCount = GetNumericValuesCount();
	int channelsCount = qMax(m_defaultChannelsCount, numericChannelsCount);

	retVal = retVal && archive.BeginMultiTag(channelsListTag, channelsTag, channelsCount);

	if (!retVal) {
		return false;
	}

	bool isStoring = archive.IsStoring();

	istd::CChangeNotifier notifier(isStoring ? NULL : this);

	if (!isStoring) {
		if ((m_defaultSamplesCount < 0) || (m_defaultChannelsCount < 0)){
			return false;
		}
	}

	for (int index = 0; index < channelsCount; ++index) {
		retVal = retVal && archive.BeginTag(channelsTag);

		QString channelName;
		QString channelDesciption;

		if (index < numericChannelsCount){
			channelName = GetNumericValueName(index);
			channelDesciption = GetNumericValueDescription(index);
		}

		retVal = retVal && archive.Process(channelName);
		retVal = retVal && archive.Process(channelDesciption);

		retVal = retVal && archive.EndTag(channelsTag);
	}

	return retVal;
}

} // namespace imeas

