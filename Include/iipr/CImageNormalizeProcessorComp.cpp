#include "iipr/CImageNormalizeProcessorComp.h"


// ACF includes
#include "imath/CFixedPointManip.h"
#include "iimg/CGeneralBitmap.h"


namespace iipr
{


// reimplemented (iipr::CImageProcessorCompBase)

bool CImageNormalizeProcessorComp::ProcessImage(
			const iprm::IParamsSet* paramsPtr,
			const iimg::IBitmap& inputImage,
			iimg::IBitmap& outputImage)
{
	if (paramsPtr == NULL){
		return outputImage.CopyFrom(inputImage);
	}

	istd::CIndex2d imageSize = inputImage.GetImageSize();
	int imageWidth = imageSize.GetX();
	int imageHeight = imageSize.GetY();

	int outputImageLeft = 0;
	int outputImageRight = imageWidth - 1;
	int outputImageTop = 0;
	int outputImageBottom = imageHeight - 1;

	const i2d::CRectangle* aoiPtr = m_defaultAoiParamCompPtr.GetPtr();
	if (m_aoiParamIdAttrPtr.IsValid()){
		aoiPtr = dynamic_cast<const i2d::CRectangle*>(paramsPtr->GetParameter((*m_aoiParamIdAttrPtr).ToString()));
	}

	if (aoiPtr != NULL){
		outputImageLeft = istd::Max(outputImageLeft, int(aoiPtr->GetLeft()));
		outputImageRight = istd::Min(outputImageRight, int(::ceil(aoiPtr->GetRight())));
		outputImageTop = istd::Max(outputImageTop, int(aoiPtr->GetLeft()));
		outputImageBottom = istd::Min(outputImageBottom, int(::ceil(aoiPtr->GetRight())));
	}

	if ((outputImageLeft > outputImageRight) || (outputImageTop > outputImageBottom)){	// if output image should be empty
		outputImage.ResetImage();

		return true;
	}

	const iprm::ILinearAdjustParams* adjustParamsPtr = m_defaultAdjustParamsCompPtr.GetPtr();
	if (m_adjustParamsIdAttrPtr.IsValid()){
		adjustParamsPtr = dynamic_cast<const iprm::ILinearAdjustParams*>(paramsPtr->GetParameter((*m_adjustParamsIdAttrPtr).ToString()));
	}

	double contrast = 1;
	double brightness = 0;
	if (adjustParamsPtr != NULL){
		contrast = adjustParamsPtr->GetScaleFactor();
		brightness = adjustParamsPtr->GetOffsetFactor() * 255;
	}

	if (!outputImage.CreateBitmap(
				inputImage.GetPixelFormat(),
				istd::CIndex2d(outputImageRight - outputImageLeft + 1, outputImageBottom - outputImageTop + 1))){
		SendWarningMessage(0, "Could not create output image");

		return false;
	}

	if ((contrast != 1) || (brightness != 0)){
		for (int lineIndex = outputImageTop; lineIndex <= outputImageBottom; ++lineIndex){
			const I_BYTE* inputLinePtr = static_cast<const I_BYTE*>(inputImage.GetLinePtr(lineIndex));
			I_BYTE* outputLinePtr = static_cast<I_BYTE*>(outputImage.GetLinePtr(lineIndex - outputImageTop));

			for (int x = outputImageLeft; x <= outputImageRight; ++x){
				int outputValue = int(inputLinePtr[x] * contrast + brightness);
				if (outputValue < 0){
					outputValue = 0;
				}
				else if (outputValue > 255){
					outputValue = 255;
				}
				outputLinePtr[x - outputImageLeft] = I_BYTE(outputValue);
			}
		}
	}
	else{
		for (int lineIndex = outputImageTop; lineIndex <= outputImageBottom; ++lineIndex){
			const I_BYTE* inputLinePtr = static_cast<const I_BYTE*>(inputImage.GetLinePtr(lineIndex));
			I_BYTE* outputLinePtr = static_cast<I_BYTE*>(outputImage.GetLinePtr(lineIndex - outputImageTop));

			for (int x = outputImageLeft; x <= outputImageRight; ++x){
				outputLinePtr[x - outputImageLeft] = inputLinePtr[x];
			}
		}
	}

	return true;
}


} // namespace iipr

