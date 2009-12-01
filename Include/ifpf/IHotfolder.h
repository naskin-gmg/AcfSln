#ifndef ifpf_IHotfolder_included
#define ifpf_IHotfolder_included


// ACF includes
#include "iprm/IParamsSet.h"

// AcfSln includes
#include "ifpf/IHotfolderProcessingItem.h"


namespace ifpf
{


/**
	Interface for a hotfolder's dynamic model.
*/
class IHotfolder: virtual public istd::IChangeable
{
public:
	enum ChangeFlags
	{
		CF_FILE_ADDED = 0x100000,
		CF_FILE_REMOVED = 0x200000
	};
	
	/**
		Get the count of processing items in the hotfolder.
	*/
	virtual int GetProcessingItemsCount() const = 0;

	/**
		Get the processing item with the index \c processingItemIndex.
	*/
	virtual ifpf::IHotfolderProcessingItem* GetProcessingItem(int processingItemIndex) const = 0;

	/**
		Get the working state of the hotfolder.
	*/
	virtual bool IsWorking() const = 0;

	/**
		If \c working is \c true, the hotfolder processing will be started or stopped otherwise.
	*/
	virtual void SetWorking(bool working = true) = 0;

	/**
		Get the hotfolder's parameters
	*/
	virtual iprm::IParamsSet* GetHotfolderParams() const = 0;
};


} // namespace ifpf


#endif // !ifpf_IHotfolder_included


