// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#include <iedge/CExtractedEdgeLinesSupplierComp.h>


namespace iedge
{


// reimplemented (iedge::IEdgeLinesProvider)

const CEdgeLineContainer* CExtractedEdgeLinesSupplierComp::GetEdgesContainer() const
{
	return GetWorkProduct();
}


// protected methods

// reimplemented (iinsp::TSupplierCompWrap)

iinsp::ISupplier::WorkStatus CExtractedEdgeLinesSupplierComp::ProduceObject(CEdgeLineContainer& result) const
{
	result.Reset();

	if (!m_bitmapProviderCompPtr.IsValid()){
		SendCriticalMessage(0, "Bad component architecture, 'BitmapProvider' component reference is not set");

		return WS_FAILED;
	}

	if (!m_edgesExtractorCompPtr.IsValid()){
		SendCriticalMessage(0, "Bad component architecture, 'EdgesExtractor' component reference is not set");

		return WS_FAILED;
	}

	const iimg::IBitmap* bitmapPtr = m_bitmapProviderCompPtr->GetBitmap();
	if (bitmapPtr != NULL){
		Timer performanceTimer(this, "Edge extraction");
		Q_UNUSED(performanceTimer);

		if (m_edgesExtractorCompPtr->DoContourExtraction(GetModelParametersSet(), *bitmapPtr, result)){
			const i2d::ICalibration2d* bitmapCalibrationPtr = bitmapPtr->GetCalibration();
			if (bitmapCalibrationPtr != NULL){
				result.Transform(*bitmapCalibrationPtr);	// move to global coordinates
			}

			if (m_calibrationProviderCompPtr.IsValid()){
				const i2d::ICalibration2d* calibrationPtr = m_calibrationProviderCompPtr->GetCalibration();

				if (calibrationPtr != NULL){
					istd::TUniqueInterfacePtr<i2d::ICalibration2d> calibCopyPtr;
					calibCopyPtr.MoveCastedPtr(calibrationPtr->CloneMe());

					if (calibCopyPtr.IsValid()){
						result.InvTransform(*calibrationPtr);
						result.SetCalibration(calibCopyPtr.PopInterfacePtr(), true);
					}
				}
			}

			return WS_OK;
		}
	}

	return WS_FAILED;
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
	m_bitmapProviderCompPtr.EnsureInitialized();
	m_edgesExtractorCompPtr.EnsureInitialized();
}


} // namespace iedge


