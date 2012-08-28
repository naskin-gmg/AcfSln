#ifndef iedge_CProcessedEdgeLinesSupplierComp_included
#define iedge_CProcessedEdgeLinesSupplierComp_included


// ACF includes
#include "iproc/TSupplierCompWrap.h"

// ACF-Solutions includes
#include "iedge/IEdgeLinesProvider.h"
#include "iedge/IEdgeLinesProcessor.h"
#include "iedge/CEdgeLine.h"


namespace iedge
{


class CProcessedEdgeLinesSupplierComp:
			public iproc::TSupplierCompWrap<CEdgeLine::Container>,
			virtual public IEdgeLinesProvider
{
public:
	typedef iproc::TSupplierCompWrap<CEdgeLine::Container> BaseClass;

	I_BEGIN_COMPONENT(CProcessedEdgeLinesSupplierComp);
		I_REGISTER_INTERFACE(IEdgeLinesProvider);
		I_ASSIGN(m_edgeLinesProviderCompPtr, "EdgeLinesProvider", "Provide list of edge list to processing", true, "EdgeLinesProvider");
		I_ASSIGN_TO(m_edgeLinesProviderModelCompPtr, m_edgeLinesProviderCompPtr, false);
		I_ASSIGN(m_edgesProcessorCompPtr, "EdgesProcessor", "Processor (edges to edges)", true, "EdgesProcessor");
	I_END_COMPONENT;

	// reimplemented (iedge::IEdgeLinesProvider)
	virtual const CEdgeLine::Container* GetEdgesContainer() const;

protected:
	// reimplemented (iproc::TSupplierCompWrap)
	virtual int ProduceObject(CEdgeLine::Container& result) const;

	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated();

private:
	I_REF(IEdgeLinesProvider, m_edgeLinesProviderCompPtr);
	I_REF(imod::IModel, m_edgeLinesProviderModelCompPtr);
	I_REF(IEdgeLinesProcessor, m_edgesProcessorCompPtr);
};


} // namespace iedge


#endif // !iedge_CProcessedEdgeLinesSupplierComp_included


