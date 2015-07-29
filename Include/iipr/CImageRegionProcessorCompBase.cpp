#include "iipr/CImageRegionProcessorCompBase.h"


 // ACF includes
#include "istd/CChangeNotifier.h"
#include "ibase/CSize.h"
#include "i2d/IObject2d.h"
#include "i2d/ICalibrationProvider.h"
#include "iimg/IBitmap.h"
#include "iprm/TParamsPtr.h"


namespace iipr
{


// public methods

// reimplemented (iproc::IProcessor)

int CImageRegionProcessorCompBase::DoProcessing(
				const iprm::IParamsSet* paramsPtr,
				const istd::IPolymorphic* inputPtr,
				istd::IChangeable* outputPtr,
				ibase::IProgressManager* /*progressManagerPtr*/)
{
	const iimg::IBitmap* inputBitmapPtr = dynamic_cast<const iimg::IBitmap*>(inputPtr);
	if (inputBitmapPtr == NULL){
		SendErrorMessage(0, "Processor input is not a bitmap");

		return TS_INVALID;
	}

	i2d::CRectangle inputImageArea(inputBitmapPtr->GetImageSize());

	iprm::TParamsPtr<i2d::IObject2d> aoiPtr(paramsPtr, *m_aoiParamIdAttrPtr);

	istd::TDelPtr<i2d::IObject2d> transformedRegionPtr;

	if (aoiPtr.IsValid()){
		const i2d::ICalibration2d* logToPhysicalTransformPtr = aoiPtr->GetCalibration();

		if (logToPhysicalTransformPtr != NULL){
			transformedRegionPtr.SetCastedOrRemove<istd::IChangeable>(aoiPtr->CloneMe());
			if (transformedRegionPtr.IsValid()){
				const i2d::ICalibration2d* calibrationPtr = transformedRegionPtr->GetCalibration();
				if (calibrationPtr != NULL){
					transformedRegionPtr->Transform(*calibrationPtr);

					transformedRegionPtr->SetCalibration(NULL);
				}

				aoiPtr.SetPtr(transformedRegionPtr.GetPtr());
			}
		}
	}
	else{
		if (*m_allowEmptyRegionAttrPtr){
			aoiPtr.SetPtr(&inputImageArea);			
		}
	}

	return ProcessImageRegion(*inputBitmapPtr, paramsPtr, aoiPtr.GetPtr(), outputPtr) ? TS_OK : TS_INVALID;
}


} // namespace iipr


