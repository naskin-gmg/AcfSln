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

	I_BEGIN_COMPONENT(CConvolutionProcessorComp);
		I_ASSIGN(m_normalizeKernelAttrPtr, "NormalizeKernel", "If true then kernel will be automatically normalized", true, false);
	I_END_COMPONENT;

protected:
	// reimplemented (iipr::TImageParamProcessorCompBase)
	virtual bool ParamProcessImage(
				const IConvolutionKernel2d* paramsPtr,
				const iimg::IBitmap& inputImage,
				iimg::IBitmap& outputImage);

private:
	I_ATTR(bool, m_normalizeKernelAttrPtr);
};


} // namespace iipr


#endif // !iipr_CConvolutionProcessorComp_included

