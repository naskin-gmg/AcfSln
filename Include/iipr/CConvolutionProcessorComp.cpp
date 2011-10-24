#include "iipr/CConvolutionProcessorComp.h"


// STL includes
#include <vector>


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
			const istd::CIndex2d& kernelSize,
			std::vector<KernelElement> fastAccessElements,
			iimg::IBitmap& outputImage)
{
	I_ASSERT(kernelSize.GetX() > 0);
	I_ASSERT(kernelSize.GetY() > 0);

	static const WorkingType maxClipValue = (WorkingType(1) << (sizeof(PixelType) * 8 + ValueShift)) - 1;

	istd::CIndex2d inputImageSize = inputImage.GetImageSize();
	istd::CIndex2d outputImageSize(inputImageSize.GetX() - kernelSize.GetX() + 1, inputImageSize.GetY() - kernelSize.GetY() + 1);

	if (outputImageSize.IsSizeEmpty()){
		return false;
	}

	int kernelElementsCount = int(fastAccessElements.size());

	for (int y = 0; y < outputImageSize.GetY(); ++y){
		const I_BYTE* inputPtr = static_cast<const I_BYTE*>(outputImage.GetLinePtr(y));
		I_BYTE* outputPtr = static_cast<I_BYTE*>(outputImage.GetLinePtr(y));

		for (int x = 0; x < outputImageSize.GetX(); ++x){
			WorkingType sums[ChannelsCount] = {0};
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
		return false;
	}

	istd::CIndex2d kernelSize = paramsPtr->GetKernelSize();
	if (kernelSize.IsSizeEmpty()){
		return false;
	}

	int linesDifference = inputImage.GetLinesDifference();
	int pixelsDifference = inputImage.GetPixelsDifference();

	double maxKernelAbsValue = 0;
	istd::CIndex2d index;
	for (index[1] = 0; index[1] < kernelSize[1]; ++index[1]){
		for (index[0] = 0; index[0] < kernelSize[0]; ++index[0]){
			double absValue = std::fabs(paramsPtr->GetKernelElement(index));
			if (absValue > maxKernelAbsValue){
				maxKernelAbsValue = absValue;
			}
		}
	}

	double scaleFactor = 0;

	int pixelFormat = inputImage.GetPixelFormat();
	switch (pixelFormat){
	case iimg::IBitmap::PF_GRAY:
	case iimg::IBitmap::PF_RGB:
	case iimg::IBitmap::PF_RGBA:
		scaleFactor = (1 << 22) / maxKernelAbsValue;
		break;

	case iimg::IBitmap::PF_GRAY16:
		scaleFactor = ((1 << 14) - 1) / maxKernelAbsValue;
		break;

	case iimg::IBitmap::PF_GRAY32:
		scaleFactor = ((1 << 30) - 1) / maxKernelAbsValue;
		break;

	default:
		return false;
	}

	std::vector<KernelElement> fastAccessElements;

	for (index[1] = 0; index[1] < kernelSize[1]; ++index[1]){
		for (index[0] = 0; index[0] < kernelSize[0]; ++index[0]){
			double value = paramsPtr->GetKernelElement(index);

			KernelElement element;
			element.offset = index[0] * pixelsDifference + index[1] * linesDifference;
			element.factor = int(scaleFactor * value);
			element.alphaFactor = std::abs(element.factor);

			if (element.factor != 0){
				fastAccessElements.push_back(element);
			}
		}
	}

	switch (pixelFormat){
	case iimg::IBitmap::PF_GRAY:
		return DoConvolution<I_BYTE, I_SDWORD, 1, 1, -1, 22, true, true>(inputImage, kernelSize, fastAccessElements, outputImage);

	case iimg::IBitmap::PF_RGB:
		return DoConvolution<I_BYTE, I_SDWORD, 4, 4, -1, 22, true, true>(inputImage, kernelSize, fastAccessElements, outputImage);

	case iimg::IBitmap::PF_RGBA:
		return DoConvolution<I_BYTE, I_SDWORD, 4, 4, 3, 22, true, true>(inputImage, kernelSize, fastAccessElements, outputImage);

	case iimg::IBitmap::PF_GRAY16:
		return DoConvolution<I_WORD, I_SDWORD, 1, 1, -1, 14, true, true>(inputImage, kernelSize, fastAccessElements, outputImage);

	case iimg::IBitmap::PF_GRAY32:
		return DoConvolution<I_DWORD, I_SQWORD, 1, 1, -1, 30, true, true>(inputImage, kernelSize, fastAccessElements, outputImage);

	default:
		return false;
	}
}


} // namespace iipr


