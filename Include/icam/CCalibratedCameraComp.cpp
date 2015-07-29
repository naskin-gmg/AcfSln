#include "icam/CCalibratedCameraComp.h"


// ACF includes
#include "iprm/TParamsPtr.h"
#include "imeas/INumericValue.h"


namespace icam
{


// public static methods

bool CCalibratedCameraComp::ReadImageResolution(const iimg::IBitmap& bitmap, double& resolution)
{
	istd::CIndex2d imageSize = bitmap.GetImageSize();
	if (!imageSize.IsSizeEmpty()){
		if (imageSize.GetX() >= sizeof(double) + sizeof(RESOLUTION_PATTERN)){
			const qint8* rawData = (qint8*)bitmap.GetLinePtr(0);
			Q_ASSERT(rawData != NULL);	// Becouse bitmap is not empty.
			qint32& bitmapPattern = (qint32&)rawData[0];
			double& value = (double&)rawData[sizeof(RESOLUTION_PATTERN)];

			if (bitmapPattern == RESOLUTION_PATTERN && !_isnan(value) && (value > 0)){
				resolution = value;

				return true;
			}
		}
	}

	return false;
}


bool CCalibratedCameraComp::WriteImageResolution(iimg::IBitmap& bitmap, double resolution)
{
	istd::CIndex2d imageSize = bitmap.GetImageSize();
	if (!imageSize.IsSizeEmpty()){
		if (imageSize.GetX() >= sizeof(double) + sizeof(RESOLUTION_PATTERN)){
			const qint8* rawData = (qint8*)bitmap.GetLinePtr(0);
			Q_ASSERT(rawData != NULL);	// Becouse bitmap is not empty.
			qint32& bitmapPattern = (qint32&)rawData[0];
			double& bitmapCalib = (double&)rawData[sizeof(RESOLUTION_PATTERN)];

			bitmapPattern = qint32(RESOLUTION_PATTERN);
			bitmapCalib = resolution;

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


