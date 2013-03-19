#include "iipr/CBitmapOperations.h"


// STL incldues
#include <cstring>

// ACF includes
#include "iimg/CScanlineMask.h"


namespace iipr
{


// public static methods

bool CBitmapOperations::ReduceBitmapToRegion(
			const iimg::IBitmap& inputBitmap,
			const i2d::IObject2d& region,
			iimg::IBitmap& outputBitmap)
{
	istd::CIndex2d inputBitmapSize = inputBitmap.GetImageSize();

	iimg::CScanlineMask bitmapRegion;
	i2d::CRect clipArea(inputBitmapSize);
	if (!bitmapRegion.CreateFromGeometry(region, &clipArea)){
		return false;
	}

	i2d::CRect regionRect = bitmapRegion.GetBoundingBox();

	int regionLeft = qMax(regionRect.GetLeft(), 0);
	int regionRight = qMin(regionRect.GetRight(), inputBitmapSize.GetX());
	int regionTop = qMax(regionRect.GetTop(), 0);
	int regionBottom = qMin(regionRect.GetBottom(), inputBitmapSize.GetY());

	if ((regionRight <= regionLeft) || (regionBottom <= regionTop)){
		outputBitmap.ResetImage();

		return true;
	}

	ibase::CSize outputBitmapSize(
				int(regionRight - regionLeft),
				int(regionBottom - regionTop));
	if (!outputBitmap.CreateBitmap(inputBitmap.GetPixelFormat(), outputBitmapSize)){
		return false;
	}

	int outputBitmapLineSize = outputBitmap.GetLineBytesCount();
	quint8* outputImageBufferPtr = (quint8*)outputBitmap.GetLinePtr(0);
	std::memset(outputImageBufferPtr, 0, outputBitmapLineSize * outputBitmap.GetImageSize().GetY()); 

	int pixelBits = outputBitmap.GetPixelBitsCount();
	int pixelBytes = pixelBits / 8;

	if (pixelBytes * 8 != pixelBits){
		return false;
	}
	
	for (int y = regionTop; y < regionBottom; y++){
		const quint8* inputLinePtr = (quint8*)inputBitmap.GetLinePtr(y);

		istd::CIntRanges::RangeList rangesList;
		const istd::CIntRanges* rangesPtr = bitmapRegion.GetPixelRanges(y);
		if (rangesPtr == NULL){
			continue;
		}

		rangesPtr->GetAsList(clipArea.GetHorizontalRange(), rangesList);

		quint8* outputImageLinePtr = (quint8*)outputBitmap.GetLinePtr(y - regionTop);

		for (		istd::CIntRanges::RangeList::ConstIterator iter = rangesList.begin();
					iter != rangesList.end();
					++iter){
			const istd::CIntRange& pixelRange = *iter;

			int rangeStart = qMax(pixelRange.GetMinValue(), 0);
			Q_ASSERT(rangeStart >= regionLeft);
			int rangeEnd = qMin(pixelRange.GetMaxValue(), inputBitmap.GetImageSize().GetX());
			Q_ASSERT(rangeEnd <= regionRight);

			int bytesToCopy = (rangeEnd - rangeStart) * pixelBytes;

			if (bytesToCopy > 0){
				std::memcpy(	outputImageLinePtr + (rangeStart - regionLeft) * pixelBytes,
								inputLinePtr + rangeStart * pixelBytes,
								bytesToCopy);
			}
		}
	}

	return true;
}


bool CBitmapOperations::JoinBitmaps(
			const BitmapList& bitmapList,
			JoinMode joinMode,
			iimg::IBitmap& outputBitmap,
			istd::ILogger* operationLogPtr)
{
	int height = 0;
	int width = 0;
	int inputBitmapsCount = bitmapList.count();

	if (inputBitmapsCount <= 0){
		if (operationLogPtr != NULL){
			operationLogPtr->SendLogMessage(istd::IInformationProvider::IC_ERROR, 0, "The list of input bitmaps is empty", "BitmapOperations");
		}

		return false;
	}

	iimg::IBitmap::PixelFormat outputPixelFormat = bitmapList.at(0)->GetPixelFormat();

	// Calculate the size of the output bitmap:
	for (int i = 0; i < inputBitmapsCount; i++){
		istd::CIndex2d imageSize = bitmapList.at(i)->GetImageSize();
		if (joinMode == JM_VERTICAL){
			height += imageSize.GetY();
			width = qMax(width, imageSize.GetX());
		}
		else{
			width += imageSize.GetX();
			height = qMax(height, imageSize.GetY());
		}
	}

	if (!outputBitmap.CreateBitmap(outputPixelFormat, istd::CIndex2d(width, height))){
		if (operationLogPtr != NULL){
			operationLogPtr->SendLogMessage(istd::IInformationProvider::IC_ERROR, 0, "Output bitmap could not be created", "BitmapOperations");
		}

		return false;
	}

	int offsetX = 0;
	int offsetY = 0;

	for (int index = 0; index < inputBitmapsCount; index++){
		const iimg::IBitmap& inputSingleBitmap = *bitmapList.at(index);

		int singleBitmapHeight = inputSingleBitmap.GetImageSize().GetY();

		// If there is a bitmap in the list with different pixel format, cancel the calculation and exit:
		if (inputSingleBitmap.GetPixelFormat() != outputPixelFormat){
			if (operationLogPtr != NULL){
				operationLogPtr->SendLogMessage(istd::IInformationProvider::IC_ERROR, 0, "Input bitmaps have different pixel format", "BitmapOperations");
			}

			outputBitmap.ResetImage();

			return false;
		}

		int lineBytes = inputSingleBitmap.GetLineBytesCount();

		for (int y = 0; y < singleBitmapHeight; y++){
			void* outputLinePtr = NULL;

			// Do vertical joining:
			if (joinMode == JM_VERTICAL){
				outputLinePtr = const_cast<void*>(outputBitmap.GetLinePtr(offsetY++));
			}
			// Do horizontal joining:
			else{
				outputLinePtr = (quint8*)(outputBitmap.GetLinePtr(y)) + offsetX;
			}

			const void* inputLinePtr = inputSingleBitmap.GetLinePtr(y);

			Q_ASSERT(outputLinePtr != NULL);
			Q_ASSERT(inputLinePtr != NULL);

			memcpy(outputLinePtr, inputLinePtr, lineBytes);
		}
	}

	return true;
}


bool CBitmapOperations::CombineBitmaps(
			const BitmapList& bitmapList,
			CombineMode combineMode,
			iimg::IBitmap& outputBitmap,
			imath::CVarVector* weghtsPtr,
			istd::ILogger* operationLogPtr)
{
	int inputBitmapsCount = bitmapList.count();

	if (inputBitmapsCount <= 0){
		if (operationLogPtr != NULL){
			operationLogPtr->SendLogMessage(istd::IInformationProvider::IC_ERROR, 0, "The list of input bitmaps is empty", "BitmapOperations");
		}

		return false;
	}

	iimg::IBitmap::PixelFormat outputPixelFormat = bitmapList.at(0)->GetPixelFormat();

	switch (outputPixelFormat){
		case iimg::IBitmap::PF_GRAY:
		case iimg::IBitmap::PF_RGB:
		case iimg::IBitmap::PF_RGBA:
			break;

		default:
			if (operationLogPtr != NULL){
				operationLogPtr->SendLogMessage(istd::IInformationProvider::IC_ERROR, 0, "Currently the image format is not supported for this operation", "BitmapOperations");
			}

			return false;
	}

	int width = bitmapList.at(0)->GetImageSize().GetX();
	int height = bitmapList.at(0)->GetImageSize().GetY();

	// Calculate the size of the output bitmap:
	for (int i = 0; i < inputBitmapsCount; i++){
		istd::CIndex2d imageSize = bitmapList.at(i)->GetImageSize();

		width = qMin(width, imageSize.GetX());
		height = qMin(height, imageSize.GetY());
	}

	if (!outputBitmap.CreateBitmap(outputPixelFormat, istd::CIndex2d(width, height))){
		if (operationLogPtr != NULL){
			operationLogPtr->SendLogMessage(istd::IInformationProvider::IC_ERROR, 0, "Output bitmap could not be created", "BitmapOperations");
		}

		return false;
	}

	int pixelComponentsCount = outputBitmap.GetComponentsCount();

	for (int y = 0; y < height; y++){
		void* outputLinePtr = const_cast<void*>(outputBitmap.GetLinePtr(y));

		for (int x = 0; x < width; x++){
			for (int componentIndex = 0; componentIndex < pixelComponentsCount; componentIndex++){
				quint32 outputPixelComponentValue = 0;
				double weightSum = 0.0;

				switch (combineMode){
				case CM_MIN:
					weightSum = 1.0;
					outputPixelComponentValue = INT_MAX;
					break;

				case CM_MAX:
					weightSum = 1.0;
					break;

				default:
					break;
				}

				int pixelComponentIndex = x * pixelComponentsCount + componentIndex;

				for (int inputBitmapIndex = 0; inputBitmapIndex < inputBitmapsCount; inputBitmapIndex++){
					const iimg::IBitmap& inputBitmap = *bitmapList.at(inputBitmapIndex);

					const quint8* inputPixelPtr = ((quint8*)inputBitmap.GetLinePtr(y)) + pixelComponentIndex;

					switch (combineMode){
						case CM_AVERAGE:
							if (weghtsPtr == NULL){
								outputPixelComponentValue += *inputPixelPtr;
								weightSum += 1.0;
							}
							else{
								double weight = weghtsPtr->GetElement(inputBitmapIndex);
								Q_ASSERT(weight >= 0);
								Q_ASSERT(weight <= 1.0);

								outputPixelComponentValue += *inputPixelPtr * weight;
							}
							break;

						case CM_MAX:
							outputPixelComponentValue = qMax<quint32>(*inputPixelPtr, outputPixelComponentValue);
							break;

						case CM_MIN:
							outputPixelComponentValue = qMin<quint32>(*inputPixelPtr, outputPixelComponentValue);
							break;
					}
				}

				quint8* outputPixelPtr = ((quint8*)outputLinePtr) + pixelComponentIndex;

				*outputPixelPtr = quint8(outputPixelComponentValue / weightSum);
			}
		}
	}

	return true;
}


} // namespace iipr


