#ifndef iipr_CImagePolarTransformProcessorComp_included
#define iipr_CImagePolarTransformProcessorComp_included


// ACF-Solutions includes
#include "iipr/CImageRegionProcessorCompBase.h"


namespace iipr
{


/**	
	Implementation of a processor for the polar tranformation of an image region.
*/
class CImagePolarTransformProcessorComp: public CImageRegionProcessorCompBase
{
public:
	typedef CImageRegionProcessorCompBase BaseClass;
	
	I_BEGIN_COMPONENT(CImagePolarTransformProcessorComp);
		I_ASSIGN(m_angleResolutionParamIdAttrPtr, "AngleResolutionParamId", "ID of the angle resolution parameter in the parameter set", false, "AngleResolutionParamId");
		I_ASSIGN(m_interpolationParamIdAttrPtr, "InterpolationParamId", "ID of the interpolation parameter in the parameter set", false, "InterpolationParamId");
	I_END_COMPONENT;

protected:
	// reimplemented (CImageRegionProcessorCompBase)
	virtual bool ProcessImageRegion(
				const iimg::IBitmap& inputBitmap,
				const iprm::IParamsSet* paramsPtr,
				const i2d::IObject2d* aoiPtr,
				istd::IChangeable* outputPtr) const;

private:
	I_ATTR(QByteArray, m_angleResolutionParamIdAttrPtr);
	I_ATTR(QByteArray, m_interpolationParamIdAttrPtr);
};


} // namespace iipr


#endif // !iipr_CImagePolarTransformProcessorComp_included

