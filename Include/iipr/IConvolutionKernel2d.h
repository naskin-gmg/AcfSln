#ifndef iipr_IConvolutionKernel2d_included
#define iipr_IConvolutionKernel2d_included


// ACF includes
#include "iser/ISerializable.h"


namespace iipr
{


class IConvolutionKernel2d: virtual public iser::ISerializable
{
public:
	/**
		Get size of this kernel.
	*/
	virtual istd::CIndex2d GetKernelSize() const = 0;
	virtual double GetKernelElement(const istd::CIndex2d& index) const = 0;
	virtual bool SetKernelElement(const istd::CIndex2d& index, double value) = 0;
};


} // namespace iipr


#endif // !iipr_IConvolutionKernel2d_included


