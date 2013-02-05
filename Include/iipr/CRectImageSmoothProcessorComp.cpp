#include "iipr/CRectImageSmoothProcessorComp.h"


// ACF includes
#include "imath/CFixedPointManip.h"

#include "iimg/CGeneralBitmap.h"


namespace iipr
{


// reimplemented (imeas::INumericConstraints)

int CRectImageSmoothProcessorComp::GetNumericValuesCount() const
{
	if (*m_unitModeAttrPtr == UM_PERCENT_DIAG){
		return 1;
	}
	else{
		return 2;
	}
}


QString CRectImageSmoothProcessorComp::GetNumericValueName(int index) const
{
	if (*m_unitModeAttrPtr == UM_PERCENT_DIAG){
		return QObject::tr("Filter");
	}
	else{
		switch (index){
		case 0:
			return QObject::tr("Filter width");

		case 1:
			return QObject::tr("Filter height");

		default:
			return "";
		}
	}
}


QString CRectImageSmoothProcessorComp::GetNumericValueDescription(int index) const
{
	if (*m_unitModeAttrPtr == UM_PERCENT_DIAG){
		return QObject::tr("Bluring filter length");
	}
	else{
		switch (index){
		case 0:
			return QObject::tr("Horizontal bluring filter length");

		case 1:
			return QObject::tr("Vertical bluring filter length");

		default:
			return "";
		}
	}
}


const imath::IUnitInfo& CRectImageSmoothProcessorComp::GetNumericValueUnitInfo(int /*index*/) const
{
	return *this;
}


// reimplemented (iipr::TImageParamProcessorCompBase<imeas::INumericValue>)

bool CRectImageSmoothProcessorComp::ParamProcessImage(
			const imeas::INumericValue* paramsPtr,
			const iimg::IBitmap& inputImage,
			iimg::IBitmap& outputImage)
{
	if (paramsPtr == NULL){
		return false;
	}

	if (inputImage.GetComponentBitsCount() != 8){
		return false;	// Bad input image format
	}

	int componentsCount = inputImage.GetComponentsCount();
	Q_ASSERT(inputImage.GetPixelBitsCount() == componentsCount * 8);

	istd::CIndex2d imageSize = inputImage.GetImageSize();
	if (imageSize.IsSizeEmpty()){
		return false;
	}

	imath::CVarVector filterLengths = paramsPtr->GetValues();
	int filterDimensionsCount = filterLengths.GetElementsCount();
	if (filterDimensionsCount < 1){
		return false;
	}

	int imageWidth = imageSize.GetX();
	int imageHeight = imageSize.GetY();

	int kernelMaxWidth = 0;
	int kernelMaxHeight = 0;
	switch (*m_unitModeAttrPtr){
	case UM_PERCENT:
		kernelMaxWidth = qMax(1, qMin(int(filterLengths[0] * imageWidth), imageWidth));
		kernelMaxHeight = qMax(1, qMin(int((filterDimensionsCount < 2)? filterLengths[0]: filterLengths[1] * imageHeight), imageHeight));
		break;

	case UM_PERCENT_DIAG:
		{
			double diag = qSqrt(double(imageWidth * imageWidth + imageHeight * imageHeight));
			kernelMaxWidth = qMax(1, qMin(int(filterLengths[0] * diag), imageWidth));
			kernelMaxHeight = qMax(1, qMin((filterDimensionsCount < 2)? kernelMaxWidth: int(filterLengths[1] * diag), imageHeight));
		}
		break;

	default:
		kernelMaxWidth = qMax(1, qMin(int(filterLengths[0]), imageWidth));
		kernelMaxHeight = qMax(1, qMin((filterDimensionsCount < 2)? kernelMaxWidth: int(filterLengths[1]), imageHeight));
		break;
	}

	Q_ASSERT(kernelMaxWidth >= 1);
	Q_ASSERT(kernelMaxHeight >= 1);

	istd::CIndex2d outputImageSize = imageSize;
	if (*m_borderModeAttrPtr != BM_STRETCH_KERNEL){
		outputImageSize[0] -= kernelMaxWidth - 1;
		outputImageSize[1] -= kernelMaxHeight - 1;
	}

	if (!outputImage.CreateBitmap(inputImage.GetPixelFormat(), outputImageSize)){
		return false;	// cannot create output image
	}

	iimg::IBitmap* firstPassOutputImagePtr = &outputImage;
	const iimg::IBitmap* secondPassInputImagePtr = &inputImage;
	int outputImageWidth = outputImageSize.GetX();

	iimg::CGeneralBitmap tempBitmap;
	if ((kernelMaxWidth > 1) && (kernelMaxHeight > 1)){
		tempBitmap.CreateBitmap(inputImage.GetPixelFormat(), istd::CIndex2d(outputImageWidth, imageHeight));

		firstPassOutputImagePtr = &tempBitmap;
		secondPassInputImagePtr = &tempBitmap;
	}
	else if ((kernelMaxWidth <= 1) && (kernelMaxHeight <= 1)){
		return outputImage.CopyFrom(inputImage);
	}

	if (kernelMaxWidth > 1){
		Q_ASSERT(firstPassOutputImagePtr != NULL);


		for (int componentIndex = 0; componentIndex < componentsCount; componentIndex++){
			for (int y = 0; y < imageHeight; ++y){
				quint8* outputPtr = (quint8*)firstPassOutputImagePtr->GetLinePtr(y) + componentIndex;

				const quint8* inputLinePtr = (const quint8*)inputImage.GetLinePtr(y);

				int meanValue = 0;

				int kernelWidth = 0;
				while (kernelWidth < kernelMaxWidth){
					meanValue += inputLinePtr[componentIndex + componentsCount * kernelWidth++];

					if (kernelWidth >= kernelMaxWidth){
						break;
					}

					if (*m_borderModeAttrPtr == BM_STRETCH_KERNEL){
						*outputPtr = quint8((meanValue + 0.5) / kernelWidth);

						outputPtr += componentsCount;
					}

					meanValue += inputLinePtr[componentIndex + componentsCount * kernelWidth++];
				}

				int headX = kernelWidth;
				int tailX = 0;
				while (headX < imageWidth){
					*outputPtr = quint8((meanValue + 0.5) / kernelWidth);

					outputPtr += componentsCount;

					meanValue += inputLinePtr[componentIndex + componentsCount * headX++];

					Q_ASSERT(tailX < imageWidth);
					meanValue -= inputLinePtr[componentIndex + componentsCount * tailX++];
				}

				if (*m_borderModeAttrPtr == BM_STRETCH_KERNEL){
					while (kernelWidth > 0){
						*outputPtr = quint8((meanValue + 0.5) / kernelWidth);

						outputPtr += componentsCount;

						Q_ASSERT(tailX < imageWidth);
						meanValue -= inputLinePtr[componentIndex + componentsCount * tailX++];
						kernelWidth--;

						if  (kernelWidth <= 0){
							break;
						}

						Q_ASSERT(tailX < imageWidth);
						meanValue -= inputLinePtr[componentIndex + componentsCount * tailX++];
						kernelWidth--;
					}
				}

				Q_ASSERT(outputPtr <= (quint8*)firstPassOutputImagePtr->GetLinePtr(y) + componentIndex + outputImageWidth * componentsCount);
			}
		}
	}

	if (kernelMaxHeight > 1){
		Q_ASSERT(secondPassInputImagePtr != NULL);

		int inputLinesDifference = secondPassInputImagePtr->GetLinesDifference();
		int outputLinesDifference = outputImage.GetLinesDifference();

		for (int x = 0; x < outputImageWidth * componentsCount; ++x){	
			double meanValue = 0;
			const quint8* inputHeadPixelPtr = ((const quint8*)secondPassInputImagePtr->GetLinePtr(0)) + x;
			const quint8* inputTailPixelPtr = inputHeadPixelPtr;
			quint8* outputPixelPtr = (quint8*)(outputImage.GetLinePtr(0)) + x;

			int kernelHeight = 0;
			while (kernelHeight < kernelMaxHeight){
				meanValue += *inputHeadPixelPtr;
				inputHeadPixelPtr += inputLinesDifference;
				kernelHeight++;

				if (kernelHeight >= kernelMaxHeight){
					break;
				}

				if (*m_borderModeAttrPtr == BM_STRETCH_KERNEL){
					*outputPixelPtr = quint8((meanValue + 0.5) / kernelHeight);
					outputPixelPtr += outputLinesDifference;
				}

				meanValue += *inputHeadPixelPtr;
				inputHeadPixelPtr += inputLinesDifference;
				kernelHeight++;
			}

			int headY = kernelHeight;

			for (;headY < imageHeight; ++headY){
				*outputPixelPtr = quint8((meanValue + 0.5) / kernelHeight);
				outputPixelPtr += outputLinesDifference;

				meanValue += *inputHeadPixelPtr;
				inputHeadPixelPtr += inputLinesDifference;

				meanValue -= *inputTailPixelPtr;
				inputTailPixelPtr += inputLinesDifference;
			}

			if (*m_borderModeAttrPtr == BM_STRETCH_KERNEL){
				while (kernelHeight > 0){
					*outputPixelPtr = quint8((meanValue + 0.5) / kernelHeight);
					outputPixelPtr += outputLinesDifference;

					meanValue -= *inputTailPixelPtr;
					inputTailPixelPtr += inputLinesDifference;
					--kernelHeight;

					if  (kernelHeight <= 0){
						break;
					}

					meanValue -= *inputTailPixelPtr;
					inputTailPixelPtr += inputLinesDifference;
					--kernelHeight;
				}
			}
		}
	}

	return true;
}


// reimplemented (imath::IUnitInfo)

int CRectImageSmoothProcessorComp::GetUnitType() const
{
	switch (*m_unitModeAttrPtr){
	case UM_PERCENT:
	case UM_PERCENT_DIAG:
		return UT_RELATIVE;

	default:
		return UT_TECHNICAL;
	}
}


QString CRectImageSmoothProcessorComp::GetUnitName() const
{
	switch (*m_unitModeAttrPtr){
	case UM_PERCENT:
	case UM_PERCENT_DIAG:
		return "%";

	default:
		return "px";
	}
}


double CRectImageSmoothProcessorComp::GetDisplayMultiplicationFactor() const
{
	switch (*m_unitModeAttrPtr){
	case UM_PERCENT:
	case UM_PERCENT_DIAG:
		return 100;

	default:
		return 1;
	}
}


istd::CRange CRectImageSmoothProcessorComp::GetValueRange() const
{
	switch (*m_unitModeAttrPtr){
	case UM_PERCENT:
	case UM_PERCENT_DIAG:
		return istd::CRange(0, 1);

	default:
		return istd::CRange(1, 100);
	}
}


const imath::IDoubleManip& CRectImageSmoothProcessorComp::GetValueManip() const
{
	static imath::CFixedPointManip pixelManip(0);
	static imath::CFixedPointManip percentManip(3);

	switch (*m_unitModeAttrPtr){
	case UM_PERCENT:
	case UM_PERCENT_DIAG:
		return percentManip;

	default:
		return pixelManip;
	}
}


} // namespace iipr


