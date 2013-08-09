#include "iedge/CExtractedEdgeLinesSupplierComp.h"


namespace iedge
{


// reimplemented (iedge::IEdgeLinesProvider)

const CEdgeLineContainer* CExtractedEdgeLinesSupplierComp::GetEdgesContainer() const
{
	return GetWorkProduct();
}


// protected methods

// reimplemented (iproc::TSupplierCompWrap)

int CExtractedEdgeLinesSupplierComp::ProduceObject(CEdgeLineContainer& result) const
{
	result.Reset();

	if (m_bitmapProviderCompPtr.IsValid() && m_edgesExtractorCompPtr.IsValid()){
		const iimg::IBitmap* bitmapPtr = m_bitmapProviderCompPtr->GetBitmap();
		if (bitmapPtr != NULL){
			Timer performanceTimer(this, "Edge extraction");

			if (m_edgesExtractorCompPtr->DoContourExtraction(GetModelParametersSet(), *bitmapPtr, result)){
				const i2d::ICalibration2d* bitmapCalibrationPtr = bitmapPtr->GetCalibration();
				if (bitmapCalibrationPtr != NULL){
					result.Transform(*bitmapCalibrationPtr);	// move to global coordinates
				}

				if (m_calibrationProviderCompPtr.IsValid()){
					const i2d::ICalibration2d* calibrationPtr = m_calibrationProviderCompPtr->GetCalibration();

					if (calibrationPtr != NULL){
						result.InvTransform(*calibrationPtr);
						result.SetCalibration(calibrationPtr);
					}
				}

				return WS_OK;
			}
		}

		return WS_ERROR;
	}
	else{
		return WS_CRITICAL;
	}
}


// reimplemented (icomp::CComponentBase)

void CExtractedEdgeLinesSupplierComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	if (m_bitmapProviderModelCompPtr.IsValid()){
		RegisterSupplierInput(m_bitmapProviderModelCompPtr.GetPtr(), m_bitmapSupplierCompPtr.GetPtr());
	}

	if (m_calibrationProviderModelCompPtr.IsValid()){
		RegisterSupplierInput(m_calibrationProviderModelCompPtr.GetPtr(), m_calibrationSupplierCompPtr.GetPtr());
	}

	// Force components initialization
	m_bitmapProviderCompPtr.IsValid();
	m_edgesExtractorCompPtr.IsValid();
}


} // namespace iedge


