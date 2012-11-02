#include "iipr/CMorphologicalProcessorComp.h"


// STL includes
#include <cstring>

 // ACF includes
#include "istd/TChangeNotifier.h"
#include "ibase/CSize.h"
#include "iimg/CGeneralBitmap.h"
#include "iprm/TParamsPtr.h"
#include "iimg/CScanlineMask.h"

// ACF-Solitions includes
#include "imeas/INumericValue.h"


namespace iipr
{


template <typename PixelComponentType>
inline void MinFunctor(PixelComponentType inputValue, PixelComponentType& outputValue)
{
	if (inputValue < outputValue){
		outputValue = inputValue;
	}
}

template <typename PixelComponentType>
inline void MaxFunctor(PixelComponentType inputValue, PixelComponentType& outputValue)
{
	if (inputValue > outputValue){
		outputValue = inputValue;
	}
}


template <typename PixelComponentType, PixelComponentType OutputInitValue, void (*CalculateOutputValue)(PixelComponentType, PixelComponentType&)>
static void DoFilter(
			int kernelWidth,
			int kernelHeight,
			const iimg::IBitmap& inputImage,
			const i2d::CRect& region,
			iimg::IBitmap& outputImage)
{
	int kernelHalfWidth = kernelWidth / 2.0;
	int kernelHalfHeight = kernelHeight / 2.0;

	int regionLeft = region.GetLeft();
	int regionRight = region.GetRight();
	int regionTop = region.GetTop();
	int regionBottom = region.GetBottom();

	int inputImageWidth = inputImage.GetImageSize().GetX();
	int inputImageHeight = inputImage.GetImageSize().GetY();

	int componentsCount = outputImage.GetComponentsCount();

	for (int componentIndex = 0; componentIndex < componentsCount; componentIndex++){
		for( int y = regionTop; y < regionBottom; ++y){
			I_ASSERT(y >= 0);
			I_ASSERT(y < inputImageHeight);

			const PixelComponentType* inputLinePtr = (PixelComponentType*)inputImage.GetLinePtr(y);
			PixelComponentType* outputLinePtr = (PixelComponentType*)outputImage.GetLinePtr(y);

			for(int x = regionLeft; x < regionLeft + kernelHalfWidth; ++x){
				PixelComponentType outputValue = OutputInitValue;

				int outputComponentPosition = x * componentsCount + componentIndex;

				for(int kernelX = -kernelHalfWidth; kernelX <= kernelHalfWidth; ++kernelX){
					int pixelIndex = x + kernelX;

					if (pixelIndex >= 0 && pixelIndex < inputImageWidth){
						int componentPosition = pixelIndex * componentsCount + componentIndex;

						CalculateOutputValue(inputLinePtr[componentPosition], outputValue);
					}
				}

				*(outputLinePtr + outputComponentPosition) = outputValue;
			}

			for(int x = regionLeft + kernelHalfWidth; x < regionRight - kernelHalfWidth; ++x){
				PixelComponentType outputValue = OutputInitValue;
				int outputComponentPosition = x * componentsCount + componentIndex;

				for(int kernelX = -kernelHalfWidth; kernelX <= kernelHalfWidth; ++kernelX){
					int pixelIndex = (x + kernelX) * componentsCount + componentIndex;

					CalculateOutputValue(inputLinePtr[pixelIndex], outputValue);
				}

				*(outputLinePtr + outputComponentPosition) = outputValue;
			}

			for(int x = regionRight - kernelHalfWidth; x < regionRight; ++x){
				PixelComponentType outputValue = OutputInitValue;
				int outputComponentPosition = x * componentsCount + componentIndex;

				for(int kernelX = -kernelHalfWidth; kernelX <= kernelHalfWidth; ++kernelX){
					int pixelIndex = x + kernelX;

					if (pixelIndex >= 0 && pixelIndex < inputImageWidth){
						int componentPosition = pixelIndex * componentsCount + componentIndex;

						CalculateOutputValue(inputLinePtr[componentPosition], outputValue);
					}
				}

				*(outputLinePtr + outputComponentPosition) = outputValue;
			}
		}
	} // componentIndex

	for (int componentIndex = 0; componentIndex < componentsCount; componentIndex++){
		iimg::CGeneralBitmap tempBitmap;
		tempBitmap.CopyFrom(outputImage);

		int inputLineDifference = tempBitmap.GetLinesDifference();

		for( int y = regionTop; y < regionTop + kernelHalfHeight; ++y){
			I_ASSERT(y >= 0);
			I_ASSERT(y < inputImageHeight);

			const PixelComponentType* inputLinePtr = (PixelComponentType*)tempBitmap.GetLinePtr(y);
			PixelComponentType* outputLinePtr = (PixelComponentType*)outputImage.GetLinePtr(y);

			for(int x = regionLeft; x < regionRight; ++x){
				PixelComponentType outputValue = OutputInitValue;

				int componentPosition = x * componentsCount + componentIndex;

				for(int kernelY = -kernelHalfHeight; kernelY <= kernelHalfHeight; ++kernelY){
					int imageLineIndex = y + kernelY;

					if (imageLineIndex >= 0 && imageLineIndex < inputImageHeight){
						PixelComponentType value = inputLinePtr[componentPosition + kernelY * inputLineDifference];

						CalculateOutputValue(value, outputValue);
					}
				}

				*(outputLinePtr + componentPosition) = outputValue;
			}
		}

		for( int y = regionTop + kernelHalfHeight; y < regionBottom - kernelHalfHeight; ++y){
			I_ASSERT(y >= 0);
			I_ASSERT(y < inputImageHeight);

			const PixelComponentType* inputLinePtr = (PixelComponentType*)tempBitmap.GetLinePtr(y);
			PixelComponentType* outputLinePtr = (PixelComponentType*)outputImage.GetLinePtr(y);

			for(int x = regionLeft; x < regionRight; ++x){
				PixelComponentType outputValue = OutputInitValue;

				int componentPosition = x * componentsCount + componentIndex;

				for(int kernelY = -kernelHalfHeight; kernelY <= kernelHalfHeight; ++kernelY){
					PixelComponentType value = inputLinePtr[componentPosition + kernelY * inputLineDifference];

					CalculateOutputValue(value, outputValue);
				}

				*(outputLinePtr + componentPosition) = outputValue;
			}
		}

		for( int y = regionBottom - kernelHalfHeight; y < regionBottom; ++y){
			I_ASSERT(y >= 0);
			I_ASSERT(y < inputImageHeight);

			const PixelComponentType* inputLinePtr = (PixelComponentType*)tempBitmap.GetLinePtr(y);
			PixelComponentType* outputLinePtr = (PixelComponentType*)outputImage.GetLinePtr(y);

			for(int x = regionLeft; x < regionRight; ++x){
				PixelComponentType outputValue = OutputInitValue;
				int componentPosition = x * componentsCount + componentIndex;

				for(int kernelY = -kernelHalfHeight; kernelY <= kernelHalfHeight; ++kernelY){
					int imageLineIndex = y + kernelY;

					if (imageLineIndex >= 0 && imageLineIndex < inputImageHeight){
						PixelComponentType value = inputLinePtr[componentPosition + kernelY * inputLineDifference];

						CalculateOutputValue(value, outputValue);
					}
				}

				*(outputLinePtr + componentPosition) = outputValue;
			}
		}
	} // componentIndex
}


// protected methods

// reimplemented (CImageRegionProcessorCompBase)

bool CMorphologicalProcessorComp::ProcessImageRegion(
			const iimg::IBitmap& inputBitmap,
			const iprm::IParamsSet* paramsPtr,
			const i2d::IObject2d* aoiPtr,
			istd::IChangeable* outputPtr) const
{
	if (inputBitmap.IsEmpty()){
		SendWarningMessage(0, "Input bitmap is empty.");

		return false;
	}

	if (aoiPtr == NULL){
		SendWarningMessage(0, "Crop region is not defined");

		return false;
	}

	iimg::IBitmap* outputBitmapPtr = dynamic_cast<iimg::IBitmap*>(outputPtr);
	if (outputBitmapPtr == NULL){
		SendWarningMessage(0, "Output bitmap is not set");

		return false;
	}

	istd::CIndex2d inputBitmapSize = inputBitmap.GetImageSize();

	iimg::CScanlineMask bitmapRegion;
	i2d::CRect clipArea(inputBitmapSize);
	if (!bitmapRegion.CreateFromGeometry(*aoiPtr, &clipArea)){
		SendWarningMessage(0, "Cannot create the region");

		return false;
	}

	i2d::CRect regionRect = bitmapRegion.GetBoundingBox();
	if (regionRect.IsEmpty() || !regionRect.IsValid()){
		SendWarningMessage(0, "Cannot process an empty or invalid region");

		return false;
	}

	if (!outputBitmapPtr->CopyFrom(inputBitmap)){
		return false;
	}

	regionRect.Intersection(i2d::CRect(inputBitmap.GetImageSize()));

	iprm::TParamsPtr<imeas::INumericValue> filterSizePtr(paramsPtr, *m_filterSizeParamsIdAttrPtr);
	if (filterSizePtr == NULL){
		return false;
	}

	imath::CVarVector filterLengths = filterSizePtr->GetValues();
	int filterDimensionsCount = filterLengths.GetElementsCount();
	if (filterDimensionsCount < 1){
		return false;
	}

	int imageWidth = inputBitmapSize.GetX();
	int imageHeight = inputBitmapSize.GetY();

	int kernelMaxWidth = qMax(1, qMin(int(filterLengths[0]), imageWidth));
	int kernelMaxHeight = qMax(1, qMin((filterDimensionsCount < 2)? kernelMaxWidth: int(filterLengths[1]), imageHeight));

	I_ASSERT(kernelMaxWidth >= 1);
	I_ASSERT(kernelMaxHeight >= 1);

	int pixelFormat = inputBitmap.GetPixelFormat();
	switch (pixelFormat){
		case iimg::IBitmap::PF_GRAY:
		case iimg::IBitmap::PF_RGB:
		case iimg::IBitmap::PF_RGBA:
			if (*m_processingModeAttrPtr == PM_EROSION){
				DoFilter<quint8, 255, MinFunctor<quint8> >(kernelMaxWidth, kernelMaxHeight, inputBitmap, regionRect, *outputBitmapPtr);
			}
			else{
				DoFilter<quint8, 0, MaxFunctor<quint8> >(kernelMaxWidth, kernelMaxHeight, inputBitmap, regionRect, *outputBitmapPtr);
			}
			break;

		case iimg::IBitmap::PF_GRAY16:
			if (*m_processingModeAttrPtr == PM_EROSION){
				DoFilter<quint16, (1 << 16) - 1, MinFunctor<quint16> >(kernelMaxWidth, kernelMaxHeight, inputBitmap, regionRect, *outputBitmapPtr);
			}
			else{
				DoFilter<quint16, 0, MaxFunctor<quint16> >(kernelMaxWidth, kernelMaxHeight, inputBitmap, regionRect, *outputBitmapPtr);
			}
			break;

		case iimg::IBitmap::PF_GRAY32:
			if (*m_processingModeAttrPtr == PM_EROSION){
				DoFilter<quint32, (quint64(1) << 32) - 1, MinFunctor<quint32> >(kernelMaxWidth, kernelMaxHeight, inputBitmap, regionRect, *outputBitmapPtr);
			}
			else{
				DoFilter<quint32, 0, MaxFunctor<quint32> >(kernelMaxWidth, kernelMaxHeight, inputBitmap, regionRect, *outputBitmapPtr);
			}
			break;

	default:
		return false;
	}

	return true;
}


} // namespace iipr


