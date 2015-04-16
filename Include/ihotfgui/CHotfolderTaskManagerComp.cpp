#include "ihotfgui/CHotfolderTaskManagerComp.h"


// Qt includes
#include <QtCore/QDir>
#if QT_VERSION >= 0x050000
#include <QtWidgets/QApplication>
#else
#include <QtGui/QApplication>
#endif

// ACF includes
#include "istd/TChangeDelegator.h"
#include "istd/CChangeNotifier.h"
#include "istd/CChangeGroup.h"
#include "ifile/IFileNameParam.h"
#include "iproc/IProcessor.h"


namespace ihotfgui
{


// public methods

CHotfolderTaskManagerComp::CHotfolderTaskManagerComp()
	:m_fileSystemChangeStorageObserver(*this),
	m_parametersObserver(*this),
	m_isInitialized(false)
{
}


// reimplemented (ihotf::IHotfolderTaskManager)

ihotf::IHotfolderProcessingItem* CHotfolderTaskManagerComp::GetNextProcessingTask() const
{
	Q_ASSERT(m_hotfolderProcessingInfoCompPtr.IsValid());
	if (!m_hotfolderProcessingInfoCompPtr.IsValid()){
		return NULL;
	}
		
	int itemsCount = m_hotfolderProcessingInfoCompPtr->GetProcessingItemsCount();
	for (int itemIndex = 0; itemIndex < itemsCount; itemIndex++){
		ihotf::IHotfolderProcessingItem* processingItemPtr = m_hotfolderProcessingInfoCompPtr->GetProcessingItem(itemIndex);
		Q_ASSERT(processingItemPtr != NULL);

		if (processingItemPtr->GetProcessingState() == iproc::IProcessor::TS_NONE){
			if (m_fileNamingCompPtr.IsValid()){
				QString outputFilePath = m_fileNamingCompPtr->CalculateFileName(processingItemPtr->GetInputFile(), QString(), NULL);

				processingItemPtr->SetOutputFile(outputFilePath);
			}

			return processingItemPtr;
		}
	}

	return NULL;
}


// protected methods

QStringList CHotfolderTaskManagerComp::GetFilesFromStorage(const ihotf::IFileSystemChangeStorage& storage, int fileState) const
{
	int filesCount = storage.GetStorageItemsCount();

	QStringList files;
	for (int index = 0; index < filesCount; index++){
		int itemState = storage.GetItemState(index);

		if ((itemState & fileState) != 0){
			files.push_back(storage.GetItemPath(index));
		}
	}

	return files;
}


void CHotfolderTaskManagerComp::OnFilesAddedEvent(const ihotf::IFileSystemChangeStorage& storage)
{
	AddFilesToProcessingQueue(GetFilesFromStorage(storage, ihotf::IFileSystemChangeStorage::CF_NEW));
}


void CHotfolderTaskManagerComp::OnFilesRemovedEvent(const ihotf::IFileSystemChangeStorage& storage)
{
	RemoveFilesFromProcessingQueue(GetFilesFromStorage(storage, ihotf::IFileSystemChangeStorage::CF_REMOVED));
}


void CHotfolderTaskManagerComp::OnFilesModifiedEvent(const ihotf::IFileSystemChangeStorage& storage)
{
	RestartProcessingQueueFiles(GetFilesFromStorage(storage, ihotf::IFileSystemChangeStorage::CF_MODIFIED));
}


void CHotfolderTaskManagerComp::AddFilesToProcessingQueue(const QStringList& files)
{
	Q_ASSERT(m_hotfolderProcessingInfoCompPtr.IsValid());
	Q_ASSERT(!files.isEmpty());

	if (m_hotfolderProcessingInfoCompPtr.IsValid()){
		static istd::IChangeable::ChangeSet changeSet(ihotf::IHotfolderProcessingInfo::CF_FILE_ADDED);
		istd::CChangeNotifier hotfolderNotifier(m_hotfolderProcessingInfoCompPtr.GetPtr(), changeSet);

		for (int fileIndex = 0; fileIndex < int(files.size()); fileIndex++){
			m_hotfolderProcessingInfoCompPtr->AddProcessingItem(files[fileIndex]);
		}
	}
}


void CHotfolderTaskManagerComp::RemoveFilesFromProcessingQueue(const QStringList& files)
{
	Q_ASSERT(m_hotfolderProcessingInfoCompPtr.IsValid());
	Q_ASSERT(!files.isEmpty());

	if (!m_hotfolderProcessingInfoCompPtr.IsValid()){
		return;
	}

	istd::CChangeGroup changePtr(m_hotfolderProcessingInfoCompPtr.GetPtr());

	for (int fileIndex = 0; fileIndex < int(files.size()); fileIndex++){
		ihotf::IHotfolderProcessingItem* processingItemPtr = FindProcessingItem(files[fileIndex]);
		if (processingItemPtr != NULL){
			static istd::IChangeable::ChangeSet changeSet(ihotf::IHotfolderProcessingInfo::CF_FILE_REMOVED);
			istd::CChangeNotifier hotfolderNotifier(m_hotfolderProcessingInfoCompPtr.GetPtr(), changeSet);

			m_hotfolderProcessingInfoCompPtr->RemoveProcessingItem(processingItemPtr);
		}
	}
}


void CHotfolderTaskManagerComp::RestartProcessingQueueFiles(const QStringList& files)
{
	Q_ASSERT(m_hotfolderProcessingInfoCompPtr.IsValid());
	Q_ASSERT(!files.isEmpty());

	if (!m_hotfolderProcessingInfoCompPtr.IsValid()){
		return;
	}

	for (int fileIndex = 0; fileIndex < int(files.size()); fileIndex++){
		ihotf::IHotfolderProcessingItem* processingItemPtr = FindProcessingItem(files[fileIndex]);
		if (processingItemPtr != NULL){
			processingItemPtr->SetProcessingState(iproc::IProcessor::TS_NONE);
		}
	}
}


// reimplemented (icomp::CComponentBase)

void CHotfolderTaskManagerComp::OnComponentCreated()
{
	Q_ASSERT(m_hotfolderSettingsModelCompPtr.IsValid());
	if (m_hotfolderSettingsModelCompPtr.IsValid()){
		m_hotfolderSettingsModelCompPtr->AttachObserver(&m_parametersObserver);
	}

	Q_ASSERT(m_fileSystemChangeStorageModelCompPtr.IsValid());
	if (m_fileSystemChangeStorageModelCompPtr.IsValid()){
		m_fileSystemChangeStorageModelCompPtr->AttachObserver(&m_fileSystemChangeStorageObserver);
	}

	BaseClass::OnComponentCreated();

	m_isInitialized = true;
}


void CHotfolderTaskManagerComp::OnComponentDestroyed()
{
	Q_ASSERT(m_hotfolderSettingsModelCompPtr.IsValid());
	if (m_hotfolderSettingsModelCompPtr.IsValid() && m_hotfolderSettingsModelCompPtr->IsAttached(&m_parametersObserver)){
		m_hotfolderSettingsModelCompPtr->DetachObserver(&m_parametersObserver);
	}

	Q_ASSERT(m_fileSystemChangeStorageModelCompPtr.IsValid());
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
	QStringList addedDirectories = GetAddedInputDirectories();
	for (int pathIndex = 0; pathIndex < int(addedDirectories.size()); pathIndex++){
		const iprm::IParamsSet* monitoringParamsPtr = GetMonitoringParamsSet(addedDirectories[pathIndex]);
		if (monitoringParamsPtr != NULL){
			AddDirectoryMonitor(addedDirectories[pathIndex], monitoringParamsPtr);
		}
	}
	
	QStringList removedDirectories = GetRemovedInputDirectories();
	for (int pathIndex = 0; pathIndex < int(removedDirectories.size()); pathIndex++){
		RemoveDirectoryMonitor(removedDirectories[pathIndex]);

		RemoveDirectoryItems(removedDirectories[pathIndex]);
	}
}


QStringList CHotfolderTaskManagerComp::GetInputDirectories() const
{
	QStringList inputDirectories;

	if (m_inputDirectoriesManagerCompPtr.IsValid() ){
		int inputDirectoriesCount = m_inputDirectoriesManagerCompPtr->GetParamsSetsCount();
		for (int inputIndex = 0; inputIndex < inputDirectoriesCount; inputIndex++){
			iprm::IParamsSet* inputDirectoryParamPtr = m_inputDirectoriesManagerCompPtr->GetParamsSet(inputIndex);
			Q_ASSERT(inputDirectoryParamPtr != NULL);

			const ifile::IFileNameParam* monitoringDirectoryPtr = dynamic_cast<const ifile::IFileNameParam*>(inputDirectoryParamPtr->GetParameter("DirectoryPath"));
			Q_ASSERT(monitoringDirectoryPtr != NULL);
			Q_ASSERT(monitoringDirectoryPtr->GetPathType() == ifile::IFileNameParam::PT_DIRECTORY);

			inputDirectories.push_back(monitoringDirectoryPtr->GetPath());
		}
	}

	return inputDirectories;
}


const iprm::IParamsSet* CHotfolderTaskManagerComp::GetMonitoringParamsSet(const QString& directoryPath) const
{
	if (m_inputDirectoriesManagerCompPtr.IsValid() ){
		int inputDirectoriesCount = m_inputDirectoriesManagerCompPtr->GetParamsSetsCount();
		for (int inputIndex = 0; inputIndex < inputDirectoriesCount; inputIndex++){
			iprm::IParamsSet* inputDirectoryParamPtr = m_inputDirectoriesManagerCompPtr->GetParamsSet(inputIndex);
			Q_ASSERT(inputDirectoryParamPtr != NULL);

			const ifile::IFileNameParam* monitoringDirectoryPtr = dynamic_cast<const ifile::IFileNameParam*>(inputDirectoryParamPtr->GetParameter("DirectoryPath"));
			Q_ASSERT(monitoringDirectoryPtr != NULL);
			Q_ASSERT(monitoringDirectoryPtr->GetPathType() == ifile::IFileNameParam::PT_DIRECTORY);

			if (monitoringDirectoryPtr->GetPath() == directoryPath){
				return inputDirectoryParamPtr;
			}
		}
	}

	return NULL;
}


QStringList CHotfolderTaskManagerComp::GetAddedInputDirectories() const
{
	QStringList inputDirectories = GetInputDirectories();
	QStringList addedDirectories;

	for (int index = 0; index < int(inputDirectories.size()); index++){
		if (m_directoryMonitorsMap.find(inputDirectories[index]) == m_directoryMonitorsMap.end()){
			addedDirectories.push_back(inputDirectories[index]);
		}
	}

	return addedDirectories;
}


QStringList CHotfolderTaskManagerComp::GetRemovedInputDirectories() const
{
	QStringList inputDirectories = GetInputDirectories();
	QStringList removedDirectories;

	for (		DirectoryMonitorsMap::const_iterator index = m_directoryMonitorsMap.begin();
				index != m_directoryMonitorsMap.end();
				index++){
		QStringList::const_iterator foundIter = qFind(inputDirectories.begin(), inputDirectories.end(), index.key());
		if (foundIter == inputDirectories.end()){
			removedDirectories.push_back(index.key());
		}
	}

	return removedDirectories;
}


ihotf::IDirectoryMonitor* CHotfolderTaskManagerComp::AddDirectoryMonitor(const QString& directoryPath, const iprm::IParamsSet* monitoringParamsPtr)
{
	istd::TDelPtr<icomp::IComponent> monitorCompPtr(m_monitorFactCompPtr.CreateComponent());
	if (monitorCompPtr.IsValid()){
		ihotf::IDirectoryMonitor* directoryMonitorPtr = m_monitorFactCompPtr.ExtractInterface(monitorCompPtr.GetPtr());
		if (directoryMonitorPtr != NULL){
			m_directoryMonitorsMap[directoryPath] = directoryMonitorPtr;

			monitorCompPtr.PopPtr();

			directoryMonitorPtr->StartObserving(monitoringParamsPtr);
		}

		return directoryMonitorPtr;
	}

	return NULL;
}

	
void CHotfolderTaskManagerComp::RemoveDirectoryMonitor(const QString& directoryPath)
{
	DirectoryMonitorsMap::iterator monitorIter = m_directoryMonitorsMap.find(directoryPath);
	if (monitorIter != m_directoryMonitorsMap.end()){
		monitorIter.value()->StopObserving();

		m_directoryMonitorsMap.erase(monitorIter);
	}
}


void CHotfolderTaskManagerComp::RemoveDirectoryItems(const QString& directoryPath)
{
	Q_ASSERT(m_hotfolderProcessingInfoCompPtr.IsValid());
	if (!m_hotfolderProcessingInfoCompPtr.IsValid()){
		return;
	}

	QStringList removedFiles;
	int itemsCount = m_hotfolderProcessingInfoCompPtr->GetProcessingItemsCount();
	for (int itemIndex = 0; itemIndex < itemsCount; itemIndex++){
		ihotf::IHotfolderProcessingItem* processingItemPtr = m_hotfolderProcessingInfoCompPtr->GetProcessingItem(itemIndex);
		Q_ASSERT(processingItemPtr != NULL);

		QString filePath = processingItemPtr->GetInputFile();
		QFileInfo fileInfo(filePath);
		if (fileInfo.canonicalPath() == directoryPath){
			removedFiles.push_back(processingItemPtr->GetInputFile());
		}
	}

	RemoveFilesFromProcessingQueue(removedFiles);
}


ihotf::IHotfolderProcessingItem* CHotfolderTaskManagerComp::FindProcessingItem(const QString& fileName) const
{
	Q_ASSERT(m_hotfolderProcessingInfoCompPtr.IsValid());
	if (!m_hotfolderProcessingInfoCompPtr.IsValid()){
		return NULL;
	}
		
	int itemsCount = m_hotfolderProcessingInfoCompPtr->GetProcessingItemsCount();
	for (int itemIndex = 0; itemIndex < itemsCount; itemIndex++){
		ihotf::IHotfolderProcessingItem* processingItemPtr = m_hotfolderProcessingInfoCompPtr->GetProcessingItem(itemIndex);
		Q_ASSERT(processingItemPtr != NULL);

		if (processingItemPtr->GetInputFile() == fileName){
			return processingItemPtr;
		}
	}

	return NULL;
}


ihotf::IHotfolderProcessingItem* CHotfolderTaskManagerComp::GetItemFromId(const QByteArray& itemUuid) const
{
	Q_ASSERT(m_hotfolderProcessingInfoCompPtr.IsValid());
	if (!m_hotfolderProcessingInfoCompPtr.IsValid()){
		return NULL;
	}
		
	int itemsCount = m_hotfolderProcessingInfoCompPtr->GetProcessingItemsCount();
	for (int itemIndex = 0; itemIndex < itemsCount; itemIndex++){
		ihotf::IHotfolderProcessingItem* processingItemPtr = m_hotfolderProcessingInfoCompPtr->GetProcessingItem(itemIndex);

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
	
void CHotfolderTaskManagerComp::FileSystemChangeStorageObserver::AfterUpdate(imod::IModel* modelPtr, const istd::IChangeable::ChangeSet& changeSet)
{
	ihotf::IFileSystemChangeStorage* storagePtr = dynamic_cast<ihotf::IFileSystemChangeStorage*>(modelPtr);
	Q_ASSERT(storagePtr != NULL);
	if (storagePtr == NULL){
		m_parent.SendCriticalMessage(0, "File system changes storage is invalid", "Hotfolder manager");

		return;
	}

	if (changeSet.Contains(ihotf::IFileSystemChangeStorage::CF_NEW)){
		m_parent.OnFilesAddedEvent(*storagePtr);
	}

	if (changeSet.Contains(ihotf::IFileSystemChangeStorage::CF_REMOVED)){
		m_parent.OnFilesRemovedEvent(*storagePtr);
	}

	if (changeSet.Contains(ihotf::IFileSystemChangeStorage::CF_MODIFIED)){
		m_parent.OnFilesModifiedEvent(*storagePtr);
	}
}


// public methods of embedded class ParametersObserver

CHotfolderTaskManagerComp::ParametersObserver::ParametersObserver(CHotfolderTaskManagerComp& parent)
	:m_parent(parent)
{
}


// reimplemented (imod::IObserver)
	
void CHotfolderTaskManagerComp::ParametersObserver::AfterUpdate(imod::IModel* /*modelPtr*/, const istd::IChangeable::ChangeSet& /*changeSet*/)
{
	m_parent.SynchronizeWithModel();
}


} // namespace ihotfgui


