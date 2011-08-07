#ifndef ifpf_IFileSystemChangeStorage_included
#define ifpf_IFileSystemChangeStorage_included


// ACF includes
#include "istd/IChangeable.h"
#include "istd/CString.h"


namespace ifpf
{


/**
	Common interface for a storage of files.
*/
class IFileSystemChangeStorage: virtual public istd::IChangeable
{
public:
	/**
		File system change flags.
	*/
	enum ChangeFlags
	{
		/**
			File(s) were added.
		*/
		CF_NEW = 0x1000000,

		/**
			File(s) were removed.
		*/
		CF_REMOVED = 0x2000000,

		/**
			File(s) were modified.
		*/
		CF_MODIFIED = 0x4000000,

		/**
			File's attributes were changed.
		*/
		CF_ATTRIBUTE_CHANGED = 0x8000000,

		/**
			All possible changes
		*/
		CF_SOME_CHANGES = CF_NEW | CF_REMOVED | CF_MODIFIED | CF_ATTRIBUTE_CHANGED
	};

	/**
		Get number of items in the storage.
	*/
	virtual int GetStorageItemsCount() const = 0;

	/**
		Get item path with the given index from the storage.
	*/
	virtual istd::CString GetItemPath(int fileIndex) const = 0;

	/**
		Get item state with the given index from the storage.
	*/
	virtual int GetItemState(int fileIndex) const = 0;

	/**
		Update storage item. If the item not exists, new one will be created and added to the storage.
		If the item is marked as "removed" it will not deleted from the storage.
		Use RemoveItemStorage, if you want permanently remove the item from the storage.
	*/
	virtual void UpdateStorageItem(const istd::CString& path, int itemFlags) = 0;

	/**
		Reset the storage.
	*/
	virtual void ResetStorage() = 0;
};


} // namespace ifpf


#endif // !ifpf_IFileSystemChangeStorage_included


