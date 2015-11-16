#ifndef ihotf_IFileSystemChangeStorage_included
#define ihotf_IFileSystemChangeStorage_included


// ACF includes
#include "istd/IChangeable.h"
#include <QtCore/QString>


namespace ihotf
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
		CF_NEW = 0x2363d50,

		/**
			File(s) were removed.
		*/
		CF_REMOVED,

		/**
			File(s) were modified.
		*/
		CF_MODIFIED,

		/**
			File's attributes were changed.
		*/
		CF_ATTRIBUTE_CHANGED,

		/**
			All data was clear.
		*/
		CF_DATA_RESET
	};

	/**
		Get number of items in the storage.
	*/
	virtual int GetStorageItemsCount() const = 0;

	/**
		Get item path with the given index from the storage.
	*/
	virtual QString GetItemPath(int fileIndex) const = 0;

	/**
		Get item state with the given index from the storage.
	*/
	virtual int GetItemState(int fileIndex) const = 0;

	/**
		Update storage item. If the item not exists, new one will be created and added to the storage.
		If the item is marked as "removed" it will not deleted from the storage.
		Use RemoveItemStorage, if you want permanently remove the item from the storage.
	*/
	virtual void UpdateStorageItem(const QString& path, int changeFlag) = 0;

	/**
		Remove the item from the storage.
	*/
	virtual void RemoveStorageItem(int itemIndex) = 0;

	/**
		Reset the storage.
	*/
	virtual void ResetStorage() = 0;
};


} // namespace ihotf


#endif // !ihotf_IFileSystemChangeStorage_included


