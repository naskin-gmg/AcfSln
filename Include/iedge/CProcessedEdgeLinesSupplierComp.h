#ifndef iedge_CProcessedEdgeLinesSupplierComp_included
#define iedge_CProcessedEdgeLinesSupplierComp_included


// ACF includes
#include "iinsp/ISupplier.h"
#include "iinsp/TSupplierCompWrap.h"
#include "iedge/IEdgeLinesProvider.h"
#include "iedge/IEdgeLinesProcessor.h"
#include "iedge/CEdgeLineContainer.h"


namespace iedge
{


class CProcessedEdgeLinesSupplierComp:
			public iinsp::TSupplierCompWrap<CEdgeLineContainer>,
			virtual public IEdgeLinesProvider
{
public:
	typedef iinsp::TSupplierCompWrap<CEdgeLineContainer> BaseClass;

	I_BEGIN_COMPONENT(CProcessedEdgeLinesSupplierComp);
		I_REGISTER_INTERFACE(IEdgeLinesProvider);
		I_ASSIGN(m_edgeLinesProviderCompPtr, "EdgeLinesProvider", "Provide list of edge list to processing", true, "EdgeLinesProvider");
		I_ASSIGN_TO(m_edgeLinesSupplierCompPtr, m_edgeLinesProviderCompPtr, false);
		I_ASSIGN_TO(m_edgeLinesProviderModelCompPtr, m_edgeLinesProviderCompPtr, false);
		I_ASSIGN(m_edgesProcessorCompPtr, "EdgesProcessor", "Processor (edges to edges)", true, "EdgesProcessor");
	I_END_COMPONENT;

	// reimplemented (iedge::IEdgeLinesProvider)
	virtual const CEdgeLineContainer* GetEdgesContainer() const;

protected:
	// reimplemented (iinsp::TSupplierCompWrap)
	virtual int ProduceObject(CEdgeLineContainer& result) const;

	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated();

private:
	I_REF(IEdgeLinesProvider, m_edgeLinesProviderCompPtr);
	I_REF(iinsp::ISupplier, m_edgeLinesSupplierCompPtr);
	I_REF(imod::IModel, m_edgeLinesProviderModelCompPtr);
	I_REF(IEdgeLinesProcessor, m_edgesProcessorCompPtr);
};


} // namespace iedge


#endif // !iedge_CProcessedEdgeLinesSupplierComp_included


