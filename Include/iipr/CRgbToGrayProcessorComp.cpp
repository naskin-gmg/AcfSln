#include "iipr/CRgbToGrayProcessorComp.h"


 // ACF includes
#include "istd/TChangeNotifier.h"

#include "ibase/CSize.h"


// IACF includes
#include "iipr/TImagePixelInterpolator.h"


namespace iipr
{


// reimplemented (iproc::IProcessor)

int CRgbToGrayProcessorComp::DoProcessing(
				const iprm::IParamsSet* /*paramsPtr*/,
				const istd::IPolymorphic* inputPtr,
				istd::IChangeable* outputPtr,
				iproc::IProgressManager* /*progressManagerPtr*/)
{
	const iimg::IBitmap* inputBitmapPtr = dynamic_cast<const iimg::IBitmap*>(inputPtr);
	if (inputBitmapPtr == NULL){
		return TS_INVALID;
	}

	iimg::IBitmap* outputBitmapPtr = dynamic_cast<iimg::IBitmap*>(outputPtr);
	if (outputBitmapPtr == NULL){
		return TS_INVALID;
	}

	return ConvertImage(*inputBitmapPtr, *outputBitmapPtr) ? TS_OK : TS_INVALID;
}


// private methods

bool CRgbToGrayProcessorComp::ConvertImage(
			const iimg::IBitmap& inputBitmap,
			iimg::IBitmap& outputBitmap) const
{
	if (inputBitmap.IsEmpty()){
		return true;
	}

	int inputFormat = inputBitmap.GetPixelFormat();
	if (inputFormat == iimg::IBitmap::PF_GRAY){
		return outputBitmap.CopyFrom(inputBitmap);
	}
	else if (inputFormat != iimg::IBitmap::PF_RGB && inputFormat != iimg::IBitmap::PF_RGBA){
		SendErrorMessage(0, tr("Image format must be RGB(A)"), "RgbToGrayProcessor");

		return false;
	}

	ibase::CSize imageSize = inputBitmap.GetImageSize();

	if (!outputBitmap.CreateBitmap(iimg::IBitmap::PF_GRAY, imageSize)){
		return false;
	}

	int inputPixelComponentCount = inputBitmap.GetComponentsCount();

	for (int y = 0; y < imageSize.GetY(); ++y){
		I_BYTE* inputImageLinePtr = (I_BYTE*)inputBitmap.GetLinePtr(y);
		I_BYTE* outputImageLinePtr = (I_BYTE*)outputBitmap.GetLinePtr(y);

		for (int x = 0; x < imageSize.GetX(); ++x){
			I_BYTE* pixelPtr = inputImageLinePtr + x * inputPixelComponentCount;

			I_BYTE r = pixelPtr[0];
			I_BYTE g = pixelPtr[1];
			I_BYTE b = pixelPtr[2];

			if (inputFormat == iimg::IBitmap::PF_RGBA){
				double alpha = pixelPtr[3] / 255.0;

				r = I_BYTE(r * alpha);
				g = I_BYTE(g * alpha);
				b = I_BYTE(b * alpha);
			}

			outputImageLinePtr[x] = (77 * r + 151 * g + 28 * b) >> 8;

		}
	}

	return true;
}


} // namespace iipr


