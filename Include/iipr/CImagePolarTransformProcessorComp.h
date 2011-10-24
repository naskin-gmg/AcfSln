#ifndef iipr_CImagePolarTransformProcessorComp_included
#define iipr_CImagePolarTransformProcessorComp_included


// ACF includes
#include "i2d/CRectangle.h"

#include "iimg/IBitmap.h"

#include "iproc/TSyncProcessorCompBase.h"


// ACF-Solutions includes
#include "imeas/IDiscrDataSequence.h"


namespace iipr
{


/**	
	Implementation of a processor for the image histogram calculation.
*/
class CImagePolarTransformProcessorComp: public iproc::CSyncProcessorCompBase
{
public:
	typedef iproc::CSyncProcessorCompBase BaseClass;
	
	I_BEGIN_COMPONENT(CImagePolarTransformProcessorComp);
		I_ASSIGN(m_aoiParamsIdAttrPtr, "AoiParamsId", "ID of the AOI rectangle in the parameter set", false, "AoiParams");
		I_ASSIGN(m_interpolationParamsIdAttrPtr, "InterpolationParamsId", "ID of the interpolation parameters in the parameter set", false, "InterpolationParams");
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
	I_ATTR(istd::CString, m_interpolationParamsIdAttrPtr);
};


} // namespace iipr


#endif // !iipr_CImagePolarTransformProcessorComp_included

