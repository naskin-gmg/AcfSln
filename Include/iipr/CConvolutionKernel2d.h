#ifndef iipr_CConvolutionKernel2d_included
#define iipr_CConvolutionKernel2d_included


// ACF includes
#include "imath/CSampledFunction2d.h"

// IACF includes
#include "iipr/IConvolutionKernel2d.h"


namespace iipr
{


/**
	Implementation of a general 2D convolution kernel. 
	Internally the kernel is represented by an array of filter weights and offset positions
	for fast access to the corresponding bitmap values.

	For access and iterating through the kernel values you can use the Begin() and End() function, 
	which returns an iterator/accessor.
*/
class CConvolutionKernel2d:
			virtual public imath::CSampledFunction2d,
			virtual public IConvolutionKernel2d
{
public:
	typedef imath::CSampledFunction2d BaseClass;

	CConvolutionKernel2d();
	CConvolutionKernel2d(const imath::ISampledFunction2d& function2d);

	// reimplemented (iipr::IConvolutionKernel2d)
	virtual istd::CIndex2d GetKernelSize() const;
	virtual double GetKernelElement(const istd::CIndex2d& index) const;
	virtual bool SetKernelElement(const istd::CIndex2d& index, double value);

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive) = 0;
	virtual I_DWORD GetMinimalVersion(int versionId) const;
};


} // namespace iipr


#endif // !iipr_CConvolutionKernel2d_included

