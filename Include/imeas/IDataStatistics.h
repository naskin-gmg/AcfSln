#ifndef imeas_IDataStatistics_included
#define imeas_IDataStatistics_included


#include "istd/CRange.h"

#include "iser/ISerializable.h"


namespace imeas
{


class IDataStatistics: virtual public iser::ISerializable
{
public:

	/**
		Create statistics object from the input data.
	*/
	virtual void CreateStatistics(
				double average,
				double median,
				double standardDeviation,
				const istd::CRange& dataBoundaries) = 0;

	/**
		Get the range of values in the data sequence.
	*/
	virtual istd::CRange GetDataBoundaries() const = 0;

	/**
		Get data variance.
	*/
	virtual double GetStandardDeviation() const = 0;

	/**
		Get data average.
	*/
	virtual double GetAverage() const = 0;

	/**
		Get data median.
	*/
	virtual double GetMedian() const = 0;
};


} // namespace imeas


#endif // !imeas_IDataStatistics_included


