#include "iipr/CConvolutionProcessorComp.h"


// Qt includes
#include <QtCore/QVector>


namespace iipr
{


struct KernelElement
{
	int offset;
	int factor;
	int alphaFactor;
};


template <	typename PixelType,
			typename WorkingType,
			int PixelSize,
			int ChannelsCount,
			int AlphaIndex,
			int ValueShift,
			bool UseClipMin,
			bool UseClipMax>
bool DoConvolution(
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

		for (int x = halfKernelWidth; x < outputImageSize.GetX() - halfKernelWidth; ++x){
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
			std::memset(outputPtr, 0, halfKernelWidth * PixelSize);
		}
	}

	for (int y = outputImageSize.GetY() - halfKernelHeight; y < outputImageSize.GetY(); ++y){
		std::memset(outputImage.GetLinePtr(y), 0, outputImageSize.GetX() * PixelSize);
	}

	return true;
}


struct KernelElementFloat
{
	int offset;
	double factor;
};


template <	typename PixelType,
			typename WorkingType,
			int PixelSize>
bool DoConvolutionFloat(
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

	QVector<KernelElementFloat> fastAccessElements;

	int halfKernelWidth = kernelSize.GetX() / 2;
	int halfKernelHeight = kernelSize.GetY() / 2;

	for (index[1] = 0; index[1] < kernelSize[1]; ++index[1]){
		for (index[0] = 0; index[0] < kernelSize[0]; ++index[0]){
			double value = kernel.GetKernelElement(index);

			KernelElementFloat element;
			element.offset = (index[0] - halfKernelWidth) * pixelsDifference + (index[1] - halfKernelHeight) * linesDifference;
			element.factor = value / kernelSum;

			if (element.factor != 0){
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
		std::memset(outputImage.GetLinePtr(y), 0, outputImageSize.GetX() * PixelSize);
	}

	int kernelElementsCount = int(fastAccessElements.size());

	#pragma omp parallel for

	for (int y = halfKernelHeight; y < outputImageSize.GetY() - halfKernelHeight; ++y){
		const quint8* inputPtr = static_cast<const quint8*>(inputImage.GetLinePtr(y));
		quint8* outputPtr = static_cast<quint8*>(outputImage.GetLinePtr(y));

		if (halfKernelWidth > 0){
			std::memset(outputPtr, 0, halfKernelWidth * PixelSize);
			outputPtr += halfKernelWidth * PixelSize;
		}

		for (int x = halfKernelWidth; x < outputImageSize.GetX() - halfKernelWidth; ++x){
			WorkingType sum = initialSum;

			for (int i = 0; i < kernelElementsCount; ++i){
				const KernelElementFloat& kernelElement = fastAccessElements[i];

				WorkingType inputValue = *(const PixelType*)(inputPtr + kernelElement.offset);

				sum += inputValue * kernelElement.factor;
			}

			*(PixelType*)outputPtr = sum;

			inputPtr += PixelSize;
			outputPtr += PixelSize;
		}

		if (halfKernelWidth > 0){
			std::memset(outputPtr, 0, halfKernelWidth * PixelSize);
		}
	}

	for (int y = outputImageSize.GetY() - halfKernelHeight; y < outputImageSize.GetY(); ++y){
		std::memset(outputImage.GetLinePtr(y), 0, outputImageSize.GetX() * PixelSize);
	}

	return true;
}


// reimplemented (iipr::TImageParamProcessorCompBase<ParameterType>)

bool CConvolutionProcessorComp::ParamProcessImage(
			const IConvolutionKernel2d* paramsPtr,
			const iimg::IBitmap& inputImage,
			iimg::IBitmap& outputImage)
{
	if (paramsPtr == NULL){
		SendErrorMessage(0, "Processing parameters are not set");		
		
		return false;
	}

	istd::CIndex2d inputImageSize = inputImage.GetImageSize();
	if (inputImageSize != outputImage.GetImageSize()){
		if (!outputImage.CreateBitmap(inputImage.GetPixelFormat(), inputImageSize)){
			SendErrorMessage(0, "Output image could not be created");		

			return false;
		}
	}

	int pixelFormat = inputImage.GetPixelFormat();
	switch (pixelFormat){
	case iimg::IBitmap::PF_GRAY:
		return DoConvolution<quint8, qint32, 1, 1, -1, 22, true, true>(inputImage, *paramsPtr, *m_normalizeKernelAttrPtr, outputImage);

	case iimg::IBitmap::PF_RGB:
		return DoConvolution<quint8, qint32, 4, 4, -1, 22, true, true>(inputImage, *paramsPtr, *m_normalizeKernelAttrPtr, outputImage);

	case iimg::IBitmap::PF_RGBA:
		return DoConvolution<quint8, qint32, 4, 4, 3, 22, true, true>(inputImage, *paramsPtr, *m_normalizeKernelAttrPtr, outputImage);

	case iimg::IBitmap::PF_GRAY16:
		return DoConvolution<quint16, qint32, 2, 1, -1, 14, true, true>(inputImage, *paramsPtr, *m_normalizeKernelAttrPtr, outputImage);

	case iimg::IBitmap::PF_GRAY32:
		return DoConvolution<quint32, qint64, 1, 1, -1, 30, true, true>(inputImage, *paramsPtr, *m_normalizeKernelAttrPtr, outputImage);

	case iimg::IBitmap::PF_FLOAT32:
		return DoConvolutionFloat<float, double, 4>(inputImage, *paramsPtr, *m_normalizeKernelAttrPtr, outputImage);

	case iimg::IBitmap::PF_FLOAT64:
		return DoConvolutionFloat<double, double, 8>(inputImage, *paramsPtr, *m_normalizeKernelAttrPtr, outputImage);

	default:
		return false;
	}
}


} // namespace iipr


