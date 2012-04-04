#include "iipr/CImageHistogramProcessorComp.h"


// STL includes
#include <cmath>


 // ACF includes
#include "istd/TChangeNotifier.h"

#include "ibase/CSize.h"

#include "iimg/CBitmapRegion.h"


namespace iipr
{


// reimplemented (iproc::IProcessor)

int CImageHistogramProcessorComp::DoProcessing(
				const iprm::IParamsSet* paramsPtr,
				const istd::IPolymorphic* inputPtr,
				istd::IChangeable* outputPtr,
				iproc::IProgressManager* /*progressManagerPtr*/)
{
	const iimg::IBitmap* inputBitmapPtr = dynamic_cast<const iimg::IBitmap*>(inputPtr);
	if (inputBitmapPtr == NULL){
		return TS_INVALID;
	}

	imeas::IDiscreteDataSequence* histogramPtr = dynamic_cast<imeas::IDiscreteDataSequence*>(outputPtr);
	if (histogramPtr == NULL){
		return TS_INVALID;
	}

	const i2d::IObject2d* aoiPtr = NULL;
	if (paramsPtr != NULL && m_aoiParamIdAttrPtr.IsValid()){
		aoiPtr = dynamic_cast<const i2d::IObject2d*>(paramsPtr->GetParameter(*m_aoiParamIdAttrPtr));
	}

	return CalculateHistogramFromBitmap(*inputBitmapPtr, aoiPtr, *histogramPtr) ? TS_OK : TS_INVALID;
}


// private methods

bool CImageHistogramProcessorComp::CalculateHistogramFromBitmap(
			const iimg::IBitmap& input,
			const i2d::IObject2d* aoiPtr,
			imeas::IDiscreteDataSequence& histogram) const
{
	if (input.IsEmpty()){
		histogram.ResetSequence();

		return true;
	}

	int componentsBitCount = input.GetComponentBitsCount();
	if (componentsBitCount != 8){
		SendWarningMessage(0, "Only 8-bit images are supported");

		return false;
	}

	ibase::CSize inputSize(input.GetImageSize());
	i2d::CRectangle realArea = i2d::CRectangle(inputSize);

	const i2d::IObject2d* usedAoiPtr = (aoiPtr != NULL) ? aoiPtr : &realArea;

	iimg::CBitmapRegion bitmapRegion(&input);

	if (!bitmapRegion.CreateFromGeometry(*usedAoiPtr)){
		SendWarningMessage(0, "Cannot create the region");

		return false;
	}

	if (bitmapRegion.IsBitmapRegionEmpty()){
		SendWarningMessage(0, "Cannot process an empty region");
	
		return false;
	}

	i2d::CRectangle regionRect = bitmapRegion.GetBoundingBox();
	int regionTop = int(regionRect.GetTop());
	int regionBottom = int(regionRect.GetBottom());

	int pixelBytesCount = input.GetComponentsCount();
	int usedColorComponents = pixelBytesCount;

	int pixelFormat = input.GetPixelFormat();
	switch (pixelFormat){
	case iimg::IBitmap::PF_RGB:
		usedColorComponents = 3;
		break;

	case iimg::IBitmap::PF_RGBA:
		usedColorComponents = 4;
		break;
	}

	int histogramSize = 256 * usedColorComponents;

	istd::TDelPtr<quint32, istd::ArrayAccessor<quint32> > histogramDataPtr(new quint32[histogramSize]);
	quint32* histogramDataBufferPtr = histogramDataPtr.GetPtr();

	std::memset(histogramDataBufferPtr, 0, histogramSize * sizeof(quint32));
	int pixelCount = 0;

	for(int y = regionTop; y < regionBottom; y++){
		const iimg::CBitmapRegion::PixelRanges* rangesPtr = bitmapRegion.GetPixelRanges(y);
		if (rangesPtr == NULL){
			continue;
		}

		I_ASSERT(y >= 0);
		I_ASSERT(y < input.GetImageSize().GetY());

		for (int rangeIndex = 0; rangeIndex < int(rangesPtr->size()); rangeIndex++){
			const iimg::CBitmapRegion::PixelRange& pixelRange = rangesPtr->at(rangeIndex);

			int rangeStart = int(pixelRange.range.GetMinValue());
			int rangeEnd = int(pixelRange.range.GetMaxValue());

			quint8* inputImagePtr = ((quint8*)pixelRange.pixelBufferPtr);
			
			for (int x = rangeStart; x < rangeEnd; x++){
				I_ASSERT(x  < input.GetImageSize().GetX());
				
				for (int pixelByteIndex = 0; pixelByteIndex < pixelBytesCount; pixelByteIndex++){
					quint8 pixelComponentValue = *inputImagePtr++;

					if (pixelByteIndex < usedColorComponents){
						++histogramDataBufferPtr[pixelByteIndex + pixelComponentValue * usedColorComponents];

						pixelCount++;
					}
				}
			}
		}
	}

	pixelCount /= usedColorComponents;

	double normFactor = pow(2.0, histogram.GetSampleDepth()) - 1;

	for (int histIndex = 0; histIndex < histogramSize; histIndex++){
		double normHist = histogramDataBufferPtr[histIndex] / double(pixelCount);

		histogramDataBufferPtr[histIndex] = quint32(normHist * normFactor + 0.5);
	}

	istd::CChangeNotifier changePtr(&histogram);

	return histogram.CreateDiscrSequence(256, histogramDataPtr.PopPtr(), true, 0, 0, sizeof(quint32) * 8, usedColorComponents);
}


} // namespace iipr

