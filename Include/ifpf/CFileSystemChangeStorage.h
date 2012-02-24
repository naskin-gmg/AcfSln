#ifndef ifpf_CFileSystemChangeStorage_included
#define ifpf_CFileSystemChangeStorage_included


// STL includes
#include <vector>


// ACF includes
#include "istd/TSmartPtr.h"

#include "isys/ICriticalSection.h"


// AcfSln includes
#include "ifpf/IFileSystemChangeStorage.h"


namespace ifpf
{


/**
	Implementation of the file storage.
	All public methods in this component are thread-safe immplemented.
*/
class CFileSystemChangeStorage: virtual public ifpf::IFileSystemChangeStorage
{
public:
	CFileSystemChangeStorage();

	// reimplemented (ifpf::IFileSystemChangeStorage)
	virtual int	 GetStorageItemsCount() const;
	virtual QString GetItemPath(int fileIndex) const;
	virtual int GetItemState(int fileIndex) const;
	virtual void UpdateStorageItem(const QString& path, int itemFlags);
	virtual void ResetStorage();

private:
	int GetFileIndexFromPath(const QString& filePath) const;

private:
	struct FileItem
	{
		QString path;
		int state;
	};

	typedef std::vector<FileItem> FileItems;
	FileItems m_storageItems;

	istd::TSmartPtr<isys::ICriticalSection> m_lockPtr;
};


} // namespace ifpf


#endif // !ifpf_CFileSystemChangeStorage_included

