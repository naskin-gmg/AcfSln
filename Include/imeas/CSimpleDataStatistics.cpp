#include <imeas/CSimpleDataStatistics.h>


// ACF includes
#include <istd/CChangeNotifier.h>
#include <istd/CChangeGroup.h>
#include <iser/IArchive.h>
#include <iser/CArchiveTag.h>


namespace imeas
{


CSimpleDataStatistics::CSimpleDataStatistics()
:	m_average(0.0),
	m_median(0.0),
	m_standardDeviation(0.0)
{
}


CSimpleDataStatistics::CSimpleDataStatistics(
				double average,
				double median,
				double standardDeviation,
				const istd::CRange& dataBoundaries)
:	m_average(average),
	m_median(median),
	m_standardDeviation(standardDeviation),
	m_dataBoundaries(dataBoundaries)
{
}


void CSimpleDataStatistics::CreateStatistics(
			double average,
			double median,
			double standardDeviation,
			const istd::CRange& dataBoundaries)
{
	istd::CChangeGroup changeGroup(this);
	Q_UNUSED(changeGroup);

	if (dataBoundaries != m_dataBoundaries){
		istd::CChangeNotifier notifier(this);

		m_dataBoundaries = dataBoundaries;
	}

	if (standardDeviation != m_standardDeviation){
		istd::CChangeNotifier notifier(this);

		m_standardDeviation = standardDeviation;
	}
	
	if (average != m_average){
		istd::CChangeNotifier notifier(this);

		m_average = average;
	}
	
	if (median != m_median){
		istd::CChangeNotifier notifier(this);

		m_median = median;
	}
}


// reimplemented (imeas::IDataStatistics)

int CSimpleDataStatistics::GetSupportedStatFeatures() const
{
	return SSF_BOUNDARIES | SSF_AVERAGE | SSF_AVERAGE | SSF_MEDIAN;
}


istd::CRange CSimpleDataStatistics::GetDataBoundaries() const
{
	return m_dataBoundaries;
}


double CSimpleDataStatistics::GetStandardDeviation() const
{
	return m_standardDeviation;
}


double CSimpleDataStatistics::GetAverage() const
{
	return m_average;
}


double CSimpleDataStatistics::GetMedian() const
{
	return m_median;
}



// reimplemented (iser::ISerializable)

bool CSimpleDataStatistics::Serialize(iser::IArchive& archive)
{
	static iser::CArchiveTag averageTag("Average", "Data average", iser::CArchiveTag::TT_LEAF);
	static iser::CArchiveTag medianTag("Median", "Data median", iser::CArchiveTag::TT_LEAF);
	static iser::CArchiveTag standardDeviationTag("StandardDeviation", "Data's standard deviation", iser::CArchiveTag::TT_LEAF);
	static iser::CArchiveTag minValueTag("MinValue", "Minimal data value", iser::CArchiveTag::TT_LEAF);
	static iser::CArchiveTag maxValueTag("maxValue", "Maximal data value", iser::CArchiveTag::TT_LEAF);

	bool isStoring = archive.IsStoring();

	istd::CChangeNotifier notifier(isStoring? NULL: this);

	bool retVal = true;

	retVal = retVal && archive.BeginTag(averageTag);
	retVal = retVal && archive.Process(m_average);
	retVal = retVal && archive.EndTag(averageTag);

	retVal = retVal && archive.BeginTag(medianTag);
	retVal = retVal && archive.Process(m_median);
	retVal = retVal && archive.EndTag(medianTag);

	retVal = retVal && archive.BeginTag(standardDeviationTag);
	retVal = retVal && archive.Process(m_standardDeviation);
	retVal = retVal && archive.EndTag(standardDeviationTag);

	double minValue = m_dataBoundaries.GetMinValue();
	double maxValue = m_dataBoundaries.GetMaxValue();

	retVal = retVal && archive.BeginTag(minValueTag);
	retVal = retVal && archive.Process(minValue);
	retVal = retVal && archive.EndTag(minValueTag);

	retVal = retVal && archive.BeginTag(maxValueTag);
	retVal = retVal && archive.Process(maxValue);
	retVal = retVal && archive.EndTag(maxValueTag);

	if (!isStoring){
		m_dataBoundaries = istd::CRange(minValue, maxValue);
	}

	return retVal;
}


} // namespace imeas


