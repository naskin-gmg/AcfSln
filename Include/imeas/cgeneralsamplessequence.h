#ifndef imeas_CGeneralSamplesSequence_included
#define imeas_CGeneralSamplesSequence_included


#include "imeas/ISamplesSequence.h"


namespace imeas
{


class CGeneralSamplesSequence: virtual public ISamplesSequence
{
public:
	CGeneralSamplesSequence();

	// reimplemented (imeas::ISamplesSequence)
	virtual bool CreateSequence(int samplesCount, int channelsCount = 1);
	virtual bool IsEmpty() const;
	virtual void ResetSequence();
	virtual int GetSamplesCount() const;
	virtual int GetChannelsCount() const;
	virtual double GetSamplingPeriod() const;
	virtual void SetSamplingPeriod(double value);
	virtual double GetSample(int index, int channel = 0) const;
	virtual void SetSample(int index, int channel, double value);
	virtual bool CopySequenceFrom(const ISamplesSequence& sequence);

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);

private:
	typedef std::vector<double> Samples;
	Samples m_samples;

	int m_channelsCount;

	double m_samplingPeriod;
};


} // namespace imeas


#endif // !imeas_CGeneralSamplesSequence_included


