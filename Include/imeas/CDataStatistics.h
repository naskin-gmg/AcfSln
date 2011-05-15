#ifndef imeas_CDataStatistics_included
#define imeas_CDataStatistics_included


#include "imeas/IDataStatistics.h"


namespace imeas
{


class CDataStatistics: virtual public imeas::IDataStatistics
{
public:
	CDataStatistics();

	// reimplemented (imeas::IDataStatistics)
	virtual void CreateStatistics(
				double average,
				double median,
				double standardDeviation,
				const istd::CRange& dataBoundaries);
	virtual istd::CRange GetDataBoundaries() const;
	virtual double GetStandardDeviation() const;
	virtual double GetAverage() const;
	virtual double GetMedian() const;

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);

private:
	double m_average;
	double m_median;
	double m_standardDeviation;
	istd::CRange m_dataBoundaries;
};


} // namespace imeas


#endif // !imeas_CDataStatistics_included


