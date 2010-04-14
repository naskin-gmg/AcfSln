#include "imeas/CSamplingChannelsInfo.h"


#include "istd/TDelPtr.h"


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


// reimplemented (istd::IChangeable)

int CSamplingChannelsInfo::GetSupportedOperations() const
{
	return SO_COPY | SO_CLONE;
}


bool CSamplingChannelsInfo::CopyFrom(const istd::IChangeable& object)
{
	const CSamplingChannelsInfo* infoPtr = dynamic_cast<const CSamplingChannelsInfo*>(&object);
	if (infoPtr != NULL){
		m_channelsCount = infoPtr->m_channelsCount;
		m_samplingPeriod = infoPtr->m_samplingPeriod;

		return true;
	}

	return false;
}


istd::IChangeable* CSamplingChannelsInfo::CloneMe() const
{
	return new CSamplingChannelsInfo(m_channelsCount, m_samplingPeriod);
}


} // namespace imeas


