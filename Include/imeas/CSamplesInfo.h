#ifndef imeas_CSamplesInfo_included
#define imeas_CSamplesInfo_included


#include "imeas/IDataSequenceInfo.h"


namespace imeas
{


/**
	Stores additional data sequence data used to interpret samples value correctly.
*/
class CSamplesInfo: virtual public IDataSequenceInfo
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

	CSamplesInfo(const istd::CRange& logicalSamplesRange = istd::CRange::GetInvalid());

	/**
		Get logical position range (time axis) of this data sequence.
		For example, if you sample sound from second 1 to 5 it will be represented as range [1.0, 5.0].
		If this range is invalid, no logical range is provided.
	*/
	const istd::CRange& GetLogicalSamplesRange() const;
	/**
		Set logical position range (time axis) of this data sequence.
		\sa GetLogicalRange
	*/
	void SetLogicalSamplesRange(const istd::CRange& range);

	// reimplemented (imeas::IDataSequenceInfo)
	virtual int GetWeightMode() const;

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);

private:

	istd::CRange m_logicalSamplesRange;
};


} // namespace imeas


#endif // !imeas_CSamplesInfo_included


