#include <icam/CCalibratedCameraComp.h>


// STD includes
#include <cmath>

// ACF includes
#include <iprm/TParamsPtr.h>
#include <imeas/INumericValue.h>


using namespace std;


namespace icam
{


// public static methods

static const int s_resolutionPattern = 0xff00ff00;
static const int s_resolutionPatternXY = 0x00ff00ff;


bool CCalibratedCameraComp::ReadImageResolution(const iimg::IBitmap& bitmap, double& resolutionX, double& resolutionY)
{
	iimg::IBitmap::PixelFormat pixelFormat = bitmap.GetPixelFormat();

	istd::CIndex2d imageSize = bitmap.GetImageSize();
	if (!imageSize.IsSizeEmpty()){
		// read new pattern
		if (imageSize.GetX() >= int(2 * sizeof(double) + sizeof(s_resolutionPatternXY))){
			const qint8* rawData = (qint8*)bitmap.GetLinePtr(0);
			Q_ASSERT(rawData != NULL);
			quint32& bitmapPattern = (quint32&)rawData[0];
			double& value1 = (double&)rawData[sizeof(s_resolutionPatternXY)];
			double& value2 = (double&)rawData[sizeof(s_resolutionPatternXY) + sizeof(double)];
			if (bitmapPattern == s_resolutionPatternXY && !qIsNaN(value1) && (value1 > 0) && !qIsNaN(value2) && (value2 > 0))
			{
				resolutionX = value1;
				resolutionY = value2;
				return true;
			}
		}

		// else fall back to the older (wrong) solution
		if (imageSize.GetX() >= int(sizeof(double) + sizeof(s_resolutionPattern))){
			if (pixelFormat != iimg::IBitmap::PF_RGB){
				const qint8* rawData = (qint8*)bitmap.GetLinePtr(0);
				Q_ASSERT(rawData != NULL);
				quint32& bitmapPattern = (quint32&)rawData[0];
				double& value = (double&)rawData[sizeof(s_resolutionPattern)];

				if (bitmapPattern == s_resolutionPattern && !qIsNaN(value) && (value > 0)){
					resolutionX = resolutionY = value;

					return true;
				}
			}
			else{
				const quint8* rawData = (quint8*)bitmap.GetLinePtr(0);
				quint32 bitmapPattern = rawData[1] << 24;
				bitmapPattern |= (rawData[2] & 0xff) << 16; 
				bitmapPattern |= (rawData[5] & 0xff) << 8; 
				bitmapPattern |= rawData[6] & 0xff;

				if (bitmapPattern == s_resolutionPattern){
					union{
						quint8 data[8];
						double value;
					} res;
					res.value = 0;

					res.data[0]= rawData[9];
					res.data[1]= rawData[10];
					res.data[2]= rawData[13];
					res.data[3]= rawData[14];
					res.data[4]= rawData[17];
					res.data[5]= rawData[18];
					res.data[6]= rawData[21];
					res.data[7]= rawData[22];

					if (!qIsNaN(res.value) && (res.value > 0)){
						resolutionX = resolutionY = res.value;

						return true;
					}
				}
			}
		}
	}

	return false;
}


bool CCalibratedCameraComp::WriteImageResolution(iimg::IBitmap& bitmap, double resolutionX, double resolutionY)
{
	istd::CIndex2d imageSize = bitmap.GetImageSize();
	if (!imageSize.IsSizeEmpty()){
		// write new pattern
		if (imageSize.GetX() >= int(2 * sizeof(double) + sizeof(s_resolutionPatternXY)))
		{
			quint8* rawData = (quint8*)bitmap.GetLinePtr(0);
			Q_ASSERT(rawData != NULL);

			qint32& bitmapPattern = (qint32&)rawData[0];
			double& value1 = (double&)rawData[sizeof(s_resolutionPatternXY)];
			double& value2 = (double&)rawData[sizeof(s_resolutionPatternXY) + sizeof(double)];

			bitmapPattern = qint32(s_resolutionPatternXY);
			value1 = resolutionX;
			value2 = resolutionY;

			return true;
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
	if (retVal == TS_OK)
	{
		iprm::TParamsPtr<imeas::INumericValue> scaleParamPtr(paramsPtr, *m_scaleParamIdAttrPtr);
		if (scaleParamPtr.IsValid()){
			iimg::IBitmap* bitmapPtr = dynamic_cast<iimg::IBitmap*>(outputPtr);
			if (bitmapPtr != NULL){
				double targetResolutionX = 1.0;
				double targetResolutionY = 1.0;
				if (!ReadImageResolution(*bitmapPtr, targetResolutionX, targetResolutionY)){
					imath::CVarVector scaleValues = scaleParamPtr->GetValues();
					if (scaleValues.GetElementsCount() >= 2){
						targetResolutionX = scaleValues[0];
						targetResolutionY = scaleValues[1];
					}

					WriteImageResolution(*bitmapPtr, targetResolutionX, targetResolutionY);
				}
			}
		}
	}

	return retVal;
}


} // namespace icam


