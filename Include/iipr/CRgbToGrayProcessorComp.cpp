// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#include <iipr/CRgbToGrayProcessorComp.h>


 // ACF includes
#include <istd/CChangeNotifier.h>
#include <ibase/CSize.h>
#include <iprm/TParamsPtr.h>
#include <imeas/INumericValue.h>


namespace iipr
{


// protected methods

// reimplemented (iipr::CImageProcessorCompBase)

bool iipr::CRgbToGrayProcessorComp::ProcessImage(
			const iprm::IParamsSet* paramsPtr, 
			const iimg::IBitmap& inputImage,
			iimg::IBitmap& outputImage) const
{
	if (inputImage.IsEmpty()){
		return true;
	}

	int inputFormat = inputImage.GetPixelFormat();
	ibase::CSize imageSize = inputImage.GetImageSize();

	if (inputFormat == iimg::IBitmap::PF_GRAY){
		return outputImage.CopyFrom(inputImage);
	}
	if (inputFormat == iimg::IBitmap::PF_GRAY16) {
		if (!outputImage.CreateBitmap(iimg::IBitmap::PF_GRAY, imageSize)) {
			return false;
		}

		for (int y = 0; y < imageSize.GetY(); ++y) {
			uint16_t* inputImageLinePtr = (uint16_t*)inputImage.GetLinePtr(y);
			uint8_t* outputImageLinePtr = (uint8_t*)outputImage.GetLinePtr(y);

			for (int x = 0; x < imageSize.GetX(); ++x) {
				outputImageLinePtr[x] = uint8_t(inputImageLinePtr[x] * 255.0 / 65536);
			}
		}

		return true;
	}
	else if (inputFormat != iimg::IBitmap::PF_RGB && inputFormat != iimg::IBitmap::PF_RGBA && inputFormat != iimg::IBitmap::PF_RGB24){
		SendErrorMessage(0, QT_TR_NOOP("Image format must be RGB(A)"), "RgbToGrayProcessor");

		return false;
	}

	istd::CChangeNotifier resultNotifier(&outputImage);
	Q_UNUSED(resultNotifier);

	if (!outputImage.CreateBitmap(iimg::IBitmap::PF_GRAY, imageSize)){
		return false;
	}

	int redWeight = 255;
	int greenWeight = 255;
	int blueWeight = 255;

	iprm::TParamsPtr<imeas::INumericValue> channelAmplifierParamsPtr(paramsPtr, *m_channelWeightsParamsIdAttrPtr);
	if (channelAmplifierParamsPtr.IsValid()){
		imath::CVarVector values = channelAmplifierParamsPtr->GetValues();

		if (values.GetElementsCount() > 0)
			redWeight = values[0] >= 0 ? values[0] * 255 : 0;

		if (values.GetElementsCount() > 1)
			greenWeight = values[1] >= 0 ? values[1] * 255 : 0;

		if (values.GetElementsCount() > 2)
			blueWeight = values[2] >= 0 ? values[2] * 255 : 0;
	}

	const double weightSum = (static_cast<double>(redWeight) + static_cast<double>(greenWeight) + static_cast<double>(blueWeight)) / 255.0;

	if (weightSum > 1.0){
		redWeight /= weightSum;
		greenWeight /= weightSum;
		blueWeight /= weightSum;
	}

	const int inputPixelComponentCount = inputImage.GetComponentsCount();

	if (inputFormat == iimg::IBitmap::PF_RGB || inputFormat == iimg::IBitmap::PF_RGBA){
		for (int y = 0; y < imageSize.GetY(); ++y){
			uint8_t* inputImageLinePtr = (uint8_t*)inputImage.GetLinePtr(y);
			uint8_t* outputImageLinePtr = (uint8_t*)outputImage.GetLinePtr(y);

			for (int x = 0; x < imageSize.GetX(); ++x){
				uint8_t* pixelPtr = inputImageLinePtr + x * inputPixelComponentCount;
				outputImageLinePtr[x] = pixelPtr[3] * (redWeight * pixelPtr[2] + greenWeight * pixelPtr[1] + blueWeight * pixelPtr[0]) >> 16;
			}
		}
	}
	else if (inputFormat == iimg::IBitmap::PF_RGB24){
		for (int y = 0; y < imageSize.GetY(); ++y){
			uint8_t* inputImageLinePtr = (uint8_t*)inputImage.GetLinePtr(y);
			uint8_t* outputImageLinePtr = (uint8_t*)outputImage.GetLinePtr(y);

			for (int x = 0; x < imageSize.GetX(); ++x){
				uint8_t* pixelPtr = inputImageLinePtr + x * inputPixelComponentCount;
				outputImageLinePtr[x] = (redWeight * pixelPtr[0] + greenWeight * pixelPtr[1] + blueWeight * pixelPtr[2]) >> 8;
			}
		}
	}
	else
		return false;

	return true;
}


} // namespace iipr


