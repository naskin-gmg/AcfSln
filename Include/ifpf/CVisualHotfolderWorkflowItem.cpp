#include "ifpf/CVisualHotfolderWorkflowItem.h"


namespace ifpf
{


// public methods

// reimplemented (iser::ISerializable)

bool CVisualHotfolderWorkflowItem::Serialize(iser::IArchive& archive)
{
	return BaseClass2::Serialize(archive);
}


} // namespace ifpf



