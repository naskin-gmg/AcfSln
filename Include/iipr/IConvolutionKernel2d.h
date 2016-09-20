#ifndef iipr_IConvolutionKernel2d_included
#define iipr_IConvolutionKernel2d_included


// ACF includes
#include <istd/istd.h>
#include <istd/CIndex2d.h>
#include <iser/ISerializable.h>


namespace iipr
{


/**
	Interface for 2D-convolution filter kernel.
*/
class IConvolutionKernel2d: virtual public iser::ISerializable
{
public:
	enum SeparationType
	{
		ST_AUTO,
		/**
			Separation to horizontal and vertical kernels.
		*/
		ST_HOR_VERT
	};

	/**
		Get size of this kernel.
	*/
	virtual istd::CIndex2d GetKernelSize() const = 0;

	/**
		Set size of this kernel.
		It initialize all values to defined value.
	*/
	virtual void SetKernelSize(const istd::CIndex2d& kernelSize, double value = 0.0) = 0;

	/**
		Get kernel element value at the given position \c index
	*/
	virtual double GetKernelElement(const istd::CIndex2d& index) const = 0;

	/**
		Set kernel element value at the given position \c index.
		The method returns \c true, if the value could be set and \c false otherwise.
	*/
	virtual bool SetKernelElement(const istd::CIndex2d& index, double value) = 0;

	/**
		Get offset value will be added to convolution result.
	*/
	virtual double GetOffsetValue() const = 0;
	/**
		Set offset value will be added to convolution result.
	*/
	virtual bool SetOffsetValue(double offset) = 0;

	/**
		Get some kernel beeing combination of this and some other kernel.
	*/
	virtual bool GetCombinedKernel(const IConvolutionKernel2d& kernel, IConvolutionKernel2d& result) const = 0;
	/**
		Try separate this kernel into .
	*/
	virtual bool TrySeparateKernels(IConvolutionKernel2d& result1, IConvolutionKernel2d& result2, int separationType = ST_AUTO, double tolerance = I_BIG_EPSILON) const = 0;
};


} // namespace iipr


#endif // !iipr_IConvolutionKernel2d_included


