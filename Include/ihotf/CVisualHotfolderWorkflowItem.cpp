#include "ihotf/CVisualHotfolderWorkflowItem.h"


namespace ihotf
{


// public methods

// reimplemented (iser::ISerializable)

bool CVisualHotfolderWorkflowItem::Serialize(iser::IArchive& archive)
{
	return BaseClass2::Serialize(archive);
}


} // namespace ihotf



