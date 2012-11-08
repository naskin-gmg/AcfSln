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
		I_ASSIGN(m_startAngleParamIdAttrPtr, "StartAngleParameterId", "ID of the start angle parameter in the parameter set", false, "StartAngleParameterId");
		I_ASSIGN(m_endAngleParamIdAttrPtr, "EndAngleParameterId", "ID of the end angle parameter in the parameter set", false, "EndAngleParameterId");
	I_END_COMPONENT;

protected:
	// reimplemented (CImageRegionProcessorCompBase)
	virtual bool ProcessImageRegion(
				const iimg::IBitmap& inputBitmap,
				const iprm::IParamsSet* paramsPtr,
				const i2d::IObject2d* aoiPtr,
				istd::IChangeable* outputPtr) const;

private:
	I_ATTR(QByteArray, m_startAngleParamIdAttrPtr);
	I_ATTR(QByteArray, m_endAngleParamIdAttrPtr);
};


} // namespace iipr


#endif // !iipr_CImagePolarTransformProcessorComp_included

