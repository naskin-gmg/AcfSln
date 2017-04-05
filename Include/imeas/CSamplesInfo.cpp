#include <imeas/CSamplesInfo.h>


// Qt includes
#include <QtCore/QObject>

// ACF includes
#include <istd/CChangeNotifier.h>

#include <iser/IArchive.h>
#include <iser/CArchiveTag.h>


namespace imeas
{


CSamplesInfo::CSamplesInfo(const istd::CRange& logicalSamplesRange)
:	m_logicalSamplesRange(logicalSamplesRange)
{
}


const istd::CRange& CSamplesInfo::GetLogicalSamplesRange() const
{
	return m_logicalSamplesRange;
}


void CSamplesInfo::SetLogicalSamplesRange(const istd::CRange& range)
{
	m_logicalSamplesRange = range;
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

int CSamplesInfo::GetNumericValuesCount() const
{
	return qMax(0, GetDefaultChannelsCount());
}


QString CSamplesInfo::GetNumericValueName(int index) const
{
	return QObject::tr("Channel %1").arg(index + 1);
}


QString CSamplesInfo::GetNumericValueDescription(int /*index*/) const
{
	return QString();
}


const imath::IUnitInfo* CSamplesInfo::GetNumericValueUnitInfo(int /*index*/) const
{
	return NULL;
}


// reimplemented (iser::ISerializable)

bool CSamplesInfo::Serialize(iser::IArchive& archive)
{
	static iser::CArchiveTag logicalRangeTag("LogicalSamplesRange", "Logical range of samples axis, e.g. sampled time span", iser::CArchiveTag::TT_GROUP);

	bool retVal = true;

	retVal = retVal && archive.BeginTag(logicalRangeTag);
	if (archive.IsStoring()){
		double minValue = m_logicalSamplesRange.GetMinValue();
		double maxValue = m_logicalSamplesRange.GetMaxValue();
		retVal = retVal && archive.Process(minValue);
		retVal = retVal && archive.Process(maxValue);
	}
	else{
		istd::CChangeNotifier notifier(this);
		Q_UNUSED(notifier);

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

	return retVal;
}


} // namespace imeas


