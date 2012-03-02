#include "iipr/CImageBinarizeProcessorComp.h"


 // ACF includes
#include "istd/TChangeNotifier.h"

#include "ibase/CSize.h"


namespace iipr
{


// reimplemented (iproc::IProcessor)

int CImageBinarizeProcessorComp::DoProcessing(
				const iprm::IParamsSet* paramsPtr,
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

	const imeas::INumericParams* thresholdParamsPtr = NULL;
	if (paramsPtr != NULL && m_binarizationParamsIdAttrPtr.IsValid()){
		thresholdParamsPtr = dynamic_cast<const imeas::INumericParams*>(paramsPtr->GetParameter(*m_binarizationParamsIdAttrPtr));
	}

	if (thresholdParamsPtr == NULL){
		SendErrorMessage(0, "Threshold parameter is not set");

		return TS_INVALID;
	}

	return ConvertImage(*inputBitmapPtr, *thresholdParamsPtr, *outputBitmapPtr) ? TS_OK : TS_INVALID;
}


// private methods

bool CImageBinarizeProcessorComp::ConvertImage(
			const iimg::IBitmap& inputBitmap,
			const imeas::INumericParams& thresholdParams,
			iimg::IBitmap& outputBitmap) const
{
	if (inputBitmap.IsEmpty()){
		SendWarningMessage(0, "Input bitmap is empty.");

		return false;
	}

	if (!outputBitmap.CreateBitmap(inputBitmap.GetPixelFormat(), inputBitmap.GetImageSize())){
		return false;
	}

	int imageWidth = inputBitmap.GetImageSize().GetX();
	int imageHeight = inputBitmap.GetImageSize().GetY();

	imath::CVarVector values = thresholdParams.GetValues();
	if (values.GetElementsCount() < 1){
		SendErrorMessage(0, "Wrong processing parameters");

		return false;
	}

	double threshold = values[0];

	for(int y = 0; y < imageHeight; ++y){
		I_BYTE* outputImageBufferPtr = (I_BYTE*)outputBitmap.GetLinePtr(y);
		I_BYTE* inputImageBufferPtr = (I_BYTE*)inputBitmap.GetLinePtr(y);

		for (int x = 0; x < imageWidth; ++x){
			*outputImageBufferPtr = *inputImageBufferPtr > threshold ? 255 : 0;

			++outputImageBufferPtr, ++inputImageBufferPtr;
		}
	}

	return true;
}


} // namespace iipr


