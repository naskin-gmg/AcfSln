#ifndef imeas_IDataSequenceStatistics_included
#define imeas_IDataSequenceStatistics_included


// ACF includes
#include "iser/ISerializable.h"


namespace imeas
{


class IDataStatistics;


/**
	Common interaface for data sequence statistics.
*/
class IDataSequenceStatistics: virtual public iser::ISerializable
{
public:
	/**
		Set channel statistics for the given channel index.
	*/
	virtual bool SetChannelStatistics(const imeas::IDataStatistics& channelStatistics, int channelIndex) = 0;

	/**
		Reset all statitics data.
	*/
	virtual void ResetStatistics() = 0;

	/**
		Get number of channels.
	*/
	virtual int GetChannelsCount() const = 0;

	/**
		Get data statics of the given channel \c channelIndex.
	*/
	virtual const imeas::IDataStatistics* GetChannelStatistics(int channelIndex) const = 0;
};


} // namespace imeas


#endif // !imeas_IDataSequenceStatistics_included


