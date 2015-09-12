#include "icam/CCalibratedCameraComp.h"


// STD includes
#include <cmath>

#if defined(_MSC_VER)
	#ifndef isnan
		#define isnan(x) _isnan(x)
	#endif
#endif

// ACF includes
#include "iprm/TParamsPtr.h"
#include "imeas/INumericValue.h"


using namespace std;


namespace icam
{


// public static methods

bool CCalibratedCameraComp::ReadImageResolution(const iimg::IBitmap& bitmap, double& resolution)
{
	iimg::IBitmap::PixelFormat pixelFormat = bitmap.GetPixelFormat();

	istd::CIndex2d imageSize = bitmap.GetImageSize();
	if (!imageSize.IsSizeEmpty()){
		if (imageSize.GetX() >= int(sizeof(double) + sizeof(RESOLUTION_PATTERN))){
			if (pixelFormat != iimg::IBitmap::PF_RGB){
				const qint8* rawData = (qint8*)bitmap.GetLinePtr(0);
				Q_ASSERT(rawData != NULL);
				quint32& bitmapPattern = (quint32&)rawData[0];
				double& value = (double&)rawData[sizeof(RESOLUTION_PATTERN)];

				if (bitmapPattern == RESOLUTION_PATTERN && !isnan(value) && (value > 0)){
					resolution = value;

					return true;
				}
			}
			else{
				const qint8* rawData = (qint8*)bitmap.GetLinePtr(0);
				qint32 bitmapPattern = rawData[1] << 24; 
				bitmapPattern |= (rawData[2] & 0xff) << 16; 
				bitmapPattern |= (rawData[5] & 0xff) << 8; 
				bitmapPattern |= rawData[6] & 0xff;

				qint8 resolutionData[8] = {0};

				resolutionData[0]= rawData[9];
				resolutionData[1]= rawData[10];
				resolutionData[2]= rawData[13];
				resolutionData[3]= rawData[14];
				resolutionData[4]= rawData[17];
				resolutionData[5]= rawData[18];
				resolutionData[6]= rawData[21];
				resolutionData[7]= rawData[22];

				double& value = (double&)resolutionData;

				if ((bitmapPattern == RESOLUTION_PATTERN) && !isnan(value) && (value > 0)){
					resolution = value;

					return true;
				}
			}
		}
	}

	return false;
}


bool CCalibratedCameraComp::WriteImageResolution(iimg::IBitmap& bitmap, double resolution)
{
	iimg::IBitmap::PixelFormat pixelFormat = bitmap.GetPixelFormat();

	istd::CIndex2d imageSize = bitmap.GetImageSize();
	if (!imageSize.IsSizeEmpty()){
		if (imageSize.GetX() >= int(sizeof(double) + sizeof(RESOLUTION_PATTERN))){
			quint8* rawData = (quint8*)bitmap.GetLinePtr(0);
			Q_ASSERT(rawData != NULL);

			if (pixelFormat != iimg::IBitmap::PF_RGB){
				qint32& bitmapPattern = (qint32&)rawData[0];
				double& bitmapCalib = (double&)rawData[sizeof(RESOLUTION_PATTERN)];

				bitmapPattern = qint32(RESOLUTION_PATTERN);
				bitmapCalib = resolution;

				return true;
			}
			else{
				rawData[1] = 0xff; 
				rawData[2] = 0x00; 
				rawData[5] = 0xff; 
				rawData[6] = 0x00;

				quint8* resolutionData = (quint8*)&resolution;

				rawData[9] = resolutionData[0];
				rawData[10] = resolutionData[1];
				rawData[13] = resolutionData[2];
				rawData[14] = resolutionData[3];
				rawData[17] = resolutionData[4];
				rawData[18] = resolutionData[5];
				rawData[21] = resolutionData[6];
				rawData[22] = resolutionData[7];

				return true;
			}
		}
	}

	return false;
}


// public methods

// reimplemented (iproc::IProcessor)

int CCalibratedCameraComp::DoProcessing(
		const iprm::IParamsSet* paramsPtr,
		const istd::IPolymorphic* inputPtr,
		istd::IChangeable* outputPtr,
		ibase::IProgressManager* progressManagerPtr)
{
	int retVal = BaseClass::DoProcessing(paramsPtr, inputPtr, outputPtr, progressManagerPtr);
	if (retVal == TS_OK){

		iprm::TParamsPtr<imeas::INumericValue> scaleParamPtr(paramsPtr, *m_scaleParamIdAttrPtr);
		if (scaleParamPtr.IsValid()){
			iimg::IBitmap* bitmapPtr = dynamic_cast<iimg::IBitmap*>(outputPtr);
			if (bitmapPtr != NULL){
				double targetResolution = 1.0;
				if (!ReadImageResolution(*bitmapPtr, targetResolution)){
					imath::CVarVector scaleValues = scaleParamPtr->GetValues();
					if (scaleValues.GetElementsCount() >= 1){
						targetResolution = scaleValues[0];
					}

					WriteImageResolution(*bitmapPtr, targetResolution);
				}
			}
		}
	}

	return retVal;
}


} // namespace icam


