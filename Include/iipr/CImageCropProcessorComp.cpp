#include "iipr/CImageCropProcessorComp.h"


 // ACF includes
#include "istd/TChangeNotifier.h"

#include "ibase/CSize.h"

#include "iimg/CBitmapRegion.h"


namespace iipr
{


// reimplemented (iproc::IProcessor)

int CImageCropProcessorComp::DoProcessing(
				const iprm::IParamsSet* paramsPtr,
				const istd::IPolymorphic* inputPtr,
				istd::IChangeable* outputPtr,
				iproc::IProgressManager* /*progressManagerPtr*/)
{
	const iimg::IBitmap* inputBitmapPtr = dynamic_cast<const iimg::IBitmap*>(inputPtr);
	if (inputBitmapPtr == NULL){
		return TS_INVALID;
	}

	iimg::IBitmap* outputBitmapPtr = dynamic_cast<iimg::IBitmap*>(outputPtr);
	if (outputBitmapPtr == NULL){
		return TS_INVALID;
	}

	const i2d::IObject2d* aoiPtr = NULL;
	if (paramsPtr != NULL && m_aoiParamsIdAttrPtr.IsValid()){
		aoiPtr = dynamic_cast<const i2d::IObject2d*>(paramsPtr->GetParameter((*m_aoiParamsIdAttrPtr).ToString()));
	}

	return ConvertImage(*inputBitmapPtr, aoiPtr, *outputBitmapPtr) ? TS_OK : TS_INVALID;
}


// private methods

bool CImageCropProcessorComp::ConvertImage(
			const iimg::IBitmap& input,
			const i2d::IObject2d* aoiPtr,
			iimg::IBitmap& outputBitmap) const
{
	if (input.IsEmpty()){
		SendWarningMessage(0, "Input bitmap is empty.");

		return false;
	}

	if (aoiPtr == NULL){
		SendWarningMessage(0, "Crop region is not defined");

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

	if (!outputBitmap.CreateBitmap(input.GetPixelFormat(), outputBitmapSize)){
		return false;
	}

	int outputBitmapLineSize = outputBitmap.GetLineBytesCount();
	I_BYTE* outputImageBufferPtr = (I_BYTE*)outputBitmap.GetLinePtr(0);
	memset(outputImageBufferPtr, 0, outputBitmapLineSize * outputBitmap.GetImageSize().GetY()); 

	int regionLeft = int(regionRect.GetLeft());
	int regionTop = int(regionRect.GetTop());
	int regionBottom = int(regionRect.GetBottom());
	int pixelBytes = outputBitmap.GetPixelBitsCount() / 8;
	
	for(int y = regionTop; y < regionBottom; y++){
		const iimg::CBitmapRegion::PixelRanges* rangesPtr = bitmapRegion.GetPixelRanges(y);
		if (rangesPtr == NULL){
			continue;
		}

		I_ASSERT((y - regionTop) >= 0);
		I_ASSERT((y - regionTop) < outputBitmap.GetImageSize().GetY());

		I_BYTE* outputImageLinePtr = (I_BYTE*)outputBitmap.GetLinePtr(y - regionTop);

		for (int rangeIndex = 0; rangeIndex < int(rangesPtr->size()); rangeIndex++){
			const iimg::CBitmapRegion::PixelRange& pixelRange = rangesPtr->at(rangeIndex);

			int rangeStart = int(pixelRange.range.GetMinValue()) - regionLeft;
			int rangeEnd = int(pixelRange.range.GetMaxValue()) - regionLeft;

			I_BYTE* inputImagePtr = ((I_BYTE*)pixelRange.pixelBufferPtr);
			
			for (int x = rangeStart; x < rangeEnd; x++){
				I_ASSERT(x < outputBitmap.GetImageSize().GetX());
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


