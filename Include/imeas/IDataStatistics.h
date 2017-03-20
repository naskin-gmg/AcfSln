#ifndef imeas_IDataStatistics_included
#define imeas_IDataStatistics_included


#include <istd/TRange.h>

#include <iser/ISerializable.h>


namespace imeas
{


/**
	Common interace for simple data statistics.
	\TODO: think about moving it to imath.
*/
class IDataStatistics: virtual public istd::IChangeable
{
public:
	enum SupportedStatFeatures
	{
		SSF_BOUNDARIES = 1 << 0,
		SSF_STD_DEVIATION = 1 << 1,
		SSF_AVERAGE = 1 << 2,
		SSF_MEDIAN = 1 << 3
	};

	/**
		Get set of supported statistical features.
	*/
	virtual int GetSupportedStatFeatures() const = 0;

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


