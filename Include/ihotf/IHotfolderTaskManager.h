#ifndef ihotf_IHotfolderTaskManager_included
#define ihotf_IHotfolderTaskManager_included


// ACF includes
#include "istd/IPolymorphic.h"


// AcfSln includes
#include "ihotf/IHotfolderProcessingItem.h"


namespace ihotf
{


/**
	Common interface for a hotfolder processing trask mananger.
*/
class IHotfolderTaskManager: virtual public istd::IPolymorphic
{
public:
	/**
		Get the next processing task, if available, other
	*/
	virtual ihotf::IHotfolderProcessingItem* GetNextProcessingTask() const = 0;
};


} // namespace ihotf


#endif // !ihotf_IHotfolderTaskManager_included


