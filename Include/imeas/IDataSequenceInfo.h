#ifndef imeas_IDataSequenceInfo_included
#define imeas_IDataSequenceInfo_included


// ACF includes
#include "istd/CRange.h"
#include "iser/ISerializable.h"

#include "imeas/imeas.h"


namespace imeas
{


/**
	Stores additional data sequence data used to interpret samples value correctly.
*/
class IDataSequenceInfo: virtual public iser::ISerializable
{
public:
	enum SequenceInfoFlags
	{
		/**
			Number of samples is fixed by this info object.
		*/
		SIF_SAMPLES_COUNT_FIXED = 1 << 1,
		/**
			Number of channels is fixed by this info object.
		*/
		SIF_CHANNELS_COUNT_FIXED = 1 << 2
	};

	/**
		Describe mode of weight information.
	*/
	enum WeightMode
	{
		/**
			There is no weights.
		*/
		WM_NONE,
		/**
			The last channel (if more than 1) describes weight values, weights are shared for all channels.
		*/
		WM_LAST,
		/**
			The weight channels are interleaved with 'normal' channels (e.g. for 6 channels it will be sequence NWNWNW, N - normal, W - weight channel).
		*/
		WM_INTERLEAVED,
		/**
			There is unknown weight channel interpretation.
		*/
		WM_UNKNOWN
	};

	/**
		Get flags for this data sequence.
	*/
	virtual int GetSequenceInfoFlags() const = 0;

	/**
		Get number of samples if there are fixed.
	*/
	virtual int GetFixedSamplesCount() const = 0;
	/**
		Get number of channels if there are fixed.
	*/
	virtual int GetFixedChannelsCount() const = 0;

	/**
		Get weight mode for this sequence.
		\sa	WeightMode
	*/
	virtual int GetWeightMode() const = 0;
};


} // namespace imeas


#endif // !imeas_IDataSequenceInfo_included


