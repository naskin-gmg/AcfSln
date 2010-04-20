#ifndef ifpf_CVisualHotfolderWorkflowItem_included
#define ifpf_CVisualHotfolderWorkflowItem_included


// ACF includes
#include "i2d/CPosition2d.h"


// AcfSln includes
#include "ifpf/CHotfolderWorkflowItem.h"


namespace ifpf
{


/**
	Implementation of the dynamic data model of the hotfolder.
*/
class CVisualHotfolderWorkflowItem: public ifpf::CHotfolderWorkflowItem, virtual public i2d::CPosition2d
{
public:
	typedef ifpf::CHotfolderWorkflowItem BaseClass;
	typedef i2d::CPosition2d BaseClass2;
};


} // namespace ifpf


#endif // !ifpf_CVisualHotfolderWorkflowItem_included

