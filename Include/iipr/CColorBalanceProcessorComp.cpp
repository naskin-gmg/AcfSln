// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
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
	iprm::TParamsPtr<imeas::INumericValue> colorCoefficientsPtr(paramsPtr, m_colorCoefficientsIdAttrPtr, m_defaultColorCoefsCompPtr);
	if (!colorCoefficientsPtr.IsValid()){
		SendWarningMessageOnce(0, QObject::tr("No input or default color coefficients"), "ColorBalanceProcessor");
		outputImage.CopyFrom(inputImage);

		return true;
	}

	int inputFormat = inputImage.GetPixelFormat();
	if (inputFormat == iimg::IBitmap::PF_GRAY){
		SendWarningMessageOnce(0, QObject::tr("No color balancing for grayscale input image possible"), "ColorBalanceProcessor");
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
	if (!ValidateColorCoefficients(values)){
		SendWarningMessage(0, QObject::tr("Invalid color coefficients, color balancing omitted"), "ColorBalanceProcessor");
		outputImage.CopyFrom(inputImage);

		return true;
	}

	AssignWeights(values, redWeight, greenWeight, blueWeight);

	int inputPixelComponentCount = inputImage.GetComponentsCount();

	if (inputFormat == iimg::IBitmap::PixelFormat::PF_RGB24){

		for (int y = 0; y < imageSize.GetY(); ++y){
			quint8* inputImageLinePtr = (quint8*)inputImage.GetLinePtr(y);
			quint8* outputImageLinePtr = (quint8*)outputImage.GetLinePtr(y);

			for (int x = 0; x < imageSize.GetX(); ++x){
				quint8* pixelPtr = inputImageLinePtr + x * inputPixelComponentCount;
				quint8* outPixelPtr = outputImageLinePtr + x * inputPixelComponentCount;

				double red = (redWeight * pixelPtr[0]) / 256;
				double green = (greenWeight * pixelPtr[1]) / 256;
				double blue = (blueWeight * pixelPtr[2]) / 256;

				double factor = GetReductionFactor(red, green, blue);

				outPixelPtr[0] = qMin(int(factor * red), 255);
				outPixelPtr[1] = qMin(int(factor * green), 255);
				outPixelPtr[2] = qMin(int(factor * blue), 255);
			}
		}
	}
	else if (inputFormat == iimg::IBitmap::PixelFormat::PF_RGB || inputFormat == iimg::IBitmap::PixelFormat::PF_RGBA) {
		for (int y = 0; y < imageSize.GetY(); ++y){
			quint8* inputImageLinePtr = (quint8*)inputImage.GetLinePtr(y);
			quint8* outputImageLinePtr = (quint8*)outputImage.GetLinePtr(y);

			for (int x = 0; x < imageSize.GetX(); ++x){
				quint8* pixelPtr = inputImageLinePtr + x * inputPixelComponentCount;
				quint8* outPixelPtr = outputImageLinePtr + x * inputPixelComponentCount;


				double red = (redWeight * pixelPtr[2]) / 256;
				double green = (greenWeight * pixelPtr[1]) / 256;
				double blue = (blueWeight * pixelPtr[0]) / 256;

				double factor = GetReductionFactor(red, green, blue);

				outPixelPtr[0] = qMin(int(factor * blue), 255);
				outPixelPtr[1] = qMin(int(factor * green), 255);
				outPixelPtr[2] = qMin(int(factor * red), 255);
				outPixelPtr[3] = pixelPtr[3];
			}
		}
	}
	else
		return false;


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

			weightSum += params[1];
		}

		if (params.GetElementsCount() > 2){
			blue = params[2] * 255;

			weightSum += params[2];
		}

		//red /= weightSum;
		//green /= weightSum;
		//blue /= weightSum;
	}
}


double CColorBalanceProcessorComp::GetReductionFactor(double red, double green, double blue) const
{
	double maxValue = qMax(red, qMax(green, blue));

	if (maxValue > 255){
		return 255 / maxValue;
	}

	return 1.0;
}

bool CColorBalanceProcessorComp::ValidateColorCoefficients(const imath::CVarVector& colorCoefficients) const
{
	static const double insufficient = 0.01; // 100 times intensity reduction still doesnt make any sense

	int count = colorCoefficients.GetElementsCount();
	if (count < 3){
		return false;
	}

	bool valid = false;
	for (int index = 0; index < 3; ++index){
		valid = (valid || (fabs(colorCoefficients.GetElement(index)) > insufficient) );
	}

	return valid;
}

} // namespace qcam


 
