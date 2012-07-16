#include "iipr/CImageCropProcessorComp.h"


// STL includes
#include <cstring>

 // ACF includes
#include "istd/TChangeNotifier.h"
#include "ibase/CSize.h"
#include "iimg/IBitmap.h"
#include "iimg/CScanlineMask.h"


namespace iipr
{


// protected methods

// reimplemented (CImageRegionProcessorCompBase)

bool CImageCropProcessorComp::ProcessImageRegion(
			const iimg::IBitmap& inputBitmap,
			const iprm::IParamsSet* /*paramsPtr*/,
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

	int regionLeft = qMax(regionRect.GetLeft(), 0);
	int regionRight = qMin(regionRect.GetRight(), inputBitmapSize.GetX());
	int regionTop = qMax(regionRect.GetTop(), 0);
	int regionBottom = qMin(regionRect.GetBottom(), inputBitmapSize.GetY());

	if ((regionRight <= regionLeft) || (regionBottom <= regionTop)){
		outputBitmapPtr->ResetImage();

		return true;
	}

	ibase::CSize outputBitmapSize(
				int(regionRight - regionLeft),
				int(regionBottom - regionTop));
	if (!outputBitmapPtr->CreateBitmap(inputBitmap.GetPixelFormat(), outputBitmapSize)){
		return false;
	}

	int outputBitmapLineSize = outputBitmapPtr->GetLineBytesCount();
	quint8* outputImageBufferPtr = (quint8*)outputBitmapPtr->GetLinePtr(0);
	std::memset(outputImageBufferPtr, 0, outputBitmapLineSize * outputBitmapPtr->GetImageSize().GetY()); 

	int pixelBits = outputBitmapPtr->GetPixelBitsCount();
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

		quint8* outputImageLinePtr = (quint8*)outputBitmapPtr->GetLinePtr(y - regionTop);

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


} // namespace iipr


