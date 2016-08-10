#include "iipr/CConvolutionProcessorComp.h"


// Qt includes
#include <QtCore/QVector>

// ACF includes
#include "iprm/TParamsPtr.h"

// ACF-Solutions includes
#include "iipr/CPixelManip.h"


namespace iipr
{


struct KernelElement
{
	int offset;
	int factor;
	int alphaFactor;
};


/**
	Complex convolution for any number of channels with boundaries check and integer arithmetic.
*/
template <	typename PixelType,
			typename WorkingType,
			int PixelSize,
			int ChannelsCount,
			int AlphaIndex,
			int ValueShift,
			bool UseClipMin,
			bool UseClipMax>
bool DoComplexConvolution(
			const iimg::IBitmap& inputImage,
			const IConvolutionKernel2d& kernel,
			bool normalizeKernel,
			iimg::IBitmap& outputImage)
{
	istd::CIndex2d kernelSize = kernel.GetKernelSize();
	if (kernelSize.IsSizeEmpty()){
		return false;
	}

	int linesDifference = inputImage.GetLinesDifference();
	int pixelsDifference = inputImage.GetPixelsDifference();

	istd::CIndex2d index;
	double kernelSum = 0.0;

	for (index[1] = 0; index[1] < kernelSize[1]; ++index[1]){
		for (index[0] = 0; index[0] < kernelSize[0]; ++index[0]){
			kernelSum += qFabs(kernel.GetKernelElement(index));
		}
	}

	if (!normalizeKernel || (kernelSum <= I_EPSILON)){
		kernelSum = 1.0;
	}

	QVector<KernelElement> fastAccessElements;

	int halfKernelWidth = kernelSize.GetX() / 2;
	int halfKernelHeight = kernelSize.GetY() / 2;

	for (index[1] = 0; index[1] < kernelSize[1]; ++index[1]){
		for (index[0] = 0; index[0] < kernelSize[0]; ++index[0]){
			double value = kernel.GetKernelElement(index);

			KernelElement element;
			element.offset = (index[0] - halfKernelWidth) * pixelsDifference + (index[1] - halfKernelHeight) * linesDifference;

			element.factor = int((1 << ValueShift) * value / kernelSum);

			element.alphaFactor = qAbs(element.factor);

			if (element.factor != 0){
				fastAccessElements.push_back(element);
			}
		}
	}

	static const WorkingType maxClipValue = (WorkingType(1) << (sizeof(PixelType) * 8)) - 1;

	istd::CIndex2d inputImageSize = inputImage.GetImageSize();
	istd::CIndex2d outputImageSize = inputImageSize;

	if (outputImageSize.IsSizeEmpty()){
		return false;
	}

	double offsetValue = kernel.GetOffsetValue();

	WorkingType initialSums[ChannelsCount];
	for (int channelIndex = 0; channelIndex < ChannelsCount; ++channelIndex){
		initialSums[channelIndex] = WorkingType(offsetValue * maxClipValue * (1 << ValueShift));
	}

	for (int y = 0; y < halfKernelHeight; ++y){
		std::memset(outputImage.GetLinePtr(y), 0, outputImageSize.GetX() * PixelSize);
	}

	int kernelElementsCount = int(fastAccessElements.size());

	for (int y = halfKernelHeight; y < outputImageSize.GetY() - halfKernelHeight; ++y){
		const quint8* inputPtr = static_cast<const quint8*>(inputImage.GetLinePtr(y));
		quint8* outputPtr = static_cast<quint8*>(outputImage.GetLinePtr(y));

		if (halfKernelWidth > 0){
			std::memset(outputPtr, 0, halfKernelWidth * PixelSize);
			outputPtr += halfKernelWidth * PixelSize;
		}

		int x;
		for (x = halfKernelWidth; x < outputImageSize.GetX() - halfKernelWidth; ++x){
			WorkingType sums[ChannelsCount];
			for (int channelIndex = 0; channelIndex < ChannelsCount; ++channelIndex){
				sums[channelIndex] = initialSums[channelIndex];
			}

			WorkingType alphaSum = 0;
			for (int i = 0; i < kernelElementsCount; ++i){
				const KernelElement& kernelElement = fastAccessElements[i];

				const PixelType* inputPixelPtr = (const PixelType*)(inputPtr + kernelElement.offset);
				for (int channelIndex = 0; channelIndex < ChannelsCount; ++channelIndex){
					WorkingType inputValue = inputPixelPtr[channelIndex];

					sums[channelIndex] += inputValue * kernelElement.factor;
				}

				if (AlphaIndex >= 0){
					WorkingType inputValue = inputPixelPtr[AlphaIndex];

					alphaSum += inputValue * kernelElement.alphaFactor;
				}
			}

			for (int channelIndex = 0; channelIndex < ChannelsCount; ++channelIndex){
				WorkingType outputValue = sums[channelIndex] >> ValueShift;
				if (UseClipMin){
					if (outputValue < 0){
						outputValue = 0;
					}
				}

				if (UseClipMax){
					if (outputValue > maxClipValue){
						outputValue = maxClipValue;
					}
				}

				((PixelType*)outputPtr)[channelIndex] = PixelType(outputValue);
			}

			if (AlphaIndex >= 0){
				WorkingType outputValue = alphaSum >> ValueShift;
				if (UseClipMin){
					if (outputValue < 0){
						outputValue = 0;
					}
				}

				if (UseClipMax){
					if (outputValue > maxClipValue){
						outputValue = maxClipValue;
					}
				}

				((PixelType*)outputPtr)[AlphaIndex] = PixelType(outputValue);
			}

			inputPtr += PixelSize;
			outputPtr += PixelSize;
		}

		if (halfKernelWidth > 0){
			std::memset(outputPtr + x * PixelSize, 0, halfKernelWidth * PixelSize);
		}
	}

	for (int y = outputImageSize.GetY() - halfKernelHeight; y < outputImageSize.GetY(); ++y){
		std::memset(outputImage.GetLinePtr(y), 0, outputImageSize.GetX() * PixelSize);
	}

	return true;
}


/**
	Simple convolution for single channel without boundaries check.
*/
template <	typename InputPixelType,
			typename OutputPixelType,
			typename WorkingType>
bool DoFastConvolution(
			const iimg::IBitmap& inputImage,
			const IConvolutionKernel2d& kernel,
			bool normalizeKernel,
			iimg::IBitmap& outputImage)
{
	istd::CIndex2d kernelSize = kernel.GetKernelSize();
	if (kernelSize.IsSizeEmpty()){
		return false;
	}

	int linesDifference = inputImage.GetLinesDifference();
	int pixelsDifference = inputImage.GetPixelsDifference();

	istd::CIndex2d index;
	double kernelSum = 0.0;

	for (index[1] = 0; index[1] < kernelSize[1]; ++index[1]){
		for (index[0] = 0; index[0] < kernelSize[0]; ++index[0]){
			kernelSum += qFabs(kernel.GetKernelElement(index));
		}
	}

	if (!normalizeKernel || (kernelSum <= I_EPSILON)){
		kernelSum = 1.0;
	}

	QVector< QPair<int, WorkingType> > fastAccessElements;

	int halfKernelWidth = kernelSize.GetX() / 2;
	int halfKernelHeight = kernelSize.GetY() / 2;

	for (index[1] = 0; index[1] < kernelSize[1]; ++index[1]){
		for (index[0] = 0; index[0] < kernelSize[0]; ++index[0]){
			double value = kernel.GetKernelElement(index);

			QPair<int, WorkingType> element;
			element.first = (index[0] - halfKernelWidth) * pixelsDifference + (index[1] - halfKernelHeight) * linesDifference;
			element.second = value / kernelSum;

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

//	#pragma omp parallel for

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

	switch (inputPixelFormat){
	case iimg::IBitmap::PF_GRAY:
		switch (outputPixelFormat){
		case iimg::IBitmap::PF_GRAY:
			return DoFastConvolution<quint8, quint8, iipr::CPixelManip::GrayCropAccum32<16> >(inputImage, *procParamPtr, *m_normalizeKernelAttrPtr, outputImage);

		case iimg::IBitmap::PF_RGB:
		case iimg::IBitmap::PF_RGBA:
			return DoFastConvolution<quint8, iipr::CPixelManip::Rgba, iipr::CPixelManip::RgbCropAccum32<16> >(inputImage, *procParamPtr, *m_normalizeKernelAttrPtr, outputImage);

		case iimg::IBitmap::PF_GRAY16:
			return DoFastConvolution<quint8, quint16, iipr::CPixelManip::GrayCropAccum32<8> >(inputImage, *procParamPtr, *m_normalizeKernelAttrPtr, outputImage);

		case iimg::IBitmap::PF_GRAY32:
			return DoFastConvolution<quint8, quint32, iipr::CPixelManip::GrayCropAccum32<8> >(inputImage, *procParamPtr, *m_normalizeKernelAttrPtr, outputImage);

		case iimg::IBitmap::PF_FLOAT32:
			return DoFastConvolution<quint8, float, double>(inputImage, *procParamPtr, *m_normalizeKernelAttrPtr, outputImage);

		case iimg::IBitmap::PF_FLOAT64:
			return DoFastConvolution<quint8, double, double>(inputImage, *procParamPtr, *m_normalizeKernelAttrPtr, outputImage);

		default:
			break;
		}
		break;

	case iimg::IBitmap::PF_RGB:
		switch (outputPixelFormat){
		case iimg::IBitmap::PF_GRAY:
			return DoFastConvolution<iipr::CPixelManip::Rgba, quint8, iipr::CPixelManip::RgbCropAccum32<22> >(inputImage, *procParamPtr, *m_normalizeKernelAttrPtr, outputImage);

		case iimg::IBitmap::PF_RGB:
		case iimg::IBitmap::PF_RGBA:
			return DoFastConvolution<iipr::CPixelManip::Rgba, iipr::CPixelManip::Rgba, iipr::CPixelManip::RgbCropAccum32<22> >(inputImage, *procParamPtr, *m_normalizeKernelAttrPtr, outputImage);

		case iimg::IBitmap::PF_GRAY16:
			return DoFastConvolution<iipr::CPixelManip::Rgba, quint16, iipr::CPixelManip::RgbCropAccum32<14> >(inputImage, *procParamPtr, *m_normalizeKernelAttrPtr, outputImage);

		default:
			break;
		}
		break;

	case iimg::IBitmap::PF_RGBA:
		if (outputPixelFormat == iimg::IBitmap::PF_RGBA){
			return DoComplexConvolution<quint8, qint32, 4, 4, 3, 22, true, true>(inputImage, *procParamPtr, *m_normalizeKernelAttrPtr, outputImage);
		}
		break;

	case iimg::IBitmap::PF_GRAY16:
		switch (outputPixelFormat){
		case iimg::IBitmap::PF_GRAY16:
			return DoFastConvolution<quint16, quint16, iipr::CPixelManip::GrayCropAccum32<8> >(inputImage, *procParamPtr, *m_normalizeKernelAttrPtr, outputImage);

		case iimg::IBitmap::PF_GRAY32:
			return DoFastConvolution<quint16, quint32, iipr::CPixelManip::GrayCropAccum32<8> >(inputImage, *procParamPtr, *m_normalizeKernelAttrPtr, outputImage);

		case iimg::IBitmap::PF_FLOAT32:
			return DoFastConvolution<quint16, float, double>(inputImage, *procParamPtr, *m_normalizeKernelAttrPtr, outputImage);

		case iimg::IBitmap::PF_FLOAT64:
			return DoFastConvolution<quint16, double, double>(inputImage, *procParamPtr, *m_normalizeKernelAttrPtr, outputImage);

		default:
			break;
		}
		break;

	case iimg::IBitmap::PF_GRAY32:
		switch (outputPixelFormat){
		case iimg::IBitmap::PF_GRAY32:
			return DoComplexConvolution<quint32, qint64, 1, 1, -1, 30, true, true>(inputImage, *procParamPtr, *m_normalizeKernelAttrPtr, outputImage);

		case iimg::IBitmap::PF_FLOAT32:
			return DoFastConvolution<quint32, float, double>(inputImage, *procParamPtr, *m_normalizeKernelAttrPtr, outputImage);

		case iimg::IBitmap::PF_FLOAT64:
			return DoFastConvolution<quint32, double, double>(inputImage, *procParamPtr, *m_normalizeKernelAttrPtr, outputImage);

		default:
			break;
		}
		break;

	case iimg::IBitmap::PF_FLOAT32:
		switch (outputPixelFormat){
		case iimg::IBitmap::PF_FLOAT32:
			return DoFastConvolution<float, float, double>(inputImage, *procParamPtr, *m_normalizeKernelAttrPtr, outputImage);

		case iimg::IBitmap::PF_FLOAT64:
			return DoFastConvolution<float, double, double>(inputImage, *procParamPtr, *m_normalizeKernelAttrPtr, outputImage);

		default:
			break;
		}
		break;

	case iimg::IBitmap::PF_FLOAT64:
		return DoFastConvolution<double, double, double>(inputImage, *procParamPtr, *m_normalizeKernelAttrPtr, outputImage);

	default:
		SendErrorMessage(0, QObject::tr("Input image format '%1' not supported").arg(m_formatList.GetOptionName(inputPixelFormat)));

		return false;
	}

	SendErrorMessage(0, QObject::tr("Cannot produce output image in format '%1' from input format '%2'").arg(m_formatList.GetOptionName(outputPixelFormat)).arg(m_formatList.GetOptionName(inputPixelFormat)));

	return false;
}


} // namespace iipr


