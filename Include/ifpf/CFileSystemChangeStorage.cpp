#include "ifpf/CFileSystemChangeStorage.h"


// ACF includes
#include "istd/TChangeNotifier.h"
#include "istd/CStaticServicesProvider.h"

#include "isys/CSectionBlocker.h"


namespace ifpf
{


// public methods

CFileSystemChangeStorage::CFileSystemChangeStorage()
{
	m_lockPtr = istd::CreateService<isys::ICriticalSection>();

	I_ASSERT(m_lockPtr.IsValid());
}


// reimplemented (ifpf::IFileSystemChangeStorage)

int CFileSystemChangeStorage::GetStorageItemsCount() const
{
	isys::CSectionBlocker lock(const_cast<isys::ICriticalSection*>(m_lockPtr.GetPtr()));

	int itemsCount = int(m_storageItems.size());

	return itemsCount;
}


istd::CString CFileSystemChangeStorage::GetItemPath(int fileIndex) const
{
	isys::CSectionBlocker lock(const_cast<isys::ICriticalSection*>(m_lockPtr.GetPtr()));

	I_ASSERT(fileIndex >= 0);
	I_ASSERT(fileIndex < int(m_storageItems.size()));

	istd::CString filePath = m_storageItems[fileIndex].path;

	return filePath;
}


int CFileSystemChangeStorage::GetItemState(int fileIndex) const
{
	isys::CSectionBlocker lock(const_cast<isys::ICriticalSection*>(m_lockPtr.GetPtr()));

	I_ASSERT(fileIndex >= 0);
	I_ASSERT(fileIndex < int(m_storageItems.size()));

	int fileState = m_storageItems[fileIndex].state;

	return fileState;
}


void CFileSystemChangeStorage::UpdateStorageItem(const istd::CString& path, int itemFlags)
{
	isys::CSectionBlocker lock(const_cast<isys::ICriticalSection*>(m_lockPtr.GetPtr()));

	istd::CChangeNotifier changePtr(this, itemFlags);

	int itemIndex = GetFileIndexFromPath(path);
	if (itemIndex < 0){
		m_storageItems.push_back(FileItem());

		m_storageItems.back().path = path;
		m_storageItems.back().state = itemFlags;
	}
	else{
		FileItem& item = m_storageItems[itemIndex];

		item.state = itemFlags;
	}
}


void CFileSystemChangeStorage::ResetStorage()
{
	isys::CSectionBlocker lock(const_cast<isys::ICriticalSection*>(m_lockPtr.GetPtr()));

	istd::CChangeNotifier changePtr(this);

	m_storageItems.clear();
}


// private methods

int CFileSystemChangeStorage::GetFileIndexFromPath(const istd::CString& filePath) const
{
	for (int index = 0; index < int(m_storageItems.size()); ++index){
		if (m_storageItems[index].path == filePath){
			return index;
		}
	}

	return -1;
}


} // namespace ifpf


