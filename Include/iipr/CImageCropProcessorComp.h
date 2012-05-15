#ifndef iipr_CImageCropProcessorComp_included
#define iipr_CImageCropProcessorComp_included


// ACF-Solutions includes
#include "imeas/IDiscreteDataSequence.h"
#include "iipr/CImageRegionProcessorCompBase.h"


namespace iipr
{


/**	
	Processor for image croping.
*/
class CImageCropProcessorComp: public CImageRegionProcessorCompBase
{
public:
	typedef iproc::CSyncProcessorCompBase BaseClass;
	
	I_BEGIN_COMPONENT(CImageCropProcessorComp);
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


#endif // !iipr_CImageCropProcessorComp_included

