#include "iipr/CImageRegionProcessorCompBase.h"


 // ACF includes
#include "istd/TChangeNotifier.h"
#include "ibase/CSize.h"
#include "iimg/CBitmapRegion.h"
#include "i2d/IObject2d.h"
#include "i2d/ICalibrationProvider.h"

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
		return TS_INVALID;
	}

	const i2d::IObject2d* aoiPtr = NULL;
	if (paramsPtr != NULL && m_aoiParamIdAttrPtr.IsValid()){
		aoiPtr = dynamic_cast<const i2d::IObject2d*>(paramsPtr->GetParameter(*m_aoiParamIdAttrPtr));
	}

	istd::TDelPtr<i2d::IObject2d> transformedRegionPtr;

	if (aoiPtr != NULL && m_regionCalibrationProviderCompPtr.IsValid()){
		const i2d::ITransformation2d* logicalToPixelTransformPtr = m_regionCalibrationProviderCompPtr->GetCalibration();
		if (logicalToPixelTransformPtr != NULL){
			transformedRegionPtr.SetCastedOrRemove<istd::IChangeable>(aoiPtr->CloneMe());

			if (transformedRegionPtr.IsValid()){
				if (!transformedRegionPtr->Transform(*logicalToPixelTransformPtr)){
					return TS_INVALID;
				}

				aoiPtr = transformedRegionPtr.GetPtr();
			}
		}
	}

	return ProcessImageRegion(*inputBitmapPtr, paramsPtr, aoiPtr, outputPtr) ? TS_OK : TS_INVALID;
}


} // namespace iipr


