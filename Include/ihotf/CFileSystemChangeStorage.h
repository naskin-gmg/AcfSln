#ifndef ihotf_CFileSystemChangeStorage_included
#define ihotf_CFileSystemChangeStorage_included


// Qt includes
#include <QtCore/QVector>
#include <QtCore/QMutex>

// ACF includes
#include "istd/TSmartPtr.h"

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
	// reimplemented (ihotf::IFileSystemChangeStorage)
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

	typedef QVector<FileItem> FileItems;
	FileItems m_storageItems;

	mutable QMutex m_mutex;
};


} // namespace ihotf


#endif // !ihotf_CFileSystemChangeStorage_included

