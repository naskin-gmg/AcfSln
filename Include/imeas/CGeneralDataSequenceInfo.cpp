// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#include <imeas/CGeneralDataSequenceInfo.h>


// ACF includes
#include <istd/CChangeNotifier.h>
#include <iser/CArchiveTag.h>


namespace imeas
{


// static attributes

static const iser::CArchiveTag s_defaultSamplesCountTag("DefaultSamplesCount", "Default number of samples", iser::CArchiveTag::TT_LEAF);
static const iser::CArchiveTag s_defaultChannelsCountTag("DefaultChannelsCount", "Deafult number of channels", iser::CArchiveTag::TT_LEAF);
static const iser::CArchiveTag s_channelsListTag("Channels", "Number of channels", iser::CArchiveTag::TT_MULTIPLE);
static const iser::CArchiveTag s_channelTag("Channel", "List of channel names", iser::CArchiveTag::TT_GROUP, &s_channelsListTag);
static const iser::CArchiveTag s_nameTag("Name", "Name of channel", iser::CArchiveTag::TT_LEAF, &s_channelTag);
static const iser::CArchiveTag s_descriptionTag("Description", "Description of channel", iser::CArchiveTag::TT_LEAF, &s_channelTag);
static const iser::CArchiveTag s_valueIdTag("ValueId", "ID of the value", iser::CArchiveTag::TT_LEAF, &s_channelTag);
static const iser::CArchiveTag s_unitInfoTag("UnitInfo", "Description of channel unit", iser::CArchiveTag::TT_GROUP, &s_channelTag);


CGeneralDataSequenceInfo::CGeneralDataSequenceInfo(
			int defaultChannelsCount,
			int defaultSamplesCount,
			WeightMode mode,
			int sequenceFlags)
:	m_weightMode(mode),
	m_sequenceInfoFlags(sequenceFlags),
	m_defaultSamplesCount(defaultSamplesCount),
	m_defaultChannelsCount(defaultChannelsCount)
{
}


void CGeneralDataSequenceInfo::SetSequenceInfoFlags(int flags)
{
	if (flags != m_sequenceInfoFlags){
		istd::CChangeNotifier notifier(this);
		Q_UNUSED(notifier);

		m_sequenceInfoFlags = flags;
	}
}


void CGeneralDataSequenceInfo::SetDefaultSamplesCount(int samplesCount)
{
	if (samplesCount != m_defaultSamplesCount){
		istd::CChangeNotifier notifier(this);
		Q_UNUSED(notifier);

		m_defaultSamplesCount = samplesCount;
	}
}


void CGeneralDataSequenceInfo::SetDefaultChannelsCount(int channelsCount)
{
	if (channelsCount != m_defaultChannelsCount){
		istd::CChangeNotifier notifier(this);
		Q_UNUSED(notifier);

		m_defaultChannelsCount = channelsCount;
	}
}


void CGeneralDataSequenceInfo::SetWeightMode(WeightMode mode)
{
	if (m_weightMode != mode){
		istd::CChangeNotifier notifier(this);
		Q_UNUSED(notifier);

		m_weightMode = mode;
	}
}


// reimplemented (CGeneralNumericConstraints)

bool CGeneralDataSequenceInfo::InsertValueInfo(
			const QString& name,
			const QString& description,
			const QByteArray& valueId,
			const imath::CGeneralUnitInfo& unitInfo,
			int index)
{
	if (		((m_sequenceInfoFlags & IDataSequenceInfo::SIF_CHANNELS_COUNT_FIXED) != 0) &&
				(GetOptionsCount() >= m_defaultChannelsCount)){
		// cannot insert more than fixed
		return false;
	}

	return BaseClass::InsertValueInfo(name, description, valueId, unitInfo, index);
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
	bool retVal = true;

	retVal = retVal && archive.BeginTag(s_defaultSamplesCountTag);
	retVal = retVal && archive.Process(m_defaultSamplesCount);
	retVal = retVal && archive.EndTag(s_defaultSamplesCountTag);

	retVal = retVal && archive.BeginTag(s_defaultChannelsCountTag);
	retVal = retVal && archive.Process(m_defaultChannelsCount);
	retVal = retVal && archive.EndTag(s_defaultChannelsCountTag);

	int channelsCount = GetOptionsCount();

	retVal = retVal && archive.BeginMultiTag(s_channelsListTag, s_channelTag, channelsCount);
	if (!retVal){
		return false;
	}

	bool isStoring = archive.IsStoring();

	istd::CChangeNotifier notifier(isStoring? NULL : this);

	if (!isStoring){
		BaseClass::Reset();
	}

	for (int index = 0; index < channelsCount; ++index){
		retVal = retVal && archive.BeginTag(s_channelTag);

		QString channelName;
		QString channelDesciption;
		QByteArray valueId;
		imath::CGeneralUnitInfo unitInfo;

		if (isStoring){
			channelName = GetOptionName(index);
			channelDesciption = GetOptionDescription(index);
			valueId = GetOptionId(index);
			unitInfo = GetGeneralUnitInfo(index);
		}

		retVal = retVal && archive.BeginTag(s_nameTag);
		retVal = retVal && archive.Process(channelName);
		retVal = retVal && archive.EndTag(s_nameTag);

		retVal = retVal && archive.BeginTag(s_descriptionTag);
		retVal = retVal && archive.Process(channelDesciption);
		retVal = retVal && archive.EndTag(s_descriptionTag);

		retVal = retVal && archive.BeginTag(s_valueIdTag);
		retVal = retVal && archive.Process(valueId);
		retVal = retVal && archive.EndTag(s_valueIdTag);

		retVal = retVal && archive.BeginTag(s_unitInfoTag);
		retVal = retVal && unitInfo.Serialize(archive);
		retVal = retVal && archive.EndTag(s_unitInfoTag);

		retVal = retVal && archive.EndTag(s_channelTag);

		if (!isStoring){
			InsertValueInfo(channelName, channelDesciption, valueId, unitInfo);
		}
	}

	retVal = retVal && archive.EndTag(s_channelsListTag);

	return retVal;
}


} // namespace imeas


