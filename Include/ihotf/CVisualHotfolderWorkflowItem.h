#ifndef ihotf_CVisualHotfolderWorkflowItem_included
#define ihotf_CVisualHotfolderWorkflowItem_included


// ACF includes
#include "i2d/CPosition2d.h"


// AcfSln includes
#include "ihotf/CHotfolderWorkflowItem.h"


namespace ihotf
{


/**
	Implementation of the dynamic data model of the hotfolder.
*/
class CVisualHotfolderWorkflowItem:
			public ihotf::CHotfolderWorkflowItem,
			public i2d::CPosition2d
{
public:
	typedef ihotf::CHotfolderWorkflowItem BaseClass;
	typedef i2d::CPosition2d BaseClass2;

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);
};


} // namespace ihotf


#endif // !ihotf_CVisualHotfolderWorkflowItem_included

