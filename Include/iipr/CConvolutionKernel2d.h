#ifndef iipr_CConvolutionKernel2d_included
#define iipr_CConvolutionKernel2d_included


// ACF includes
#include <imath/CSampledFunction2d.h>

// IACF includes
#include <iipr/IConvolutionKernel2d.h>


namespace iipr
{


/**
	Implementation of a general 2D convolution kernel. 
	Internally the kernel is represented by an array of filter weights and offset positions
	for fast access to the corresponding bitmap values.
*/
class CConvolutionKernel2d:
			virtual public imath::CSampledFunction2d,
			virtual public IConvolutionKernel2d
{
public:
	typedef imath::CSampledFunction2d BaseClass;

	CConvolutionKernel2d();
	CConvolutionKernel2d(const CConvolutionKernel2d& kernel);
	explicit CConvolutionKernel2d(const imath::ISampledFunction2d& function2d);
	explicit CConvolutionKernel2d(const istd::CIndex2d& kernelSize, double value = 0);

	double GetKernelDistance(const IConvolutionKernel2d& kernel) const;

	// reimplemented (iipr::IConvolutionKernel2d)
	virtual istd::CIndex2d GetKernelSize() const;
	virtual void SetKernelSize(const istd::CIndex2d& kernelSize, double value = 0.0);
	virtual double GetKernelElement(const istd::CIndex2d& index) const;
	virtual bool SetKernelElement(const istd::CIndex2d& index, double value);
	virtual double GetOffsetValue() const;
	virtual bool SetOffsetValue(double offset);
	virtual bool GetCombinedKernel(const IConvolutionKernel2d& kernel, IConvolutionKernel2d& result) const;
	virtual bool TrySeparateKernels(IConvolutionKernel2d& result1, IConvolutionKernel2d& result2, int separationType = ST_AUTO, double tolerance = I_BIG_EPSILON) const;

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);
	virtual quint32 GetMinimalVersion(int versionId) const;

private:
	double m_valueOffset;
};


} // namespace iipr


#endif // !iipr_CConvolutionKernel2d_included

