#include <iipr/CRgbToGrayProcessorComp.h>


// Qt includes
#include <QtCore/QObject>

 // ACF includes
#include <istd/CChangeNotifier.h>
#include <ibase/CSize.h>


namespace iipr
{


// reimplemented (iproc::IProcessor)

int CRgbToGrayProcessorComp::DoProcessing(
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

	return ConvertImage(*inputBitmapPtr, *outputBitmapPtr) ? TS_OK : TS_INVALID;
}


// private methods

bool CRgbToGrayProcessorComp::ConvertImage(
			const iimg::IBitmap& inputBitmap,
			iimg::IBitmap& outputBitmap) const
{
	if (inputBitmap.IsEmpty()){
		return true;
	}

	int inputFormat = inputBitmap.GetPixelFormat();
	if (inputFormat == iimg::IBitmap::PF_GRAY){
		return outputBitmap.CopyFrom(inputBitmap);
	}
	else if (inputFormat != iimg::IBitmap::PF_RGB && inputFormat != iimg::IBitmap::PF_RGBA){
		SendErrorMessage(0, QObject::tr("Image format must be RGB(A)"), "RgbToGrayProcessor");

		return false;
	}

	ibase::CSize imageSize = inputBitmap.GetImageSize();

	if (!outputBitmap.CreateBitmap(iimg::IBitmap::PF_GRAY, imageSize)){
		return false;
	}

	int inputPixelComponentCount = inputBitmap.GetComponentsCount();

    // the loops are optimized for efficient SIMD vectorization
	if (inputFormat == iimg::IBitmap::PF_RGBA){       
        for (int y = 0; y < imageSize.GetY(); ++y){
			quint8* inputImageLinePtr = (quint8*)inputBitmap.GetLinePtr(y);
			quint8* outputImageLinePtr = (quint8*)outputBitmap.GetLinePtr(y);

            for (int x = 0; x < imageSize.GetX(); ++x){
				quint8* pixelPtr = inputImageLinePtr + x * inputPixelComponentCount;

                outputImageLinePtr[x] = pixelPtr[3] * (77 * pixelPtr[0] + 151 * pixelPtr[1] + 28 * pixelPtr[2]) >> 16;
            }
		}
	}
	else{
        for (int y = 0; y < imageSize.GetY(); ++y){
			quint8* inputImageLinePtr = (quint8*)inputBitmap.GetLinePtr(y);
			quint8* outputImageLinePtr = (quint8*)outputBitmap.GetLinePtr(y);

            for (int x = 0; x < imageSize.GetX(); ++x){
				quint8* pixelPtr = inputImageLinePtr + x * inputPixelComponentCount;

                outputImageLinePtr[x] = (77 * pixelPtr[0] + 151 * pixelPtr[1] + 28 * pixelPtr[2]) >> 8;
			}
		}
	}

	return true;
}


} // namespace iipr


