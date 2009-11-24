#ifndef imeas_CGeneralSamplesSequence_included
#define imeas_CGeneralSamplesSequence_included


// ACF includes
#include "imath/TISampledFunction.h"

#include "imeas/ISamplesSequence.h"


namespace imeas
{


/**
	General implementation of sample sequence interface ISamplesSequence.
*/
class CGeneralSamplesSequence:
			virtual public ISamplesSequence,
			virtual public imath::ISampledFunction2d
{
public:
	CGeneralSamplesSequence();

	// reimplemented (imeas::ISamplesSequence)
	virtual bool CreateSequence(int samplesCount, int channelsCount = 1);
	virtual bool IsEmpty() const;
	virtual void ResetSequence();
	virtual int GetTimeSamplesCount() const;
	virtual int GetChannelsCount() const;
	virtual double GetSamplingPeriod() const;
	virtual void SetSamplingPeriod(double value);
	virtual double GetSample(int index, int channel = 0) const;
	virtual void SetSample(int index, int channel, double value);
	virtual bool CopySequenceFrom(const ISamplesSequence& sequence);

	// reimplemented (imath::ISampledFunction2d)
	virtual bool CreateFunction(double* dataPtr, const ArgumentType& sizes);
	virtual int GetSamplesCount() const;
	virtual int GetGridSize(int dimensionIndex) const;
	virtual istd::CRange GetLogicalRange(int dimensionIndex) const;
	virtual istd::CRange GetResultValueRange(int dimensionIndex, int resultDimension = -1) const;

	// reimplemented (imath::TIMathFunction)
	virtual bool GetValueAt(const ArgumentType& argument, ResultType& result) const;
	virtual ResultType GetValueAt(const ArgumentType& argument) const;

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


