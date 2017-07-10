#ifndef iipr_CImageIntegralProcessorComp_included
#define iipr_CImageIntegralProcessorComp_included


// ACF includes
#include <istd/IPolymorphic.h>
#include <iprm/IParamsSet.h>
#include <iimg/IBitmap.h>
#include <iimg/CScanlineMask.h>
#include <iipr/CImageProcessorCompBase.h>


namespace iipr
{


/**
	Helper class allowing to find perspective calibration using list of nominal and found positions.
*/
class CImageIntegralProcessorComp: public CImageProcessorCompBase
{
public:
	/**
		Calculates integral image.
		\param	inputImage			input image.
		\param	outputImage			result image, will be initializated.
		\param	maskPtr				optional mask. Points out of mask will be not touched inside of input image and not set in output image.
		\param	outputPixelFormat	requested format of output image or \c iimg::IBitmap::PF_UNKNOWN if it should be automatically calculated.
	*/
	virtual bool CalculateIntegralImage(
				const iimg::IBitmap& inputImage,
				iimg::IBitmap& outputImage,
				const iimg::CScanlineMask* maskPtr = NULL,
				iimg::IBitmap::PixelFormat outputPixelFormat = iimg::IBitmap::PF_UNKNOWN,
				bool fillOutOfMask = true);

	/**
		Calculates integral image in place.
		\param	image	image will be converted to its integral.
	*/
	virtual bool MakeIntegralImage(iimg::IBitmap& image);

protected:
	// reimplemented (iipr::CImageProcessorCompBase)
	virtual bool ProcessImage(
				const iprm::IParamsSet* paramsPtr, 
				const iimg::IBitmap& inputImage,
				iimg::IBitmap& outputImage);
};


} // namespace iipr


#endif // !iipr_CImageIntegralProcessorComp_included


