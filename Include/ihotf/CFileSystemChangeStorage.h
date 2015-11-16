#ifndef ihotf_CFileSystemChangeStorage_included
#define ihotf_CFileSystemChangeStorage_included


// Qt includes
#include <QtCore/QList>
#include <QtCore/QMutex>

// AcfSln includes
#include "ihotf/IFileSystemChangeStorage.h"


namespace ihotf
{


/**
	Implementation of the file storage.
	All public methods in this component are thread-safe immplemented.
*/
class CFileSystemChangeStorage: virtual public ihotf::IFileSystemChangeStorage
{
public:
	CFileSystemChangeStorage();

	// reimplemented (ihotf::IFileSystemChangeStorage)
	virtual int	 GetStorageItemsCount() const;
	virtual QString GetItemPath(int fileIndex) const;
	virtual int GetItemState(int fileIndex) const;
	virtual void UpdateStorageItem(const QString& path, int changeFlag);
	virtual void RemoveStorageItem(int itemIndex);
	virtual void ResetStorage();

private:
	int GetFileIndexFromPath(const QString& filePath) const;

private:
	struct FileItem
	{
		FileItem()
			:state(0)
		{	
		}
		
		QString path;
		int state;
	};

	typedef QList<FileItem> FileItems;
	FileItems m_storageItems;

	mutable QMutex m_mutex;
};


} // namespace ihotf


#endif // !ihotf_CFileSystemChangeStorage_included

