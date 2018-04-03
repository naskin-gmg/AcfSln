#include <imeas/CSamplesInfo.h>


// Qt includes
#include <QtCore/QObject>

// ACF includes
#include <istd/CChangeNotifier.h>
#include <iser/IArchive.h>
#include <iser/CArchiveTag.h>
#include <iser/CPrimitiveTypesSerializer.h>


namespace imeas
{


CSamplesInfo::CSamplesInfo(const istd::CRange& logicalSamplesRange, const QDateTime& samplingStartTime)
:	m_logicalSamplesRange(logicalSamplesRange),
	m_samplingStartTime(samplingStartTime)
{
	QString time = m_samplingStartTime.toString();

	qDebug(qPrintable(time));
}


const istd::CRange& CSamplesInfo::GetLogicalSamplesRange() const
{
	return m_logicalSamplesRange;
}


void CSamplesInfo::SetLogicalSamplesRange(const istd::CRange& range)
{
	if (m_logicalSamplesRange != range){
		istd::CChangeNotifier changeNotifier(this);

		m_logicalSamplesRange = range;
	}
}


QDateTime CSamplesInfo::GetSamplingStartTime() const
{
	return m_samplingStartTime;
}


void CSamplesInfo::SetSamplingStartTime(const QDateTime& samplingStartTime)
{
	if (m_samplingStartTime != samplingStartTime){
		istd::CChangeNotifier changeNotifier(this);

		m_samplingStartTime = samplingStartTime;
	}
}


// reimplemented (imeas::IDataSequenceInfo)

int CSamplesInfo::GetSequenceInfoFlags() const
{
	return 0;
}


int CSamplesInfo::GetDefaultSamplesCount() const
{
	return -1;
}


int CSamplesInfo::GetDefaultChannelsCount() const
{
	return 1;
}


int CSamplesInfo::GetWeightMode() const
{
	return WM_NONE;
}


// reimplemented (imeas::INumericConstraints)

const iprm::IOptionsList& CSamplesInfo::GetValueListInfo() const
{
	return *this;
}


const imath::IUnitInfo* CSamplesInfo::GetNumericValueUnitInfo(int /*index*/) const
{
	return NULL;
}


// reimplemented (iser::ISerializable)

bool CSamplesInfo::Serialize(iser::IArchive& archive)
{
	bool retVal = true;

	istd::CChangeNotifier notifier(!archive.IsStoring() ? this : NULL);
	Q_UNUSED(notifier);

	static iser::CArchiveTag logicalRangeTag("LogicalSamplesRange", "Logical range of samples axis, e.g. sampled time span", iser::CArchiveTag::TT_GROUP);
	retVal = retVal && archive.BeginTag(logicalRangeTag);
	if (archive.IsStoring()){
		double minValue = m_logicalSamplesRange.GetMinValue();
		double maxValue = m_logicalSamplesRange.GetMaxValue();
		retVal = retVal && archive.Process(minValue);
		retVal = retVal && archive.Process(maxValue);
	}
	else{
		double minValue = 0;
		double maxValue = 0;
		retVal = retVal && archive.Process(minValue);
		retVal = retVal && archive.Process(maxValue);
		if (!retVal){
			return false;
		}

		m_logicalSamplesRange.SetMinValue(minValue);
		m_logicalSamplesRange.SetMaxValue(maxValue);
	}
	retVal = retVal && archive.EndTag(logicalRangeTag);

	static iser::CArchiveTag samplingStartTimeTag("SamplingStartTime", "Start time of the sample acquisition", iser::CArchiveTag::TT_GROUP);
	retVal = retVal && archive.BeginTag(samplingStartTimeTag);
	retVal = retVal && iser::CPrimitiveTypesSerializer::SerializeDateTime(archive, m_samplingStartTime);
	retVal = retVal && archive.EndTag(samplingStartTimeTag);

	return retVal;
}


// protected methods

// reimplemented (iprm::IOptionsList)

int CSamplesInfo::GetOptionsFlags() const
{
	return SCF_SUPPORT_UNIQUE_ID;
}


int CSamplesInfo::GetOptionsCount() const
{
	return qMax(0, GetDefaultChannelsCount());
}


QString CSamplesInfo::GetOptionName(int index) const
{
	return QObject::tr("Channel %1").arg(index + 1);
}


QString CSamplesInfo::GetOptionDescription(int /*index*/) const
{
	return QString();
}


QByteArray CSamplesInfo::GetOptionId(int index) const
{
	return QString("Channel%1").arg(index + 1).toUtf8();
}


bool CSamplesInfo::IsOptionEnabled(int /*index*/) const
{
	return true;
}


} // namespace imeas


