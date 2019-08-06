#include <iipr/CColorBalanceProcessorComp.h>


// Qt includes
#include <QtCore/QVector>

// ACF includes
#include <iprm/TParamsPtr.h>
#include <iimg/CPixelFormatList.h>
#include <iimg/CGeneralBitmap.h>

// ACF-Solutions includes
#include <iipr/CImageCopyProcessorComp.h>


namespace iipr
{


// protected methods

// reimplemented (iipr::CImageProcessorCompBase)

bool CColorBalanceProcessorComp::ProcessImage(
			const iprm::IParamsSet* paramsPtr,
			const iimg::IBitmap& inputImage,
			iimg::IBitmap& outputImage) const
{
	iprm::TParamsPtr<imeas::INumericValue> colorCoefficientsPtr(paramsPtr, *m_colorCoefficientsIdAttrPtr);
	if (!colorCoefficientsPtr.IsValid()){
		SendWarningMessage(0, QObject::tr("No input or default color coefficients"), "ColorBalanceProcessor");
		outputImage.CopyFrom(inputImage);

		return true;
	}

	int inputFormat = inputImage.GetPixelFormat();
	if (inputFormat == iimg::IBitmap::PF_GRAY){
		SendWarningMessage(0, QObject::tr("No color balancing for grayscale input image possible"), "ColorBalanceProcessor");
		return outputImage.CopyFrom(inputImage);
	}
	else if (inputFormat != iimg::IBitmap::PF_RGB && inputFormat != iimg::IBitmap::PF_RGBA && inputFormat != iimg::IBitmap::PF_RGB24){
		SendErrorMessage(0, QObject::tr("Image format must be RGB(A)"), "RgbToGrayProcessor");

		return false;
	}

	ibase::CSize imageSize = inputImage.GetImageSize();

	istd::CChangeNotifier resultNotifier(&outputImage);
	Q_UNUSED(resultNotifier);

	if (!outputImage.CreateBitmap(iimg::IBitmap::PixelFormat(inputFormat), imageSize)){
		return false;
	}

	int redWeight = 255;
	int greenWeight = 255;
	int blueWeight = 255;

	imath::CVarVector values = colorCoefficientsPtr->GetValues();
	AssignWeights(values, redWeight, greenWeight, blueWeight);

	int inputPixelComponentCount = inputImage.GetComponentsCount();

	if (inputFormat == iimg::IBitmap::PixelFormat::PF_RGB){

		for (int y = 0; y < imageSize.GetY(); ++y){
			quint8* inputImageLinePtr = (quint8*)inputImage.GetLinePtr(y);
			quint8* outputImageLinePtr = (quint8*)outputImage.GetLinePtr(y);

			for (int x = 0; x < imageSize.GetX(); ++x){
				quint8* pixelPtr = inputImageLinePtr + x * inputPixelComponentCount;
				quint8* outPixelPtr = outputImageLinePtr + x * inputPixelComponentCount;

				outPixelPtr[0] = (redWeight * pixelPtr[0]) >> 8;
				outPixelPtr[1] = (greenWeight * pixelPtr[1]) >> 8;
				outPixelPtr[2] = (blueWeight * pixelPtr[2]) >> 8;
			}
		}
	}
	else{
		for (int y = 0; y < imageSize.GetY(); ++y){
			quint8* inputImageLinePtr = (quint8*)inputImage.GetLinePtr(y);
			quint8* outputImageLinePtr = (quint8*)outputImage.GetLinePtr(y);

			for (int x = 0; x < imageSize.GetX(); ++x){
				quint8* pixelPtr = inputImageLinePtr + x * inputPixelComponentCount;
				quint8* outPixelPtr = outputImageLinePtr + x * inputPixelComponentCount;

				outPixelPtr[0] = (redWeight * pixelPtr[0]) >> 8;
				outPixelPtr[1] = (greenWeight * pixelPtr[1]) >> 8;
				outPixelPtr[2] = (blueWeight * pixelPtr[2]) >> 8;
				outPixelPtr[3] = pixelPtr[3];
			}
		}
	}


	return true;
}


// private methods

void CColorBalanceProcessorComp::AssignWeights(imath::CVarVector& params, int& red, int& green, int& blue) const
{
	if (params.GetElementsCount() > 0){
		double weightSum = params[0];

		red = params[0] * 255;

		if (params.GetElementsCount() > 1){
			green = params[1] * 255;
		}

		if (params.GetElementsCount() > 2){
			blue = params[2] * 255;
		}
	}
}


} // namespace qcam


 
