#include "iedge/CProcessedEdgeLinesSupplierComp.h"


namespace iedge
{


// reimplemented (iedge::IEdgeLinesProvider)

const CEdgeLine::Container* CProcessedEdgeLinesSupplierComp::GetEdgesContainer() const
{
	return GetWorkProduct();
}


// protected methods

// reimplemented (iproc::TSupplierCompWrap)

int CProcessedEdgeLinesSupplierComp::ProduceObject(CEdgeLine::Container& result) const
{
	if (m_edgeLinesProviderCompPtr.IsValid() && m_edgesProcessorCompPtr.IsValid()){
		const CEdgeLine::Container* containerPtr = m_edgeLinesProviderCompPtr->GetEdgesContainer();
		if (		(containerPtr != NULL) &&
					m_edgesProcessorCompPtr->DoLinesProcessing(GetModelParametersSet(), *containerPtr, result)){
			return WS_OK;
		}
		else{
			return WS_ERROR;
		}
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
		RegisterSupplierInput(m_edgeLinesProviderModelCompPtr.GetPtr());
	}
}


} // namespace iedge


