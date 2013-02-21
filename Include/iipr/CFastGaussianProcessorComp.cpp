#include "iipr/CFastGaussianProcessorComp.h"


// Qt includes
#include <QtCore/QVector>


namespace iipr
{


struct KernelElement
{
	int offset;
	int shift;
};


template <	typename PixelType,
			typename WorkingType,
			int PixelSize,
			int ChannelsCount,
			int AlphaIndex,
			int ValueShift>
bool DoConvolution(
			const iimg::IBitmap& inputImage,
			KernelElement fastAccessElements[9],
			iimg::IBitmap& outputImage)
{
	istd::CIndex2d inputImageSize = inputImage.GetImageSize();
	istd::CIndex2d outputImageSize = inputImageSize;

	if (outputImageSize.IsSizeEmpty()){
		return false;
	}

	for (int y = 1; y < outputImageSize.GetY() - 1; ++y){
		const quint8* inputPtr = static_cast<const quint8*>(inputImage.GetLinePtr(y));
		quint8* outputPtr = static_cast<quint8*>(outputImage.GetLinePtr(y));

		for (int x = 1; x < outputImageSize.GetX() - 1; ++x){
			WorkingType sums[ChannelsCount] = {0};
			WorkingType alphaSum = 0;
			for (int i = 0; i < 9; ++i){
				const KernelElement& kernelElement = fastAccessElements[i];

				const PixelType* inputPixelPtr = (const PixelType*)(inputPtr + kernelElement.offset);
				for (int channelIndex = 0; channelIndex < ChannelsCount; ++channelIndex){
					WorkingType inputValue = inputPixelPtr[channelIndex];

					sums[channelIndex] += (inputValue << kernelElement.shift);
				}

				if (AlphaIndex >= 0){
					WorkingType inputValue = inputPixelPtr[AlphaIndex];

					alphaSum += inputValue;
				}
			}
			
			for (int channelIndex = 0; channelIndex < ChannelsCount; ++channelIndex){
				WorkingType outputValue = (sums[channelIndex] >> ValueShift);

				((PixelType*)outputPtr)[channelIndex] = PixelType(outputValue);
			}

			if (AlphaIndex >= 0){
				WorkingType outputValue = alphaSum >> ValueShift;

				((PixelType*)outputPtr)[AlphaIndex] = PixelType(outputValue);
			}

			inputPtr += PixelSize;
			outputPtr += PixelSize;
		}
	}

	return true;
}


bool DoGrayConvolution(
			const iimg::IBitmap& inputImage,
			KernelElement fastAccessElements[9],
			iimg::IBitmap& outputImage)
{
	istd::CIndex2d inputImageSize = inputImage.GetImageSize();
	istd::CIndex2d outputImageSize = inputImageSize;

	if (outputImageSize.IsSizeEmpty()){
		return false;
	}

	for (int y = 1; y < outputImageSize.GetY() - 1; ++y){
		const quint8* inputPtr = static_cast<const quint8*>(inputImage.GetLinePtr(y));
		quint8* outputPtr = static_cast<quint8*>(outputImage.GetLinePtr(y));

		for (int x = 1; x < outputImageSize.GetX() - 1; ++x){
			int sums = 0;
			for (int i = 0; i < 9; ++i){
				const KernelElement& kernelElement = fastAccessElements[i];

				const quint8* inputPixelPtr = (const quint8*)(inputPtr + kernelElement.offset);
				int inputValue = *inputPixelPtr;

				sums += (inputValue << kernelElement.shift);
			}

			int outputValue = (sums >> 4);

			*outputPtr = quint8(outputValue);

			inputPtr++;
			outputPtr++;
		}
	}

	return true;
}


// reimplemented (iipr::CImageProcessorCompBase)

bool CFastGaussianProcessorComp::ProcessImage(
			const iprm::IParamsSet* /*paramsPtr*/, 
			const iimg::IBitmap& inputImage,
			iimg::IBitmap& outputImage)
{
	istd::CIndex2d inputImageSize = inputImage.GetImageSize();
	if (inputImageSize != outputImage.GetImageSize()){
		if (!outputImage.CreateBitmap(inputImage.GetPixelFormat(), inputImageSize)){
			SendErrorMessage(0, "Output image could not be created");		

			return false;
		}
	}

	int linesDifference = inputImage.GetLinesDifference();
	int pixelsDifference = inputImage.GetPixelsDifference();

	double kernelSum = 0;

	switch (*m_kernelTypeAttrPtr){
		case KT_3x3:
			kernelSum = 16;
			break;

		default:
			return false;
	}

	int pixelFormat = inputImage.GetPixelFormat();

	KernelElement fastAccessElements[9];

	// 1. filter row
	fastAccessElements[0].offset = -pixelsDifference - linesDifference;
	fastAccessElements[0].shift = 0;
	fastAccessElements[1].offset = -linesDifference;
	fastAccessElements[1].shift = 1;
	fastAccessElements[2].offset = pixelsDifference - linesDifference;
	fastAccessElements[2].shift = 0;

	// 2. filter row
	fastAccessElements[3].offset = -pixelsDifference;
	fastAccessElements[3].shift = 1;
	fastAccessElements[4].offset = 0;
	fastAccessElements[4].shift = 2;
	fastAccessElements[5].offset = pixelsDifference;
	fastAccessElements[5].shift = 1;

	// 3. filter row
	fastAccessElements[6].offset = -pixelsDifference + linesDifference;
	fastAccessElements[6].shift = 0;
	fastAccessElements[7].offset = linesDifference;
	fastAccessElements[7].shift = 1;
	fastAccessElements[8].offset = pixelsDifference + linesDifference;
	fastAccessElements[8].shift = 0;

	switch (pixelFormat){
		case iimg::IBitmap::PF_GRAY:
			return DoGrayConvolution(inputImage, fastAccessElements, outputImage);

		case iimg::IBitmap::PF_RGB:
			return DoConvolution<quint8, qint32, 4, 4, -1, 4>(inputImage, fastAccessElements, outputImage);

		case iimg::IBitmap::PF_RGBA:
			return DoConvolution<quint8, qint32, 4, 4, 3, 4>(inputImage, fastAccessElements, outputImage);

		case iimg::IBitmap::PF_GRAY16:
			return DoConvolution<quint16, qint32, 1, 1, -1, 4>(inputImage, fastAccessElements, outputImage);

		case iimg::IBitmap::PF_GRAY32:
			return DoConvolution<quint32, qint64, 1, 1, -1, 4>(inputImage, fastAccessElements, outputImage);

		default:
			SendErrorMessage(0, "Unsupported image format");

			return false;
	}
}


} // namespace iipr


