#ifndef iipr_CConvolutionProcessorComp_included
#define iipr_CConvolutionProcessorComp_included


#include "iipr/TImageParamProcessorCompBase.h"

#include "iipr/IConvolutionKernel2d.h"


namespace iipr
{


/**
	Implementation of image processor using convolution kernel.
*/
class CConvolutionProcessorComp: public iipr::TImageParamProcessorCompBase<IConvolutionKernel2d>
{
public:
	typedef iipr::TImageParamProcessorCompBase<IConvolutionKernel2d> BaseClass;

protected:
	// reimplemented (iipr::TImageParamProcessorCompBase)
	virtual bool ParamProcessImage(
				const IConvolutionKernel2d* paramsPtr,
				const iimg::IBitmap& inputImage,
				iimg::IBitmap& outputImage);
};


} // namespace iipr


#endif // !iipr_CConvolutionProcessorComp_included

