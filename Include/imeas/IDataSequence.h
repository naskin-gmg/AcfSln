#ifndef imeas_IDataSequence_included
#define imeas_IDataSequence_included


// ACF includes
#include "iser/ISerializable.h"

#include "imeas/IDataSequenceInfo.h"


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
		Set additional information about this sequence.
	*/
	virtual const IDataSequenceInfo* GetSequenceInfo() const = 0;

	/**
		Set additional information about this sequence.
	*/
	virtual bool SetSequenceInfo(const IDataSequenceInfo* infoPtr, bool releaseFlag = false) = 0;

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
};


} // namespace imeas


#endif // !imeas_IDataSequence_included


