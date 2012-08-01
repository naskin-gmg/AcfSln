#include "iipr/CImageRegionProcessorCompBase.h"


// Qt includes
#include <QStringList>

 // ACF includes
#include "istd/TChangeNotifier.h"
#include "ibase/CSize.h"
#include "i2d/IObject2d.h"
#include "i2d/ICalibrationProvider.h"
#include "iimg/IBitmap.h"

// IACF includes
#include "iipr/TImagePixelInterpolator.h"


namespace iipr
{


// public methods

// reimplemented (iproc::IProcessor)

int CImageRegionProcessorCompBase::DoProcessing(
				const iprm::IParamsSet* paramsPtr,
				const istd::IPolymorphic* inputPtr,
				istd::IChangeable* outputPtr,
				iproc::IProgressManager* /*progressManagerPtr*/)
{
	const iimg::IBitmap* inputBitmapPtr = dynamic_cast<const iimg::IBitmap*>(inputPtr);
	if (inputBitmapPtr == NULL){
		SendErrorMessage(0, "Processor input is not a bitmap");

		return TS_INVALID;
	}

	const i2d::IObject2d* aoiPtr = NULL;
	if (paramsPtr != NULL && m_aoiParamIdAttrPtr.IsValid()){
		aoiPtr = dynamic_cast<const i2d::IObject2d*>(paramsPtr->GetParameter(*m_aoiParamIdAttrPtr));
		if (aoiPtr == NULL){
			iprm::IParamsSet::Ids existingParamIds = paramsPtr->GetParamIds();
			QStringList existingIds;
			for (iprm::IParamsSet::Ids::ConstIterator index = existingParamIds.constBegin(); index != existingParamIds.constEnd(); index++){
				existingIds.push_back(*index);
			}

			QString idList = existingIds.join(", ");

			SendVerboseMessage(QString("Parameter %1 was not found in the parameter set. Following parameter IDs are registered: %2").arg(QString(*m_aoiParamIdAttrPtr)).arg(idList));
		}
	}

	istd::TDelPtr<i2d::IObject2d> transformedRegionPtr;

	if (aoiPtr != NULL){
		const i2d::ITransformation2d* pixelToLogicalTransformPtr = NULL;
		if (m_regionCalibrationProviderCompPtr.IsValid()){
			pixelToLogicalTransformPtr = m_regionCalibrationProviderCompPtr->GetCalibration();
		}
		else{
			// Try to get calibration direct from bitmap:
			const i2d::ICalibrationProvider* bitmapCalibrationpProviderPtr = dynamic_cast<const i2d::ICalibrationProvider* >(inputPtr);
			if (bitmapCalibrationpProviderPtr != NULL){
				pixelToLogicalTransformPtr = bitmapCalibrationpProviderPtr->GetCalibration();
			}
		}

		if (pixelToLogicalTransformPtr != NULL){
			transformedRegionPtr.SetCastedOrRemove<istd::IChangeable>(aoiPtr->CloneMe());

			if (transformedRegionPtr.IsValid()){
				if (!transformedRegionPtr->InvTransform(*pixelToLogicalTransformPtr)){
					SendErrorMessage(0, "2D-transformation of the processing region failed");

					return TS_INVALID;
				}

				aoiPtr = transformedRegionPtr.GetPtr();
			}
		}
	}

	return ProcessImageRegion(*inputBitmapPtr, paramsPtr, aoiPtr, outputPtr) ? TS_OK : TS_INVALID;
}


} // namespace iipr


