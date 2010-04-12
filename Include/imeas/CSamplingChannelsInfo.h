#ifndef imeas_CSamplingChannelsInfo_included
#define imeas_CSamplingChannelsInfo_included


#include "imeas/IChannelsInfo.h"


namespace imeas
{


class CSamplingChannelsInfo: virtual public IChannelsInfo
{
public:
	CSamplingChannelsInfo(int channelsCount, double samplingPeriod);

	/**
		Get sampling period of this sequence.
		Sampling period is time difference between samples.
		If returned value is not positive, no period is known.
	*/
	virtual double GetSamplingPeriod() const;

	// reimplemented (imeas::IChannelsInfo)
	virtual int GetChannelsCount() const;
	virtual istd::CRange GetValueRange(int channelIndex = -1) const;
	virtual bool HasChannelType(int channelType) const;
	virtual int GetChannelType(int channelIndex) const;

private:
	int m_channelsCount;
	double m_samplingPeriod;
};


} // namespace imeas


#endif // !imeas_CSamplingChannelsInfo_included


