#include "iedge/CProcessedEdgeLinesSupplierComp.h"


namespace iedge
{


// reimplemented (iedge::IEdgeLinesProvider)

const CEdgeLineContainer* CProcessedEdgeLinesSupplierComp::GetEdgesContainer() const
{
	return GetWorkProduct();
}


// protected methods

// reimplemented (iproc::TSupplierCompWrap)

int CProcessedEdgeLinesSupplierComp::ProduceObject(CEdgeLineContainer& result) const
{
	if (m_edgeLinesProviderCompPtr.IsValid() && m_edgesProcessorCompPtr.IsValid()){
		const CEdgeLineContainer* containerPtr = m_edgeLinesProviderCompPtr->GetEdgesContainer();

		if (containerPtr != NULL){
			Timer performanceTimer(this, "Edge processing");

			if (m_edgesProcessorCompPtr->DoLinesProcessing(GetModelParametersSet(), *containerPtr, result)){
				result.SetCalibration(containerPtr->GetCalibration());

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

void CProcessedEdgeLinesSupplierComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	if (m_edgeLinesProviderModelCompPtr.IsValid()){
		RegisterSupplierInput(m_edgeLinesProviderModelCompPtr.GetPtr(), m_edgeLinesSupplierCompPtr.GetPtr());
	}

	// Force components initialization
	m_edgeLinesProviderCompPtr.IsValid();
	m_edgesProcessorCompPtr.IsValid();
}


} // namespace iedge


