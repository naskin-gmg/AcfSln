#ifndef imeas_IDataSequence_included
#define imeas_IDataSequence_included


// ACF includes
#include "istd/TRetSmartPtr.h"
#include "istd/CRange.h"
#include "iser/ISerializable.h"

#include "imeas/imeas.h"


namespace imeas
{


/**
	General definition of sequence contains samples in regular time grid.
	All samples has the same number of channels components.
*/
class IDataSequence: virtual public iser::ISerializable
{
public:
	/**
		Create container for sample sequence with specified number of samples, channels and sample depth.
		\param	samplesCount	number of samples.
		\return					true if sample sequence was created correctly.
	*/
	virtual bool CreateSequence(int samplesCount, int channelsCount = 1) = 0;

	/**
		Return true if this sequence has no sample.
	*/
	virtual bool IsEmpty() const = 0;

	/**
		Reset this sequence.
		After this operation sequence is set to initial state.
	*/
	virtual void ResetSequence() = 0;

	/**
		Get size of this raster sequence.
	*/
	virtual int GetSamplesCount() const = 0;

	/**
		Get number of channels.
	*/
	virtual int GetChannelsCount() const = 0;

	/**
		Get sample value at specified index.
	*/
	virtual double GetSample(int index, int channel = 0) const = 0;

	/**
		Set sample value at specified index.
	*/
	virtual void SetSample(int index, int channel, double value) = 0;

	/**
		Get logical position range (time axis) of this data sequence.
		For example, if you sample sound from second 1 to 5 it will be represented as range [1.0, 5.0].
		If this range is invalid, no logical range is provided.
	*/
	virtual const istd::CRange& GetLogicalSamplesRange() const = 0;
	/**
		Set logical position range (time axis) of this data sequence.
		\sa GetLogicalRange
	*/
	virtual void SetLogicalSamplesRange(const istd::CRange& range) = 0;
};


} // namespace imeas


#endif // !imeas_IDataSequence_included


