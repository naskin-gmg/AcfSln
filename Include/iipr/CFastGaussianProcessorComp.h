// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#ifndef iipr_CFastGaussianProcessorComp_included
#define iipr_CFastGaussianProcessorComp_included


#include <iipr/CImageRegionProcessorCompBase.h>


namespace iipr
{


/**
	Implementation of fast Gaussian filtering with static defined filter kernels.
*/
class CFastGaussianProcessorComp: public iipr::CImageRegionProcessorCompBase
{
public:
	typedef iipr::CImageRegionProcessorCompBase BaseClass;

	enum KernelTypes
	{
		KT_3x3,
		KT_3x3_OPTIMIZED
	};

	I_BEGIN_COMPONENT(CFastGaussianProcessorComp);
		I_ASSIGN(m_kernelTypeAttrPtr, "KernelType", "Type of the Gaussian kernel", true, 0);
	I_END_COMPONENT;

protected:
	// reimplemented (iipr::CImageRegionProcessorCompBase)
	virtual bool ProcessImageRegion(
				const iimg::IBitmap& inputBitmap,
				const iprm::IParamsSet* paramsPtr,
				const i2d::IObject2d* aoiPtr,
				istd::IChangeable* outputPtr) const;

private:
	I_ATTR(int, m_kernelTypeAttrPtr);
};


} // namespace iipr


#endif // !iipr_CFastGaussianProcessorComp_included

