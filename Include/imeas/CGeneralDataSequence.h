#ifndef imeas_CGeneralDataSequence_included
#define imeas_CGeneralDataSequence_included


// Qt includes
#include <QtCore/QVector>

// ACF includes
#include <istd/TSmartPtr.h>
#include <istd/TOptDelPtr.h>
#include <imath/TISampledFunction.h>
#include <imeas/IDataSequence.h>


namespace imeas
{


/**
	General implementation of sample sequence interface IDataSequence.
*/
class CGeneralDataSequence:
			virtual public IDataSequence,
			virtual public imath::ISampledFunction2d
{
public:
	CGeneralDataSequence();

	const double* GetSamplesBuffer() const;
	double* GetSamplesBuffer();
	int GetSamplesBufferSize() const;

	// reimplemented (imeas::IDataSequence)
	virtual bool CreateSequence(int samplesCount, int channelsCount = 1) override;
	virtual bool CreateSequenceWithInfo(
				const istd::TTransPtr<const IDataSequenceInfo>& infoPtr,
				int samplesCount = -1,
				int channelsCount = -1) override;
	virtual const IDataSequenceInfo* GetSequenceInfo() const override;
	virtual bool IsEmpty() const override;
	virtual void ResetSequence() override;
	virtual int GetSamplesCount() const override;
	virtual int GetChannelsCount() const override;
	virtual double GetSample(int index, int channel = 0) const override;
	virtual void SetSample(int index, int channel, double value) override;

	// reimplemented (imath::ISampledFunction2d)
	virtual bool CreateFunction(double* dataPtr, const ArgumentType& sizes) override;
	virtual int GetTotalSamplesCount() const override;
	virtual int GetGridSize(int dimensionIndex) const override;
	virtual double GetSampleAt(const SampleIndex& index) const override;
	virtual istd::CRange GetLogicalRange(int dimensionIndex) const override;
	virtual istd::CRange GetResultValueRange(int dimensionIndex, int resultDimension = -1) const override;

	// reimplemented (imath::TIMathFunction)
	virtual bool GetValueAt(const ArgumentType& argument, ResultType& result) const override;
	virtual ResultType GetValueAt(const ArgumentType& argument) const override;

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive) override;

	// reimplemented (istd::IChangeable)
	virtual int GetSupportedOperations() const override;
	virtual bool CopyFrom(const istd::IChangeable& object, CompatibilityMode mode = CM_WITHOUT_REFS) override;
	virtual istd::IChangeableUniquePtr CloneMe(CompatibilityMode mode = CM_WITHOUT_REFS) const override;

private:
	typedef std::vector<double> Samples;
	Samples m_samples;

	int m_channelsCount;

	istd::TSmartPtr<const IDataSequenceInfo> m_sequenceInfoPtr;
};


// inline methods

inline const double* CGeneralDataSequence::GetSamplesBuffer() const
{
	return &m_samples[0];
}


inline double* CGeneralDataSequence::GetSamplesBuffer()
{
	return &m_samples[0];
}


inline int CGeneralDataSequence::GetSamplesBufferSize() const
{
	return int(m_samples.size());
}


} // namespace imeas


#endif // !imeas_CGeneralDataSequence_included


