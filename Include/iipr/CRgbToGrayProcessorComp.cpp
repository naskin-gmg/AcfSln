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
	if (inputFormat == iimg::IBitmap::PF_GRAY){
		return outputImage.CopyFrom(inputImage);
	}
	else if (inputFormat != iimg::IBitmap::PF_RGB && inputFormat != iimg::IBitmap::PF_RGBA && inputFormat != iimg::IBitmap::PF_RGB24){
		SendErrorMessage(0, QObject::tr("Image format must be RGB(A)"), "RgbToGrayProcessor");

		return false;
	}

	ibase::CSize imageSize = inputImage.GetImageSize();

	istd::CChangeNotifier resultNotifier(&outputImage);
	Q_UNUSED(resultNotifier);

	if (!outputImage.CreateBitmap(iimg::IBitmap::PF_GRAY, imageSize)){
		return false;
	}

	int redWeight = 77;
	int greenWeight = 151;
	int blueWeight = 28;

	iprm::TParamsPtr<imeas::INumericValue> channelAmplifierParamsPtr(paramsPtr, *m_channelWeightsParamsIdAttrPtr);
	if (channelAmplifierParamsPtr.IsValid()){
		imath::CVarVector values = channelAmplifierParamsPtr->GetValues();

		if (values.GetElementsCount() > 0){
			double weightSum = values[0];

			redWeight = values[0] * 255;

			if (values.GetElementsCount() > 1){
				greenWeight = values[1] * 255;

				weightSum += values[1];
			}

			if (values.GetElementsCount() > 2){
				blueWeight = values[2] * 255;

				weightSum += values[2];
			}

			redWeight /= weightSum;
			greenWeight /= weightSum;
			blueWeight /= weightSum;
		}
	}

	int inputPixelComponentCount = inputImage.GetComponentsCount();

	// the loops are optimized for efficient SIMD vectorization
	if (inputFormat == iimg::IBitmap::PF_RGBA){
		for (int y = 0; y < imageSize.GetY(); ++y){
			quint8* inputImageLinePtr = (quint8*)inputImage.GetLinePtr(y);
			quint8* outputImageLinePtr = (quint8*)outputImage.GetLinePtr(y);

			for (int x = 0; x < imageSize.GetX(); ++x){
				quint8* pixelPtr = inputImageLinePtr + x * inputPixelComponentCount;

				outputImageLinePtr[x] = pixelPtr[3] * (redWeight * pixelPtr[0] + greenWeight * pixelPtr[1] + blueWeight * pixelPtr[2]) >> 16;
			}
		}
	}
	else{
		for (int y = 0; y < imageSize.GetY(); ++y){
			quint8* inputImageLinePtr = (quint8*)inputImage.GetLinePtr(y);
			quint8* outputImageLinePtr = (quint8*)outputImage.GetLinePtr(y);

			for (int x = 0; x < imageSize.GetX(); ++x){
				quint8* pixelPtr = inputImageLinePtr + x * inputPixelComponentCount;

				outputImageLinePtr[x] = (redWeight * pixelPtr[0] + greenWeight * pixelPtr[1] + blueWeight * pixelPtr[2]) >> 8;
			}
		}
	}

	return true;
}


} // namespace iipr


