#ifndef iipr_CImagePolarTransformProcessorComp_included
#define iipr_CImagePolarTransformProcessorComp_included


// ACF-Solutions includes
#include "iipr/CImageRegionProcessorCompBase.h"


namespace iipr
{


/**	
	Implementation of a processor for the image histogram calculation.
*/
class CImagePolarTransformProcessorComp: public CImageRegionProcessorCompBase
{
public:
	typedef CImageRegionProcessorCompBase BaseClass;
	
	I_BEGIN_COMPONENT(CImagePolarTransformProcessorComp);
	I_END_COMPONENT;

protected:
	// reimplemented (CImageRegionProcessorCompBase)
	virtual bool ProcessImageRegion(
				const iimg::IBitmap& input,
				const iprm::IParamsSet* paramsPtr,
				const i2d::IObject2d* aoiPtr,
				istd::IChangeable* outputPtr) const;
};


} // namespace iipr


#endif // !iipr_CImagePolarTransformProcessorComp_included

