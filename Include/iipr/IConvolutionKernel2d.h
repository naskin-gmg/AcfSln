#ifndef iipr_IConvolutionKernel2d_included
#define iipr_IConvolutionKernel2d_included


// ACF includes
#include "istd/CIndex2d.h"
#include "iser/ISerializable.h"


namespace iipr
{


/**
	Interface for 2D-convolution filter kernel.
*/
class IConvolutionKernel2d: virtual public iser::ISerializable
{
public:
	/**
		Get size of this kernel.
	*/
	virtual istd::CIndex2d GetKernelSize() const = 0;

	/**
		Set size of this kernel.
	*/
	virtual void SetKernelSize(const istd::CIndex2d& kernelSize) = 0;

	/**
		Get kernel element value at the given position \c index
	*/
	virtual double GetKernelElement(const istd::CIndex2d& index) const = 0;

	/**
		Set kernel element value at the given position \c index.
		The method returns \c true, if the value could be set and \c false otherwise.
	*/
	virtual bool SetKernelElement(const istd::CIndex2d& index, double value) = 0;
};


} // namespace iipr


#endif // !iipr_IConvolutionKernel2d_included


