#ifndef iipr_CImageCropProcessorComp_included
#define iipr_CImageCropProcessorComp_included


// ACF includes
#include "i2d/CRectangle.h"

#include "iimg/IBitmap.h"

#include "iproc/TSyncProcessorCompBase.h"


// ACF-Solutions includes
#include "imeas/IDiscrDataSequence.h"


namespace iipr
{


/**	
	Processor for image croping.
*/
class CImageCropProcessorComp: public iproc::CSyncProcessorCompBase
{
public:
	typedef iproc::CSyncProcessorCompBase BaseClass;
	
	I_BEGIN_COMPONENT(CImageCropProcessorComp);
		I_ASSIGN(m_aoiParamsIdAttrPtr, "AoiParamsId", "ID of the region of interest in the parameter set", false, "AoiParams");
	I_END_COMPONENT;

	// reimplemented (iproc::IProcessor)
	virtual int DoProcessing(
				const iprm::IParamsSet* paramsPtr,
				const istd::IPolymorphic* inputPtr,
				istd::IChangeable* outputPtr,
				iproc::IProgressManager* progressManagerPtr = NULL);

private:
	bool ConvertImage(const iimg::IBitmap& input, const i2d::IObject2d* aoiPtr, iimg::IBitmap& outputBitmap) const;

private:
	I_ATTR(istd::CString, m_aoiParamsIdAttrPtr);
};


} // namespace iipr


#endif // !iipr_CImageCropProcessorComp_included

