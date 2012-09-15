#include "iipr/CTubeProjectionLinesProviderComp.h"


// ACF includes
#include "iprm/TParamsPtr.h"
#include "imeas/INumericValue.h"
#include "i2d/CTubeProjectionsGenerator.h"


namespace iipr
{


// reimplemented (iipr::CMultiLineSupplierCompBase)

int CTubeProjectionLinesProviderComp::ProduceObject(ProductType& result) const
{
	result.clear();

	iprm::IParamsSet* paramsPtr = GetModelParametersSet();
	if (paramsPtr == NULL){
		return WS_CRITICAL;
	}

	iprm::TParamsPtr<i2d::CTubePolyline> tubeRegionPtr(paramsPtr, *m_tubeParamsIdAttrPtr);
	if (tubeRegionPtr.IsValid()){
		int projectionsCount = 2;
		iprm::TParamsPtr<imeas::INumericValue> linesCountPtr(paramsPtr, *m_linesCountParamsIdAttrPtr);
		if (linesCountPtr.IsValid()){
			const imath::CVarVector values = linesCountPtr->GetValues();
			if (values.GetElementsCount() > 0){
				projectionsCount = qMax<int>(2, (int)values.GetElement(0));
			}
		}
		
		return i2d::CTubeProjectionsGenerator::GeneratorProjections(*tubeRegionPtr, projectionsCount, result) ? WS_OK : WS_ERROR;
	}

	return WS_CRITICAL;
}


} // namespace iipr


