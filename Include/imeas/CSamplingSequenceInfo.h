#ifndef imeas_CSamplingSequenceInfo_included
#define imeas_CSamplingSequenceInfo_included


#include "imeas/IDataSequenceInfo.h"


namespace imeas
{


class CSamplingSequenceInfo: virtual public IDataSequenceInfo
{
public:
	CSamplingSequenceInfo(int channelsCount, double samplingPeriod);

	/**
		Get sampling period of this sequence.
		Sampling period is time difference between samples.
		If returned value is not positive, no period is known.
	*/
	virtual double GetSamplingPeriod() const;

	// reimplemented (imeas::IDataSequenceInfo)
	virtual int GetSamplesCount() const;
	virtual int GetChannelsCount() const;
	virtual istd::CRange GetValueRange(int channelIndex = -1) const;
	virtual bool HasChannelType(int channelType) const;
	virtual int GetChannelType(int channelIndex) const;

	// reimplemented (istd::IChangeable)
	virtual int GetSupportedOperations() const;
	virtual bool CopyFrom(const istd::IChangeable& object);
	virtual istd::IChangeable* CloneMe() const;

private:
	int m_channelsCount;
	double m_samplingPeriod;
};


} // namespace imeas


#endif // !imeas_CSamplingSequenceInfo_included


