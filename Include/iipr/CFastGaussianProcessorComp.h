#ifndef iipr_CFastGaussianProcessorComp_included
#define iipr_CFastGaussianProcessorComp_included


#include "iipr/CImageProcessorCompBase.h"


namespace iipr
{


/**
	Implementation of fast Gaussian filtering with static defined filter kernels.
*/
class CFastGaussianProcessorComp: public iipr::CImageProcessorCompBase
{
public:
	typedef iipr::CImageProcessorCompBase BaseClass;

	enum KernelTypes
	{
		KT_3x3,
		KT_3x3_OPTIMIZED
	};

	I_BEGIN_COMPONENT(CFastGaussianProcessorComp);
		I_ASSIGN(m_kernelTypeAttrPtr, "KernelType", "Type of the Gaussian kernel", true, 0);
	I_END_COMPONENT;

protected:
	// reimplemented (iipr::CImageProcessorCompBase)
	virtual bool ProcessImage(
		const iprm::IParamsSet* paramsPtr, 
		const iimg::IBitmap& inputImage,
		iimg::IBitmap& outputImage);

private:
	I_ATTR(int, m_kernelTypeAttrPtr);
};


} // namespace iipr


#endif // !iipr_CFastGaussianProcessorComp_included

