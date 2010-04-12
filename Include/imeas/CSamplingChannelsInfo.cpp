#include "imeas/CSamplingChannelsInfo.h"


namespace imeas
{


CSamplingChannelsInfo::CSamplingChannelsInfo(int channelsCount, double samplingPeriod)
:	m_channelsCount(channelsCount), m_samplingPeriod(samplingPeriod)
{
}


double CSamplingChannelsInfo::GetSamplingPeriod() const
{
	return m_samplingPeriod;
}


// reimplemented (imeas::IChannelsInfo)

int CSamplingChannelsInfo::GetChannelsCount() const
{
	return m_channelsCount;
}


istd::CRange CSamplingChannelsInfo::GetValueRange(int /*channelIndex*/) const
{
	return istd::CRange::GetInvalid();
}


bool CSamplingChannelsInfo::HasChannelType(int channelType) const
{
	return (channelType == CT_NORMAL);
}


int CSamplingChannelsInfo::GetChannelType(int /*channelIndex*/) const
{
	return CT_NORMAL;
}


} // namespace imeas


