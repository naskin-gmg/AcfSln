#ifndef ifpf_IHotfolder_included
#define ifpf_IHotfolder_included


// ACF includes
#include "iser/ISerializable.h"

#include "iprm/IParamsSet.h"


// AcfSln includes
#include "ifpf/IHotfolderProcessingItem.h"


namespace ifpf
{


/**
	Interface for a hotfolder's processing data model.
*/
class IHotfolderProcessingInfo: virtual public iser::ISerializable
{
public:
	enum ChangeFlags
	{
		CF_FILE_ADDED = 0x100000,
		CF_FILE_REMOVED = 0x200000,
		CF_WORKING_STATE_CHANGED = 0x400000,
		CF_CREATE = 0x800000
	};

	/**
		Add new processing item to hotfolder's state model.
		If the output file name is not set, the file name will be calculated automatically.
	*/
	virtual const ifpf::IHotfolderProcessingItem* AddProcessingItem(
				const istd::CString& inputFilePath,
				const istd::CString& outputFilePath = istd::CString()) = 0;
	
	/**
		Remove existing processing item from the hotfolder's state model.
	*/
	virtual void RemoveProcessingItem(ifpf::IHotfolderProcessingItem* fileItemPtr) = 0;
		
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
};


} // namespace ifpf


#endif // !ifpf_IHotfolder_included


