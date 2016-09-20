#ifndef iipr_CConvolutionProcessorComp_included
#define iipr_CConvolutionProcessorComp_included


// ACF includes
#include <iimg/CScanlineMask.h>

// ACF-Solutions includes
#include <iipr/TImageParamProcessorCompBase.h>
#include <iipr/IConvolutionKernel2d.h>


namespace iipr
{


/**
	Implementation of image processor using convolution kernel.
*/
class CConvolutionProcessorComp: public iipr::TImageParamProcessorCompBase<IConvolutionKernel2d>
{
public:
	typedef iipr::TImageParamProcessorCompBase<IConvolutionKernel2d> BaseClass;

	I_BEGIN_COMPONENT(CConvolutionProcessorComp);
		I_ASSIGN(m_trySeparateKernelAttrPtr, "TrySeparateKernel", "If active, detection of kernel separation will be enabled", true, true);
	I_END_COMPONENT;

	// static methods
	/**
		Calculate complete convolution with all parameters as static function.
		\param	kernel				convolution kernel for this filter.
		\param	backgroundMode		mode of background fill.
		\param	outputPixelFormat	format of output pixel, if equals \c iimg::IBitmap::PF_UNKNOWN will automatically calculated.
		\param	inputImage			input image.
		\param	resultMask			AOI mask where the result pixels should be calculated.
		\param	outputImage			output image.
		\param	loggerPtr			optional object collecting processing messages.
	*/
	static bool DoConvolutionFilter(
				const IConvolutionKernel2d& kernel,
				BackgroundFillMode backgroundMode,
				iimg::IBitmap::PixelFormat outputPixelFormat,
				const iimg::IBitmap& inputImage,
				const iimg::CScanlineMask& resultMask,
				iimg::IBitmap& outputImage,
				istd::ILogger* loggerPtr = NULL);

protected:
	// reimplemented (iipr::TImageParamProcessorCompBase)
	virtual bool ParamProcessImage(
				const iprm::IParamsSet* paramsPtr,
				const IConvolutionKernel2d* procParamPtr,
				const iimg::IBitmap& inputImage,
				iimg::IBitmap& outputImage);

private:
	I_ATTR(bool, m_trySeparateKernelAttrPtr);
};


} // namespace iipr


#endif // !iipr_CConvolutionProcessorComp_included

