#include "iqtfpf/CHotfolderTaskManagerComp.h"


// STL includes
#include <algorithm>


// Qt includes
#include <QDir>
#include <QApplication>


// ACF includes
#include "istd/TChangeDelegator.h"
#include "istd/TChangeNotifier.h"
#include "istd/CStaticServicesProvider.h"

#include "iprm/IFileNameParam.h"

#include "iproc/IProcessor.h"

#include "iqt/CTimer.h"


namespace iqtfpf
{


// public methods

CHotfolderTaskManagerComp::CHotfolderTaskManagerComp()
	:m_fileSystemChangeStorageObserver(*this),
	m_parametersObserver(*this),
	m_isInitialized(false)
{
}


// reimplemented (ifpf::IHotfolderTaskManager)

ifpf::IHotfolderProcessingItem* CHotfolderTaskManagerComp::GetNextProcessingTask() const
{
	I_ASSERT(m_hotfolderProcessingInfoCompPtr.IsValid());
	if (!m_hotfolderProcessingInfoCompPtr.IsValid()){
		return NULL;
	}
		
	int itemsCount = m_hotfolderProcessingInfoCompPtr->GetProcessingItemsCount();
	for (int itemIndex = 0; itemIndex < itemsCount; itemIndex++){
		ifpf::IHotfolderProcessingItem* processingItemPtr = m_hotfolderProcessingInfoCompPtr->GetProcessingItem(itemIndex);
		I_ASSERT(processingItemPtr != NULL);

		if (processingItemPtr->GetProcessingState() == iproc::IProcessor::TS_NONE){
			if (m_fileNamingCompPtr.IsValid()){
				istd::CString outputFilePath = m_fileNamingCompPtr->GetFilePath(processingItemPtr->GetInputFile());

				processingItemPtr->SetOutputFile(outputFilePath);
			}

			return processingItemPtr;
		}
	}

	return NULL;
}


// protected methods

istd::CStringList CHotfolderTaskManagerComp::GetFilesFromStorage(const ifpf::IFileSystemChangeStorage& storage, int fileState) const
{
	int filesCount = storage.GetStorageItemsCount();

	istd::CStringList files;
	for (int index = 0; index < filesCount; index++){
		int itemState = storage.GetItemState(index);

		if ((itemState & fileState) != 0){
			files.push_back(storage.GetItemPath(index));
		}
	}

	return files;
}


void CHotfolderTaskManagerComp::OnFilesAddedEvent(const ifpf::IFileSystemChangeStorage& storage)
{
	AddFilesToProcessingQueue(GetFilesFromStorage(storage, ifpf::IFileSystemChangeStorage::CF_NEW));
}


void CHotfolderTaskManagerComp::OnFilesRemovedEvent(const ifpf::IFileSystemChangeStorage& storage)
{
	RemoveFilesFromProcessingQueue(GetFilesFromStorage(storage, ifpf::IFileSystemChangeStorage::CF_REMOVED));
}


void CHotfolderTaskManagerComp::OnFilesModifiedEvent(const ifpf::IFileSystemChangeStorage& storage)
{
	RestartProcessingQueueFiles(GetFilesFromStorage(storage, ifpf::IFileSystemChangeStorage::CF_MODIFIED));
}


void CHotfolderTaskManagerComp::AddFilesToProcessingQueue(const istd::CStringList& files)
{
	I_ASSERT(m_hotfolderProcessingInfoCompPtr.IsValid());
	I_ASSERT(!files.empty());

	if (m_hotfolderProcessingInfoCompPtr.IsValid()){

		istd::CChangeNotifier changePtr(m_hotfolderProcessingInfoCompPtr.GetPtr(), ifpf::IHotfolderProcessingInfo::CF_FILE_ADDED);

		for (int fileIndex = 0; fileIndex < int(files.size()); fileIndex++){
			m_hotfolderProcessingInfoCompPtr->AddProcessingItem(files[fileIndex]);
		}
	}
}


void CHotfolderTaskManagerComp::RemoveFilesFromProcessingQueue(const istd::CStringList& files)
{
	I_ASSERT(m_hotfolderProcessingInfoCompPtr.IsValid());
	I_ASSERT(!files.empty());

	if (!m_hotfolderProcessingInfoCompPtr.IsValid()){
		return;
	}

	istd::CChangeNotifier changePtr(NULL, ifpf::IHotfolderProcessingInfo::CF_FILE_REMOVED);

	for (int fileIndex = 0; fileIndex < int(files.size()); fileIndex++){
		ifpf::IHotfolderProcessingItem* processingItemPtr = FindProcessingItem(files[fileIndex]);
		if (processingItemPtr != NULL){
			changePtr.SetPtr(m_hotfolderProcessingInfoCompPtr.GetPtr());

			m_hotfolderProcessingInfoCompPtr->RemoveProcessingItem(processingItemPtr);
		}
	}
}


void CHotfolderTaskManagerComp::RestartProcessingQueueFiles(const istd::CStringList& files)
{
	I_ASSERT(m_hotfolderProcessingInfoCompPtr.IsValid());
	I_ASSERT(!files.empty());

	if (!m_hotfolderProcessingInfoCompPtr.IsValid()){
		return;
	}

	for (int fileIndex = 0; fileIndex < int(files.size()); fileIndex++){
		ifpf::IHotfolderProcessingItem* processingItemPtr = FindProcessingItem(files[fileIndex]);
		if (processingItemPtr != NULL){
			processingItemPtr->SetProcessingState(iproc::IProcessor::TS_NONE);
		}
	}
}


// reimplemented (icomp::CComponentBase)

void CHotfolderTaskManagerComp::OnComponentCreated()
{
	I_ASSERT(m_hotfolderSettingsModelCompPtr.IsValid());
	if (m_hotfolderSettingsModelCompPtr.IsValid()){
		m_hotfolderSettingsModelCompPtr->AttachObserver(&m_parametersObserver);
	}

	I_ASSERT(m_fileSystemChangeStorageModelCompPtr.IsValid());
	if (m_fileSystemChangeStorageModelCompPtr.IsValid()){
		m_fileSystemChangeStorageModelCompPtr->AttachObserver(&m_fileSystemChangeStorageObserver);
	}

	BaseClass::OnComponentCreated();

	m_isInitialized = true;
}


void CHotfolderTaskManagerComp::OnComponentDestroyed()
{
	I_ASSERT(m_hotfolderSettingsModelCompPtr.IsValid());
	if (m_hotfolderSettingsModelCompPtr.IsValid() && m_hotfolderSettingsModelCompPtr->IsAttached(&m_parametersObserver)){
		m_hotfolderSettingsModelCompPtr->DetachObserver(&m_parametersObserver);
	}

	I_ASSERT(m_fileSystemChangeStorageModelCompPtr.IsValid());
	if (m_fileSystemChangeStorageModelCompPtr.IsValid() && m_fileSystemChangeStorageModelCompPtr->IsAttached(&m_fileSystemChangeStorageObserver)){
		m_fileSystemChangeStorageModelCompPtr->DetachObserver(&m_fileSystemChangeStorageObserver);
	}

	m_directoryMonitorsMap.clear();

	BaseClass::OnComponentDestroyed();
}


// protected methods

void CHotfolderTaskManagerComp::SynchronizeWithModel(bool /*applyToPendingTasks*/)
{
	if (!m_isInitialized){
		return;
	}

	// setup directory monitoring:
	istd::CStringList addedDirectories = GetAddedInputDirectories();
	for (int pathIndex = 0; pathIndex < int(addedDirectories.size()); pathIndex++){
		const iprm::IParamsSet* monitoringParamsPtr = GetMonitoringParamsSet(addedDirectories[pathIndex]);
		if (monitoringParamsPtr != NULL){
			AddDirectoryMonitor(addedDirectories[pathIndex], monitoringParamsPtr);
		}
	}
	
	istd::CStringList removedDirectories = GetRemovedInputDirectories();
	for (int pathIndex = 0; pathIndex < int(removedDirectories.size()); pathIndex++){
		RemoveDirectoryMonitor(removedDirectories[pathIndex]);

		RemoveDirectoryItems(removedDirectories[pathIndex]);
	}
}


istd::CStringList CHotfolderTaskManagerComp::GetInputDirectories() const
{
	istd::CStringList inputDirectories;

	if (m_inputDirectoriesManagerCompPtr.IsValid() ){
		int inputDirectoriesCount = m_inputDirectoriesManagerCompPtr->GetParamsSetsCount();
		for (int inputIndex = 0; inputIndex < inputDirectoriesCount; inputIndex++){
			iprm::IParamsSet* inputDirectoryParamPtr = m_inputDirectoriesManagerCompPtr->GetParamsSet(inputIndex);
			I_ASSERT(inputDirectoryParamPtr != NULL);

			const iprm::IFileNameParam* monitoringDirectoryPtr = dynamic_cast<const iprm::IFileNameParam*>(inputDirectoryParamPtr->GetParameter("DirectoryPath"));
			I_ASSERT(monitoringDirectoryPtr != NULL);
			I_ASSERT(monitoringDirectoryPtr->GetPathType() == iprm::IFileNameParam::PT_DIRECTORY);

			inputDirectories.push_back(monitoringDirectoryPtr->GetPath());
		}
	}

	return inputDirectories;
}


const iprm::IParamsSet* CHotfolderTaskManagerComp::GetMonitoringParamsSet(const istd::CString& directoryPath) const
{
	if (m_inputDirectoriesManagerCompPtr.IsValid() ){
		int inputDirectoriesCount = m_inputDirectoriesManagerCompPtr->GetParamsSetsCount();
		for (int inputIndex = 0; inputIndex < inputDirectoriesCount; inputIndex++){
			iprm::IParamsSet* inputDirectoryParamPtr = m_inputDirectoriesManagerCompPtr->GetParamsSet(inputIndex);
			I_ASSERT(inputDirectoryParamPtr != NULL);

			const iprm::IFileNameParam* monitoringDirectoryPtr = dynamic_cast<const iprm::IFileNameParam*>(inputDirectoryParamPtr->GetParameter("DirectoryPath"));
			I_ASSERT(monitoringDirectoryPtr != NULL);
			I_ASSERT(monitoringDirectoryPtr->GetPathType() == iprm::IFileNameParam::PT_DIRECTORY);

			if (monitoringDirectoryPtr->GetPath() == directoryPath){
				return inputDirectoryParamPtr;
			}
		}
	}

	return NULL;
}


istd::CStringList CHotfolderTaskManagerComp::GetAddedInputDirectories() const
{
	istd::CStringList inputDirectories = GetInputDirectories();
	istd::CStringList addedDirectories;

	for (int index = 0; index < int(inputDirectories.size()); index++){
		if (m_directoryMonitorsMap.find(inputDirectories[index]) == m_directoryMonitorsMap.end()){
			addedDirectories.push_back(inputDirectories[index]);
		}
	}

	return addedDirectories;
}


istd::CStringList CHotfolderTaskManagerComp::GetRemovedInputDirectories() const
{
	istd::CStringList inputDirectories = GetInputDirectories();
	istd::CStringList removedDirectories;

	for (		DirectoryMonitorsMap::const_iterator index = m_directoryMonitorsMap.begin();
				index != m_directoryMonitorsMap.end();
				index++){
		istd::CStringList::const_iterator foundIter = std::find(inputDirectories.begin(), inputDirectories.end(), index->first);
		if (foundIter == inputDirectories.end()){
			removedDirectories.push_back(index->first);
		}
	}

	return removedDirectories;
}


ifpf::IDirectoryMonitor* CHotfolderTaskManagerComp::AddDirectoryMonitor(const istd::CString& directoryPath, const iprm::IParamsSet* monitoringParamsPtr)
{
	istd::TDelPtr<icomp::IComponent> monitorCompPtr(m_monitorFactCompPtr.CreateComponent());
	if (monitorCompPtr.IsValid()){
		ifpf::IDirectoryMonitor* directoryMonitorPtr = m_monitorFactCompPtr.ExtractInterface(monitorCompPtr.GetPtr());
		if (directoryMonitorPtr != NULL){
			m_directoryMonitorsMap[directoryPath] = directoryMonitorPtr;

			monitorCompPtr.PopPtr();

			directoryMonitorPtr->StartObserving(monitoringParamsPtr);
		}

		return directoryMonitorPtr;
	}

	return NULL;
}

	
void CHotfolderTaskManagerComp::RemoveDirectoryMonitor(const istd::CString& directoryPath)
{
	DirectoryMonitorsMap::iterator monitorIter = m_directoryMonitorsMap.find(directoryPath);
	if (monitorIter != m_directoryMonitorsMap.end()){
		monitorIter->second->StopObserving();

		m_directoryMonitorsMap.erase(monitorIter);
	}
}


void CHotfolderTaskManagerComp::RemoveDirectoryItems(const istd::CString& directoryPath)
{
	I_ASSERT(m_hotfolderProcessingInfoCompPtr.IsValid());
	if (!m_hotfolderProcessingInfoCompPtr.IsValid()){
		return;
	}

	istd::CStringList removedFiles;
	int itemsCount = m_hotfolderProcessingInfoCompPtr->GetProcessingItemsCount();
	for (int itemIndex = 0; itemIndex < itemsCount; itemIndex++){
		ifpf::IHotfolderProcessingItem* processingItemPtr = m_hotfolderProcessingInfoCompPtr->GetProcessingItem(itemIndex);
		I_ASSERT(processingItemPtr != NULL);

		QString filePath = iqt::GetQString(processingItemPtr->GetInputFile());
		QFileInfo fileInfo(filePath);
		if (fileInfo.canonicalPath() == iqt::GetQString(directoryPath)){
			removedFiles.push_back(processingItemPtr->GetInputFile());
		}
	}

	RemoveFilesFromProcessingQueue(removedFiles);
}


ifpf::IHotfolderProcessingItem* CHotfolderTaskManagerComp::FindProcessingItem(const istd::CString& fileName) const
{
	I_ASSERT(m_hotfolderProcessingInfoCompPtr.IsValid());
	if (!m_hotfolderProcessingInfoCompPtr.IsValid()){
		return NULL;
	}
		
	int itemsCount = m_hotfolderProcessingInfoCompPtr->GetProcessingItemsCount();
	for (int itemIndex = 0; itemIndex < itemsCount; itemIndex++){
		ifpf::IHotfolderProcessingItem* processingItemPtr = m_hotfolderProcessingInfoCompPtr->GetProcessingItem(itemIndex);
		I_ASSERT(processingItemPtr != NULL);

		if (processingItemPtr->GetInputFile() == fileName){
			return processingItemPtr;
		}
	}

	return NULL;
}


ifpf::IHotfolderProcessingItem* CHotfolderTaskManagerComp::GetItemFromId(const std::string& itemUuid) const
{
	I_ASSERT(m_hotfolderProcessingInfoCompPtr.IsValid());
	if (!m_hotfolderProcessingInfoCompPtr.IsValid()){
		return NULL;
	}
		
	int itemsCount = m_hotfolderProcessingInfoCompPtr->GetProcessingItemsCount();
	for (int itemIndex = 0; itemIndex < itemsCount; itemIndex++){
		ifpf::IHotfolderProcessingItem* processingItemPtr = m_hotfolderProcessingInfoCompPtr->GetProcessingItem(itemIndex);

		if (processingItemPtr->GetItemUuid() == itemUuid){
			return processingItemPtr;
		}
	}

	return NULL;
}


// public methods of embedded class FileSystemChangeStorageObserver

CHotfolderTaskManagerComp::FileSystemChangeStorageObserver::FileSystemChangeStorageObserver(CHotfolderTaskManagerComp& parent)
	:m_parent(parent)
{
}


// reimplemented (imod::IObserver)
	
void CHotfolderTaskManagerComp::FileSystemChangeStorageObserver::AfterUpdate(imod::IModel* modelPtr, int updateFlags, istd::IPolymorphic* /*updateParamsPtr*/)
{
	ifpf::IFileSystemChangeStorage* storagePtr = dynamic_cast<ifpf::IFileSystemChangeStorage*>(modelPtr);
	I_ASSERT(storagePtr != NULL);
	if (storagePtr == NULL){
		m_parent.SendCriticalMessage(0, "File system changes storage is invalid", "Hotfolder manager");

		return;
	}

	if ((updateFlags & ifpf::IFileSystemChangeStorage::CF_NEW) != 0){
		m_parent.OnFilesAddedEvent(*storagePtr);
	}

	if ((updateFlags & ifpf::IFileSystemChangeStorage::CF_REMOVED) != 0){
		m_parent.OnFilesRemovedEvent(*storagePtr);
	}

	if ((updateFlags & ifpf::IFileSystemChangeStorage::CF_MODIFIED) != 0){
		m_parent.OnFilesModifiedEvent(*storagePtr);
	}
}


// public methods of embedded class ParametersObserver

CHotfolderTaskManagerComp::ParametersObserver::ParametersObserver(CHotfolderTaskManagerComp& parent)
	:m_parent(parent)
{
}


// reimplemented (imod::IObserver)
	
void CHotfolderTaskManagerComp::ParametersObserver::AfterUpdate(imod::IModel* /*modelPtr*/, int /*updateFlags*/, istd::IPolymorphic* /*updateParamsPtr*/)
{
	m_parent.SynchronizeWithModel();
}


} // namespace iqtfpf


