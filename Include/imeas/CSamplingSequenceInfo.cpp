#include "imeas/CSamplingSequenceInfo.h"


#include "istd/TDelPtr.h"


namespace imeas
{


CSamplingSequenceInfo::CSamplingSequenceInfo(int channelsCount, double samplingPeriod)
:	m_channelsCount(channelsCount), m_samplingPeriod(samplingPeriod)
{
}


double CSamplingSequenceInfo::GetSamplingPeriod() const
{
	return m_samplingPeriod;
}


// reimplemented (imeas::IDataSequenceInfo)

int CSamplingSequenceInfo::GetSamplesCount() const
{
	return -1;
}


int CSamplingSequenceInfo::GetChannelsCount() const
{
	return m_channelsCount;
}


istd::CRange CSamplingSequenceInfo::GetValueRange(int /*channelIndex*/) const
{
	return istd::CRange::GetInvalid();
}


bool CSamplingSequenceInfo::HasChannelType(int channelType) const
{
	return (channelType == CT_NORMAL);
}


int CSamplingSequenceInfo::GetChannelType(int /*channelIndex*/) const
{
	return CT_NORMAL;
}


// reimplemented (istd::IChangeable)

int CSamplingSequenceInfo::GetSupportedOperations() const
{
	return SO_COPY | SO_CLONE;
}


bool CSamplingSequenceInfo::CopyFrom(const istd::IChangeable& object)
{
	const CSamplingSequenceInfo* infoPtr = dynamic_cast<const CSamplingSequenceInfo*>(&object);
	if (infoPtr != NULL){
		m_channelsCount = infoPtr->m_channelsCount;
		m_samplingPeriod = infoPtr->m_samplingPeriod;

		return true;
	}

	return false;
}


istd::IChangeable* CSamplingSequenceInfo::CloneMe() const
{
	return new CSamplingSequenceInfo(m_channelsCount, m_samplingPeriod);
}


} // namespace imeas


