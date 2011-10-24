#ifndef iipr_CImageNormalizeProcessorComp_included
#define iipr_CImageNormalizeProcessorComp_included


// ACF includes
#include "istd/CString.h"
#include "i2d/CRectangle.h"
#include "iprm/ILinearAdjustParams.h"

#include "iipr/CImageProcessorCompBase.h"


namespace iipr
{


/**
	Implementation of image smooth operator with homogeneous rectangle kernel.
	This implementation uses moving average alghorithm.
*/
class CImageNormalizeProcessorComp: public iipr::CImageProcessorCompBase
{
public:
	typedef iipr::CImageProcessorCompBase BaseClass;

	I_BEGIN_COMPONENT(CImageNormalizeProcessorComp);
		I_ASSIGN(m_aoiParamIdAttrPtr, "AoiParamId", "ID of rectangle AOI area in parameter set, only this area will processed", false, "AoiParamId");
		I_ASSIGN(m_adjustParamsIdAttrPtr, "AdjustParamsId", "ID of parameters for contrast and brightness in parameter set", false, "AdjustParamsId");
		I_ASSIGN(m_defaultAoiParamCompPtr, "DefaultAoiParam", "Default parameter for rectangle AOI area, if no parameters are specified", false, "DefaultAoiParam");
		I_ASSIGN(m_defaultAdjustParamsCompPtr, "DefaultAdjustParams", "Default adjust parameters, if no parameters are specified", false, "DefaultAdjustParams");
	I_END_COMPONENT;

protected:
	// reimplemented (iipr::CImageProcessorCompBase)
	virtual bool ProcessImage(
				const iprm::IParamsSet* paramsPtr,
				const iimg::IBitmap& inputImage,
				iimg::IBitmap& outputImage);

private:
	I_ATTR(istd::CString, m_aoiParamIdAttrPtr);
	I_ATTR(istd::CString, m_adjustParamsIdAttrPtr);
	I_REF(i2d::CRectangle, m_defaultAoiParamCompPtr);
	I_REF(iprm::ILinearAdjustParams, m_defaultAdjustParamsCompPtr);
};


} // namespace iipr


#endif // !iipr_CImageNormalizeProcessorComp_included


