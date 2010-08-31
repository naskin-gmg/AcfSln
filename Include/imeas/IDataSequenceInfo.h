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
		Get weight mode for this sequence.
		\sa	WeightMode
	*/
	virtual int GetWeightMode() const = 0;

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


#endif // !imeas_IDataSequenceInfo_included


