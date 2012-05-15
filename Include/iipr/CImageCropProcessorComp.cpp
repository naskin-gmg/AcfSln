#include "iipr/CImageCropProcessorComp.h"


 // ACF includes
#include "istd/TChangeNotifier.h"

#include "ibase/CSize.h"

#include "iimg/CBitmapRegion.h"


namespace iipr
{


// protected methods

// reimplemented (CImageRegionProcessorCompBase)

bool CImageCropProcessorComp::ProcessImageRegion(
			const iimg::IBitmap& input,
			const iprm::IParamsSet* /*paramsPtr*/,
			const i2d::IObject2d* aoiPtr,
			istd::IChangeable* outputPtr) const
{
	if (input.IsEmpty()){
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

	iimg::CBitmapRegion bitmapRegion(&input);
	if (!bitmapRegion.CreateFromGeometry(*aoiPtr)){
		SendWarningMessage(0, "Cannot create the region");

		return false;
	}

	if (bitmapRegion.IsBitmapRegionEmpty()){
		SendWarningMessage(0, "Cannot process an empty region");
		
		return false;
	}

	i2d::CRectangle regionRect = bitmapRegion.GetBoundingBox();
	ibase::CSize outputBitmapSize(
				int(regionRect.GetWidth()),
				int(regionRect.GetHeight()));

	if (!outputBitmapPtr->CreateBitmap(input.GetPixelFormat(), outputBitmapSize)){
		return false;
	}

	int outputBitmapLineSize = outputBitmapPtr->GetLineBytesCount();
	quint8* outputImageBufferPtr = (quint8*)outputBitmapPtr->GetLinePtr(0);
	memset(outputImageBufferPtr, 0, outputBitmapLineSize * outputBitmapPtr->GetImageSize().GetY()); 

	int regionLeft = int(regionRect.GetLeft());
	int regionTop = int(regionRect.GetTop());
	int regionBottom = int(regionRect.GetBottom());
	int pixelBytes = outputBitmapPtr->GetPixelBitsCount() / 8;
	
	for(int y = regionTop; y < regionBottom; y++){
		const iimg::CBitmapRegion::PixelRanges* rangesPtr = bitmapRegion.GetPixelRanges(y);
		if (rangesPtr == NULL){
			continue;
		}

		I_ASSERT((y - regionTop) >= 0);
		I_ASSERT((y - regionTop) < outputBitmapPtr->GetImageSize().GetY());

		quint8* outputImageLinePtr = (quint8*)outputBitmapPtr->GetLinePtr(y - regionTop);

		for (int rangeIndex = 0; rangeIndex < int(rangesPtr->size()); rangeIndex++){
			const iimg::CBitmapRegion::PixelRange& pixelRange = rangesPtr->at(rangeIndex);

			int rangeStart = int(pixelRange.range.GetMinValue()) - regionLeft;
			int rangeEnd = int(pixelRange.range.GetMaxValue()) - regionLeft;

			quint8* inputImagePtr = ((quint8*)pixelRange.pixelBufferPtr);
			
			for (int x = rangeStart; x < rangeEnd; x++){
				I_ASSERT(x < outputBitmapPtr->GetImageSize().GetX());
				I_ASSERT(x + regionLeft < input.GetImageSize().GetX());
				
				for (int pixelByteIndex = 0; pixelByteIndex < pixelBytes; pixelByteIndex++){
					*(outputImageLinePtr + x * pixelBytes + pixelByteIndex) = *inputImagePtr;

					inputImagePtr++;
				}
			}
		}
	}

	return true;
}


} // namespace iipr


