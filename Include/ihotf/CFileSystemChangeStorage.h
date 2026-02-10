// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#ifndef ihotf_CFileSystemChangeStorage_included
#define ihotf_CFileSystemChangeStorage_included


// Qt includes
#include <QtCore/QList>
#include <QtCore/QMutex>

// AcfSln includes
#include <ihotf/IFileSystemChangeStorage.h>


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
	virtual int	 GetStorageItemsCount() const override;
	virtual QString GetItemPath(int fileIndex) const override;
	virtual int GetItemState(int fileIndex) const override;
	virtual void UpdateStorageItem(const QString& path, int changeFlag) override;
	virtual void RemoveStorageItem(int itemIndex) override;
	virtual void ResetStorage() override;

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

#if QT_VERSION >= 0x060000
	mutable QRecursiveMutex m_mutex;
#else
	mutable QMutex m_mutex;
#endif
};


} // namespace ihotf


#endif // !ihotf_CFileSystemChangeStorage_included

