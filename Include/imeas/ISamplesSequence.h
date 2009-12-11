#ifndef imeas_ISamplesSequence_included
#define imeas_ISamplesSequence_included


#include "iser/ISerializable.h"

#include "imeas/imeas.h"


namespace imeas
{


/**
	General definition of sequence contains samples in regular time grid.
	All samples has the same number of channels components.
*/
class ISamplesSequence: virtual public iser::ISerializable
{
public:
	/**
		Create container for sample sequence with specified number of samples, channels and sample depth.
		\param	samplesCount	number of samples.
		\param	channelsCount	number of channels.
		\return					true if sample sequence was created correctly.
	*/
	virtual bool CreateSequence(int timeSamplesCount, int channelsCount = 1) = 0;

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
	virtual int GetTimeSamplesCount() const = 0;

	/**
		Get number of color components.
	*/
	virtual int GetChannelsCount() const = 0;

	/**
		Get sampling period of this sequence.
		Sampling period is time difference between samples.
		If returned value is not positive, no period is known.
	*/
	virtual double GetSamplingPeriod() const = 0;

	/**
		Set sampling period of this sequence.
		Sampling period is time difference between samples.
		If value is not positive, it means no period is known.
	*/
	virtual void SetSamplingPeriod(double value) = 0;

	/**
		Get sample value at specified index.
	*/
	virtual double GetSample(int index, int channel = 0) const = 0;

	/**
		Set sample value at specified index.
	*/
	virtual void SetSample(int index, int channel, double value) = 0;
};


} // namespace imeas


#endif // !imeas_ISamplesSequence_included


