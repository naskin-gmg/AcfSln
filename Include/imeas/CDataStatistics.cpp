#include "imeas/CDataStatistics.h"


#include "istd/TChangeNotifier.h"

#include "iser/IArchive.h"
#include "iser/CArchiveTag.h"


namespace imeas
{

CDataStatistics::CDataStatistics()
	:m_average(0.0),
	m_median(0.0),
	m_standardDeviation(0.0)
{
}


// reimplemented (imeas::IDataStatistics)

void CDataStatistics::CreateStatistics(
			double average,
			double median,
			double standardDeviation,
			const istd::CRange& dataBoundaries)
{
	istd::CChangeNotifier changePtr(NULL);

	if (dataBoundaries != m_dataBoundaries){
		changePtr.SetPtr(this);

		m_dataBoundaries = dataBoundaries;
	}

	if (standardDeviation != m_standardDeviation){
		changePtr.SetPtr(this);

		m_standardDeviation = standardDeviation;
	}
	
	if (average != m_average){
		changePtr.SetPtr(this);

		m_average = average;
	}
	
	if (median != m_median){
		changePtr.SetPtr(this);

		m_median = median;
	}
}


istd::CRange CDataStatistics::GetDataBoundaries() const
{
	return m_dataBoundaries;
}


double CDataStatistics::GetStandardDeviation() const
{
	return m_standardDeviation;
}


double CDataStatistics::GetAverage() const
{
	return m_average;
}


double CDataStatistics::GetMedian() const
{
	return m_median;
}



// reimplemented (iser::ISerializable)

bool CDataStatistics::Serialize(iser::IArchive& archive)
{
	bool retVal = true;

	static iser::CArchiveTag averageTag("Average", "Data average");
	retVal = retVal && archive.BeginTag(averageTag);
	retVal = retVal && archive.Process(m_average);
	retVal = retVal && archive.EndTag(averageTag);

	static iser::CArchiveTag medianTag("Median", "Data median");
	retVal = retVal && archive.BeginTag(medianTag);
	retVal = retVal && archive.Process(m_median);
	retVal = retVal && archive.EndTag(medianTag);

	static iser::CArchiveTag standardDeviationTag("StandardDeviation", "Data's standard deviation");
	retVal = retVal && archive.BeginTag(standardDeviationTag);
	retVal = retVal && archive.Process(m_standardDeviation);
	retVal = retVal && archive.EndTag(standardDeviationTag);

	double minValue = m_dataBoundaries.GetMinValue();
	double maxValue = m_dataBoundaries.GetMaxValue();

	static iser::CArchiveTag minValueTag("MinValue", "Minimal data value");
	retVal = retVal && archive.BeginTag(minValueTag);
	retVal = retVal && archive.Process(minValue);
	retVal = retVal && archive.EndTag(minValueTag);

	static iser::CArchiveTag maxValueTag("maxValue", "Maximal data value");
	retVal = retVal && archive.BeginTag(maxValueTag);
	retVal = retVal && archive.Process(maxValue);
	retVal = retVal && archive.EndTag(maxValueTag);

	if (!archive.IsStoring()){
		m_dataBoundaries = istd::CRange(minValue, maxValue);
	}

	return retVal;
}


} // namespace imeas