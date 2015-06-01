#include "iedge/CProcessedEdgeLinesSupplierComp.h"


namespace iedge
{


// reimplemented (iedge::IEdgeLinesProvider)

const CEdgeLineContainer* CProcessedEdgeLinesSupplierComp::GetEdgesContainer() const
{
	return GetWorkProduct();
}


// protected methods

// reimplemented (iinsp::TSupplierCompWrap)

int CProcessedEdgeLinesSupplierComp::ProduceObject(CEdgeLineContainer& result) const
{
	if (m_edgeLinesProviderCompPtr.IsValid() && m_edgesProcessorCompPtr.IsValid()){
		const CEdgeLineContainer* containerPtr = m_edgeLinesProviderCompPtr->GetEdgesContainer();

		if (containerPtr != NULL){
			Timer performanceTimer(this, "Edge processing");

			if (m_edgesProcessorCompPtr->DoLinesProcessing(GetModelParametersSet(), *containerPtr, result)){
				istd::TDelPtr<i2d::ICalibration2d> newCalibration;

				// copy calibration from original
				const i2d::ICalibration2d* calibrationPtr = containerPtr->GetCalibration();
				if (calibrationPtr != NULL){
					newCalibration.SetCastedOrRemove(calibrationPtr->CloneMe());
				}

				result.SetCalibration(newCalibration.PopPtr(), true);

				return WS_OK;
			}
		}

		return WS_ERROR;
	}

	SendCriticalMessage(0, "Bad component architecture, 'EdgeLinesProvider' or 'EdgesProcessor' component references are not set");

	return WS_CRITICAL;
}


// reimplemented (icomp::CComponentBase)

void CProcessedEdgeLinesSupplierComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	if (m_edgeLinesProviderModelCompPtr.IsValid()){
		RegisterSupplierInput(m_edgeLinesProviderModelCompPtr.GetPtr(), m_edgeLinesSupplierCompPtr.GetPtr());
	}

	// Force components initialization
	m_edgeLinesProviderCompPtr.EnsureInitialized();
	m_edgesProcessorCompPtr.EnsureInitialized();
}


} // namespace iedge


