#include "iipr/CConvolutionProcessorComp.h"


// Qt includes
#include <QtCore/QVector>

// ACF includes
#include "iprm/TParamsPtr.h"

// ACF-Solutions includes
#include "iipr/CPixelManip.h"
#include "iipr/CConvolutionKernel2d.h"


namespace iipr
{


/**
	Template convolution funtion.
*/
template <	typename InputPixelType,
			typename OutputPixelType,
			typename WorkingType>
bool DoFastConvolution(
			const iimg::IBitmap& inputImage,
			const IConvolutionKernel2d& kernel,
			iimg::IBitmap& outputImage)
{
	istd::CIndex2d kernelSize = kernel.GetKernelSize();
	Q_ASSERT(!kernelSize.IsSizeEmpty());

	int linesDifference = inputImage.GetLinesDifference();
	int pixelsDifference = inputImage.GetPixelsDifference();

	QVector< QPair<int, WorkingType> > fastAccessElements;

	int halfKernelWidth = kernelSize.GetX() / 2;
	int halfKernelHeight = kernelSize.GetY() / 2;

	istd::CIndex2d index;
	for (index[1] = 0; index[1] < kernelSize[1]; ++index[1]){
		for (index[0] = 0; index[0] < kernelSize[0]; ++index[0]){
			double value = kernel.GetKernelElement(index);

			QPair<int, WorkingType> element;
			element.first = (index[0] - halfKernelWidth) * pixelsDifference + (index[1] - halfKernelHeight) * linesDifference;
			element.second = value;

			if (element.second != 0){
				fastAccessElements.push_back(element);
			}
		}
	}

	istd::CIndex2d inputImageSize = inputImage.GetImageSize();
	istd::CIndex2d outputImageSize = inputImageSize;

	if (outputImageSize.IsSizeEmpty()){
		return false;
	}

	double offsetValue = kernel.GetOffsetValue();

	WorkingType initialSum = WorkingType(offsetValue);

	for (int y = 0; y < halfKernelHeight; ++y){
		std::memset(outputImage.GetLinePtr(y), 0, outputImageSize.GetX() * sizeof(OutputPixelType));
	}

	int kernelElementsCount = int(fastAccessElements.size());

	#pragma omp parallel for

	for (int y = halfKernelHeight; y < outputImageSize.GetY() - halfKernelHeight; ++y){
		const InputPixelType* inputPtr = static_cast<const InputPixelType*>(inputImage.GetLinePtr(y));
		OutputPixelType* outputPtr = static_cast<OutputPixelType*>(outputImage.GetLinePtr(y));

		if (halfKernelWidth > 0){
			std::memset(outputPtr, 0, halfKernelWidth * sizeof(OutputPixelType));
		}

		int x;
		for (x = halfKernelWidth; x < outputImageSize.GetX() - halfKernelWidth; ++x){
			WorkingType sum = initialSum;

			for (int i = 0; i < kernelElementsCount; ++i){
				const QPair<int, WorkingType>& kernelElement = fastAccessElements[i];

				WorkingType inputValue = *(const InputPixelType*)((quint8*)&inputPtr[x] + kernelElement.first);
				inputValue *= kernelElement.second;

				sum += inputValue;
			}

			outputPtr[x] = sum;
		}

		if (halfKernelWidth > 0){
			std::memset(outputPtr + x, 0, halfKernelWidth * sizeof(OutputPixelType));
		}
	}

	for (int y = outputImageSize.GetY() - halfKernelHeight; y < outputImageSize.GetY(); ++y){
		std::memset(outputImage.GetLinePtr(y), 0, outputImageSize.GetX() * sizeof(OutputPixelType));
	}

	return true;
}


// reimplemented (iipr::TImageParamProcessorCompBase<ParameterType>)

bool CConvolutionProcessorComp::ParamProcessImage(
			const iprm::IParamsSet* /*paramsPtr*/,
			const IConvolutionKernel2d* procParamPtr,
			iimg::IBitmap::PixelFormat outputPixelFormat,
			const iimg::IBitmap& inputImage,
			iimg::IBitmap& outputImage)
{
	if (procParamPtr == NULL){
		SendErrorMessage(0, "Processing parameters are not set");		
		
		return false;
	}

	iimg::IBitmap::PixelFormat inputPixelFormat = inputImage.GetPixelFormat();
	if (outputPixelFormat <= iimg::IBitmap::PF_UNKNOWN){
		outputPixelFormat = inputPixelFormat;
	}

	istd::CIndex2d inputImageSize = inputImage.GetImageSize();
	if (inputImageSize != outputImage.GetImageSize() || outputPixelFormat != outputImage.GetPixelFormat()){
		if (!outputImage.CreateBitmap(iimg::IBitmap::PixelFormat(outputPixelFormat), inputImageSize)){
			SendErrorMessage(0, "Output image could not be created");		

			return false;
		}
	}

	istd::CIndex2d kernelSize = procParamPtr->GetKernelSize();
	if (kernelSize.IsSizeEmpty()){
		return false;
	}

	double kernelPosSum = 0.0;
	double kernelSum = 0.0;

	istd::CIndex2d index;
	for (index[1] = 0; index[1] < kernelSize[1]; ++index[1]){
		for (index[0] = 0; index[0] < kernelSize[0]; ++index[0]){
			double kernelElement = procParamPtr->GetKernelElement(index);

			kernelSum += kernelElement;
			kernelPosSum += qFabs(kernelElement);
		}
	}

	iipr::CConvolutionKernel2d normalizedKernel;
	if (*m_normalizeKernelAttrPtr && (kernelPosSum > I_EPSILON)){
		normalizedKernel.SetKernelSize(kernelSize);

		for (index[1] = 0; index[1] < kernelSize[1]; ++index[1]){
			for (index[0] = 0; index[0] < kernelSize[0]; ++index[0]){
				double kernelElement = procParamPtr->GetKernelElement(index);

				normalizedKernel.SetKernelElement(index, kernelElement / kernelPosSum);
			}
		}

		procParamPtr = &normalizedKernel;
	}

	bool hasNegativeKernel = kernelSum != kernelPosSum;

	switch (inputPixelFormat){
	case iimg::IBitmap::PF_GRAY:
		switch (outputPixelFormat){
		case iimg::IBitmap::PF_GRAY:
			if (hasNegativeKernel){
				if (kernelPosSum > 1){
					return DoFastConvolution<quint8, quint8, iipr::CPixelManip::GrayCropAccum32<qint32, 16, true, false> >(inputImage, *procParamPtr, outputImage);
				}
				else{
					return DoFastConvolution<quint8, quint8, iipr::CPixelManip::GrayCropAccum32<qint32, 23, true, false> >(inputImage, *procParamPtr, outputImage);
				}
			}
			else{
				if (kernelPosSum > 1){
					return DoFastConvolution<quint8, quint8, iipr::CPixelManip::GrayCropAccum32<qint32, 16, false, true> >(inputImage, *procParamPtr, outputImage);
				}
				else{
					return DoFastConvolution<quint8, quint8, iipr::CPixelManip::GrayCropAccum32<quint32, 24, false, false> >(inputImage, *procParamPtr, outputImage);
				}
			}

		case iimg::IBitmap::PF_RGB:
		case iimg::IBitmap::PF_RGBA:
			return DoFastConvolution<quint8, iipr::CPixelManip::Rgba, iipr::CPixelManip::RgbCropAccum32<qint32, 16, true, true> >(inputImage, *procParamPtr, outputImage);

		case iimg::IBitmap::PF_GRAY16:
			return DoFastConvolution<quint8, quint16, iipr::CPixelManip::GrayCropAccum32<qint32, 8, true, false> >(inputImage, *procParamPtr, outputImage);

		case iimg::IBitmap::PF_GRAY32:
			return DoFastConvolution<quint8, quint32, iipr::CPixelManip::GrayCropAccum32<qint64, 16, true, false> >(inputImage, *procParamPtr, outputImage);

		case iimg::IBitmap::PF_FLOAT32:
			return DoFastConvolution<quint8, float, double>(inputImage, *procParamPtr, outputImage);

		case iimg::IBitmap::PF_FLOAT64:
			return DoFastConvolution<quint8, double, double>(inputImage, *procParamPtr, outputImage);

		default:
			break;
		}
		break;

	case iimg::IBitmap::PF_RGB:
		switch (outputPixelFormat){
		case iimg::IBitmap::PF_GRAY:
			return DoFastConvolution<iipr::CPixelManip::Rgba, quint8, iipr::CPixelManip::RgbCropAccum32<qint32, 22, true, true> >(inputImage, *procParamPtr, outputImage);

		case iimg::IBitmap::PF_RGB:
		case iimg::IBitmap::PF_RGBA:
			return DoFastConvolution<iipr::CPixelManip::Rgba, iipr::CPixelManip::Rgba, iipr::CPixelManip::RgbCropAccum32<qint32, 22, true, true> >(inputImage, *procParamPtr, outputImage);

		case iimg::IBitmap::PF_GRAY16:
			return DoFastConvolution<iipr::CPixelManip::Rgba, quint16, iipr::CPixelManip::RgbCropAccum32<qint32, 14, true, false> >(inputImage, *procParamPtr, outputImage);

		default:
			break;
		}
		break;

	case iimg::IBitmap::PF_RGBA:
		if (outputPixelFormat == iimg::IBitmap::PF_RGBA){
			return DoFastConvolution<iipr::CPixelManip::Rgba, iipr::CPixelManip::Rgba, iipr::CPixelManip::RgbaCropAccum32<qint32, 22, true, true> >(inputImage, *procParamPtr, outputImage);
		}
		break;

	case iimg::IBitmap::PF_GRAY16:
		switch (outputPixelFormat){
		case iimg::IBitmap::PF_GRAY:
			return DoFastConvolution<quint16, quint8, iipr::CPixelManip::GrayCropAccum32<qint32, 8, true, true> >(inputImage, *procParamPtr, outputImage);

		case iimg::IBitmap::PF_GRAY16:
			return DoFastConvolution<quint16, quint16, iipr::CPixelManip::GrayCropAccum32<qint32, 8, true, true> >(inputImage, *procParamPtr, outputImage);

		case iimg::IBitmap::PF_GRAY32:
			return DoFastConvolution<quint16, quint32, iipr::CPixelManip::GrayCropAccum32<qint64, 16, true, false> >(inputImage, *procParamPtr, outputImage);

		case iimg::IBitmap::PF_FLOAT32:
			return DoFastConvolution<quint16, float, double>(inputImage, *procParamPtr, outputImage);

		case iimg::IBitmap::PF_FLOAT64:
			return DoFastConvolution<quint16, double, double>(inputImage, *procParamPtr, outputImage);

		default:
			break;
		}
		break;

	case iimg::IBitmap::PF_GRAY32:
		switch (outputPixelFormat){
		case iimg::IBitmap::PF_GRAY32:
			return DoFastConvolution<quint32, quint32, iipr::CPixelManip::GrayCropAccum32<qint64, 16, true, true>>(inputImage, *procParamPtr, outputImage);

		case iimg::IBitmap::PF_FLOAT32:
			return DoFastConvolution<quint32, float, double>(inputImage, *procParamPtr, outputImage);

		case iimg::IBitmap::PF_FLOAT64:
			return DoFastConvolution<quint32, double, double>(inputImage, *procParamPtr, outputImage);

		default:
			break;
		}
		break;

	case iimg::IBitmap::PF_FLOAT32:
		switch (outputPixelFormat){
		case iimg::IBitmap::PF_FLOAT32:
			return DoFastConvolution<float, float, double>(inputImage, *procParamPtr, outputImage);

		case iimg::IBitmap::PF_FLOAT64:
			return DoFastConvolution<float, double, double>(inputImage, *procParamPtr, outputImage);

		default:
			break;
		}
		break;

	case iimg::IBitmap::PF_FLOAT64:
		return DoFastConvolution<double, double, double>(inputImage, *procParamPtr, outputImage);

	default:
		SendErrorMessage(0, QObject::tr("Input image format '%1' not supported").arg(m_formatList.GetOptionName(inputPixelFormat)));

		return false;
	}

	SendErrorMessage(0, QObject::tr("Cannot produce output image in format '%1' from input format '%2'").arg(m_formatList.GetOptionName(outputPixelFormat)).arg(m_formatList.GetOptionName(inputPixelFormat)));

	return false;
}


} // namespace iipr


