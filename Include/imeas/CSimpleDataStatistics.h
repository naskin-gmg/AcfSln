#ifndef imeas_CSimpleDataStatistics_included
#define imeas_CSimpleDataStatistics_included


#include <imeas/IDataStatistics.h>


namespace imeas
{


/**
	General implementation of the IDataStatistics interface.
*/
class CSimpleDataStatistics:
			virtual public imeas::IDataStatistics,
			virtual public iser::ISerializable
{
public:
	CSimpleDataStatistics();
	CSimpleDataStatistics(
				double average,
				double median,
				double standardDeviation,
				const istd::CRange& dataBoundaries);

	virtual void CreateStatistics(
				double average,
				double median,
				double standardDeviation,
				const istd::CRange& dataBoundaries);

	// reimplemented (imeas::IDataStatistics)
	virtual int GetSupportedStatFeatures() const override;
	virtual istd::CRange GetDataBoundaries() const override;
	virtual double GetStandardDeviation() const override;
	virtual double GetAverage() const override;
	virtual double GetMedian() const override;

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive) override;

private:
	double m_average;
	double m_median;
	double m_standardDeviation;
	istd::CRange m_dataBoundaries;
};


} // namespace imeas


#endif // !imeas_CSimpleDataStatistics_included


