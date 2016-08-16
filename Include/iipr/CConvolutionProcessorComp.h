#ifndef iipr_CConvolutionProcessorComp_included
#define iipr_CConvolutionProcessorComp_included


// ACF-Solutions includes
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
		I_ASSIGN(m_defaultAoiCompPtr, "DefaultAoi", "Area of interest used if not specified in parameters", false, "DefaultAoi");
		I_ASSIGN(m_aoiParamIdAttrPtr, "AoiParamId", "ID of area of interest in parameter set", false, "AoiParams");
		I_ASSIGN(m_aoiModeAttrPtr, "AoiMode", "Mode of interpretation of AOI:\n\t0 - AOI contains input pixels,\n\t1 - AOI contains output pixels", true, 0);
	I_END_COMPONENT;

protected:
	// reimplemented (iipr::TImageParamProcessorCompBase)
	virtual bool ParamProcessImage(
				const iprm::IParamsSet* paramsPtr,
				const IConvolutionKernel2d* procParamPtr,
				iimg::IBitmap::PixelFormat outputPixelFormat,
				const iimg::IBitmap& inputImage,
				iimg::IBitmap& outputImage);

private:
	I_ATTR(bool, m_normalizeKernelAttrPtr);
	I_REF(i2d::IObject2d, m_defaultAoiCompPtr);
	I_ATTR(QByteArray, m_aoiParamIdAttrPtr);
	I_ATTR(int, m_aoiModeAttrPtr);
};


} // namespace iipr


#endif // !iipr_CConvolutionProcessorComp_included

