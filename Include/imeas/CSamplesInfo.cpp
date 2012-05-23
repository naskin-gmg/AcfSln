#include "imeas/CSamplesInfo.h"


// ACF includes
#include "istd/TChangeNotifier.h"

#include "iser/IArchive.h"
#include "iser/CArchiveTag.h"


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

int CSamplesInfo::GetDefaultSamplesCount() const
{
	return -1;
}


int CSamplesInfo::GetDefaultChannelsCount() const
{
	return 1;
}


// reimplemented (iser::ISerializable)

bool CSamplesInfo::Serialize(iser::IArchive& archive)
{
	static iser::CArchiveTag logicalRangeTag("LogicalSamplesRange", "Logical range of samples axis, e.g. sampled time span");

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


