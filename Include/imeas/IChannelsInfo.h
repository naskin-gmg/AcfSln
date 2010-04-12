#ifndef imeas_IChannelsInfo_included
#define imeas_IChannelsInfo_included


#include "istd/IPolymorphic.h"
#include "istd/CRange.h"

#include "imeas/imeas.h"


namespace imeas
{


class IChannelsInfo: virtual public istd::IPolymorphic
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
		Get number of channels.
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


#endif // !imeas_IChannelsInfo_included


