#include "iipr/CAdaptiveImageBinarizeProcessorComp.h"


// ACF includes
#include "istd/CChangeNotifier.h"
#include "iprm/TParamsPtr.h"
#include "iimg/CGeneralBitmap.h"

// ACF-Solutions includes
#include "iipr/CRectImageSmoothProcessorComp.h"


namespace iipr
{


// reimplemented (iproc::IProcessor)

int CAdaptiveImageBinarizeProcessorComp::DoProcessing(
				const iprm::IParamsSet* /*paramsPtr*/,
				const istd::IPolymorphic* inputPtr,
				istd::IChangeable* outputPtr,
				ibase::IProgressManager* /*progressManagerPtr*/)
{
	const iimg::IBitmap* inputBitmapPtr = dynamic_cast<const iimg::IBitmap*>(inputPtr);
	if (inputBitmapPtr == NULL){
		return TS_INVALID;
	}

	iimg::IBitmap* outputBitmapPtr = dynamic_cast<iimg::IBitmap*>(outputPtr);
	if (outputBitmapPtr == NULL){
		return TS_INVALID;
	}

	return ConvertImage(*inputBitmapPtr, 0.05, *outputBitmapPtr) ? TS_OK : TS_INVALID;
}


// private methods

bool CAdaptiveImageBinarizeProcessorComp::ConvertImage(
			const iimg::IBitmap& inputBitmap,
			double minContrast,
			iimg::IBitmap& outputBitmap) const
{
	if (inputBitmap.IsEmpty()){
		SendWarningMessage(0, "Input bitmap is empty.");

		return false;
	}

	iimg::CGeneralBitmap smoothedBitmap;

	CRectImageSmoothProcessorComp::DoImageFilter(inputBitmap, smoothedBitmap, 3, 3, iipr::CRectImageSmoothProcessorComp::BM_STRETCH_KERNEL);

	if (!outputBitmap.CreateBitmap(smoothedBitmap.GetPixelFormat(), smoothedBitmap.GetImageSize())){
		return false;
	}

	int imageWidth = smoothedBitmap.GetImageSize().GetX();
	int imageHeight = smoothedBitmap.GetImageSize().GetY();

	double threshold = 0.0;
	int pixelCount = 0;

	struct TileInfo
	{
		TileInfo()
			:thresholdSum(0),
			pixelCount(0)
		{
		}

		int thresholdSum;
		int pixelCount;
	};

	QMap<int, TileInfo> tileThresholds;

	int minContrastNorm = minContrast * 255;

	for (int y = 0; y < imageHeight; ++y){
		quint8* inputImageBufferPtr = (quint8*)inputBitmap.GetLinePtr(y);
		quint8* smoothedImageBufferPtr = (quint8*)smoothedBitmap.GetLinePtr(y);

		for (int x = 0; x < imageWidth; ++x){
			int tileIndex = GetTileIndex(x, y, -1);

			if (!tileThresholds.contains(tileIndex)){
				tileThresholds[tileIndex] = TileInfo();
			}

			int diff = labs(*inputImageBufferPtr - *smoothedImageBufferPtr);
			if (diff >= minContrastNorm)
			{
				tileThresholds[tileIndex].thresholdSum += *inputImageBufferPtr;
				tileThresholds[tileIndex].pixelCount++;
			}

			++inputImageBufferPtr, ++smoothedImageBufferPtr;
		}
	}

	if (pixelCount > 0)
	{
		threshold /= double(pixelCount);
	}

	for (int y = 0; y < imageHeight; ++y){
		quint8* outputImageBufferPtr = (quint8*)outputBitmap.GetLinePtr(y);
		quint8* inputImageBufferPtr = (quint8*)inputBitmap.GetLinePtr(y);

		for (int x = 0; x < imageWidth; ++x){
			*outputImageBufferPtr = *inputImageBufferPtr > threshold ? 255 : 0;

			++outputImageBufferPtr, ++inputImageBufferPtr;
		}
	}

	return true;
}


int CAdaptiveImageBinarizeProcessorComp::GetTileIndex(int x, int y, int tileSize)
{
	if (tileSize < 0){
		return 0;
	}

	return -1;
}


} // namespace iipr


