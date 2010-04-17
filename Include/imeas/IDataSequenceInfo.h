#ifndef imeas_IDataSequenceInfo_included
#define imeas_IDataSequenceInfo_included


#include "istd/IChangeable.h"
#include "istd/CRange.h"

#include "imeas/imeas.h"


namespace imeas
{


class IDataSequenceInfo: virtual public istd::IChangeable
{
public:
	enum ChannelType
	{
		/**
			Normal channel.
		*/
		CT_NORMAL,
		/**
			Discrete channel, data of this channel contains values without fractional part.
		*/
		CT_DISCRETE,
		/**
			Weight channel.
			It contains values in range [0, 1] representing weights of rest other channels.
		*/
		CT_WEIGHT
	};

	/**
		Get number of samples.
		If number of samples is undefined, it returns negative value.
	*/
	virtual int GetSamplesCount() const = 0;

	/**
		Get number of channels.
		If number of channels is undefined, it returns negative value.
	*/
	virtual int GetChannelsCount() const = 0;

	/**
		Get range of possible channel values.
		If this information is not supported, invalid range will be returned.
		\channelIndex	index of channel or negative value if all channels are meaned.
	*/
	virtual istd::CRange GetValueRange(int channelIndex = -1) const = 0;

	/**
		Check, if some channel type is present.
	*/
	virtual bool HasChannelType(int channelType) const = 0;

	/**
		Get type of specified channel.
	*/
	virtual int GetChannelType(int channelIndex) const = 0;
};


} // namespace imeas


#endif // !imeas_IDataSequenceInfo_included


