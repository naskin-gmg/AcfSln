#ifndef ifpf_IHotfolderTaskManager_included
#define ifpf_IHotfolderTaskManager_included


// ACF includes
#include "istd/IPolymorphic.h"


// AcfSln includes
#include "ifpf/IHotfolderProcessingItem.h"


namespace ifpf
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
	virtual ifpf::IHotfolderProcessingItem* GetNextProcessingTask() const = 0;
};


} // namespace ifpf


#endif // !ifpf_IHotfolderTaskManager_included


