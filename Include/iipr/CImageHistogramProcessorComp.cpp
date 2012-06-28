#include "iipr/CImageHistogramProcessorComp.h"


// Qt includes
#include <QtCore/qmath.h>

// ACF includes
#include "istd/TChangeNotifier.h"
#include "ibase/CSize.h"
#include "iimg/IBitmap.h"
#include "iimg/CScanlineMask.h"


namespace iipr
{


// protected methods

// reimplemented (CImageRegionProcessorCompBase)

bool CImageHistogramProcessorComp::ProcessImageRegion(
			const iimg::IBitmap& inputBitmap,
			const iprm::IParamsSet* /*paramsPtr*/,
			const i2d::IObject2d* aoiPtr,
			istd::IChangeable* outputPtr) const
{
	imeas::IDiscreteDataSequence* histogramPtr = dynamic_cast<imeas::IDiscreteDataSequence*>(outputPtr);
	if (histogramPtr == NULL){
		return false;
	}

	if (inputBitmap.IsEmpty()){
		histogramPtr->ResetSequence();

		return true;
	}


	int componentsBitCount = inputBitmap.GetComponentBitsCount();
	if (componentsBitCount != 8){
		SendWarningMessage(0, "Only 8-bit images are supported");

		return false;
	}

	ibase::CSize inputBitmapSize(inputBitmap.GetImageSize());
	i2d::CRectangle realArea = i2d::CRectangle(inputBitmapSize);

	const i2d::IObject2d* usedAoiPtr = (aoiPtr != NULL)? aoiPtr: &realArea;

	iimg::CScanlineMask bitmapRegion;
	i2d::CRect clipArea(inputBitmap.GetImageSize());
	if (!bitmapRegion.CreateFromGeometry(*usedAoiPtr, &clipArea)){
		SendWarningMessage(0, "Cannot create the region");

		return false;
	}

	if (bitmapRegion.IsBitmapRegionEmpty()){
		SendWarningMessage(0, "Cannot process an empty region");
	
		return false;
	}

	i2d::CRect regionRect = bitmapRegion.GetBoundingBox();
	int regionTop = qMax(regionRect.GetTop(), 0);
	int regionBottom = qMin(regionRect.GetBottom(), inputBitmapSize.GetY());

	int pixelBytesCount = inputBitmap.GetComponentsCount();
	int usedColorComponents = pixelBytesCount;

	int pixelFormat = inputBitmap.GetPixelFormat();
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

	for (int y = regionTop; y < regionBottom; y++){
		const quint8* inputLinePtr = (quint8*)inputBitmap.GetLinePtr(y);

		const iimg::CScanlineMask::PixelRanges* rangesPtr = bitmapRegion.GetPixelRanges(y);
		if (rangesPtr == NULL){
			continue;
		}

		for (int rangeIndex = 0; rangeIndex < rangesPtr->size(); rangeIndex++){
			const istd::CIntRange& pixelRange = rangesPtr->at(rangeIndex);

			int rangeStart = qMax(pixelRange.GetMinValue(), 0);
			int rangeEnd = qMin(pixelRange.GetMaxValue(), inputBitmapSize.GetX());

			if (rangeEnd > rangeStart){
				const quint8* inputPixelPtr = inputLinePtr + rangeStart * pixelBytesCount;

				for (int x = rangeStart; x < rangeEnd; ++x){
					for (int componentIndex = 0; componentIndex < usedColorComponents; ++componentIndex){
						quint8 pixelComponentValue = inputPixelPtr[componentIndex];

						++histogramDataBufferPtr[componentIndex + pixelComponentValue * usedColorComponents];
					}

					inputPixelPtr += pixelBytesCount;
				}

				pixelCount += rangeEnd - rangeStart;
			}
		}
	}

	double normFactor = qPow(2.0, histogramPtr->GetSampleDepth()) - 1;

	for (int histIndex = 0; histIndex < histogramSize; histIndex++){
		double normHist = histogramDataBufferPtr[histIndex] / double(pixelCount);

		histogramDataBufferPtr[histIndex] = quint32(normHist * normFactor + 0.5);
	}

	istd::CChangeNotifier changePtr(histogramPtr);

	return histogramPtr->CreateDiscrSequence(256, histogramDataPtr.PopPtr(), true, 0, 0, sizeof(quint32) * 8, usedColorComponents);
}


} // namespace iipr

