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
	:m_directoryMonitorObserver(*this),
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

void CHotfolderTaskManagerComp::OnFilesAddedEvent(const istd::CStringList& addedFiles)
{
	I_ASSERT(m_hotfolderProcessingInfoCompPtr.IsValid());

	if (m_hotfolderProcessingInfoCompPtr.IsValid()){

		istd::CChangeNotifier changePtr(m_hotfolderProcessingInfoCompPtr.GetPtr(), ifpf::IHotfolderProcessingInfo::CF_FILE_ADDED);

		for (int fileIndex = 0; fileIndex < int(addedFiles.size()); fileIndex++){
			istd::CString outputFilePath = m_fileNamingCompPtr->GetFilePath(addedFiles[fileIndex]);

			m_hotfolderProcessingInfoCompPtr->AddProcessingItem(addedFiles[fileIndex], outputFilePath);
		}
	}
}


void CHotfolderTaskManagerComp::OnFilesRemovedEvent(const istd::CStringList& removedFiles)
{
	I_ASSERT(m_hotfolderProcessingInfoCompPtr.IsValid());
	if (!m_hotfolderProcessingInfoCompPtr.IsValid()){
		return;
	}

	istd::CChangeNotifier changePtr(m_hotfolderProcessingInfoCompPtr.GetPtr(), ifpf::IHotfolderProcessingInfo::CF_FILE_REMOVED);

	bool processingItemsRemoved = false;

	for (int fileIndex = 0; fileIndex < int(removedFiles.size()); fileIndex++){
		ifpf::IHotfolderProcessingItem* processingItemPtr = FindProcessingItem(removedFiles[fileIndex]);
		if (processingItemPtr != NULL){
			m_hotfolderProcessingInfoCompPtr->RemoveProcessingItem(processingItemPtr);

			processingItemsRemoved = true;
		}
	}

	// abort update operation:
	if (!processingItemsRemoved){
		changePtr.Abort();
	}
}


void CHotfolderTaskManagerComp::OnFilesModifiedEvent(const istd::CStringList& modifiedFiles)
{
	I_ASSERT(m_hotfolderProcessingInfoCompPtr.IsValid());
	if (!m_hotfolderProcessingInfoCompPtr.IsValid()){
		return;
	}

	for (int fileIndex = 0; fileIndex < int(modifiedFiles.size()); fileIndex++){
		ifpf::IHotfolderProcessingItem* processingItemPtr = FindProcessingItem(modifiedFiles[fileIndex]);
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

	BaseClass::OnComponentCreated();

	m_isInitialized = true;
}


void CHotfolderTaskManagerComp::OnComponentDestroyed()
{
	I_ASSERT(m_hotfolderSettingsModelCompPtr.IsValid());
	if (m_hotfolderSettingsModelCompPtr.IsValid() && m_hotfolderSettingsModelCompPtr->IsAttached(&m_parametersObserver)){
		m_hotfolderSettingsModelCompPtr->DetachObserver(&m_parametersObserver);
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

			imod::IModel* directoryMonitorModelPtr = dynamic_cast<imod::IModel*>(directoryMonitorPtr);
			I_ASSERT(directoryMonitorModelPtr != NULL);

			directoryMonitorModelPtr->AttachObserver(&m_directoryMonitorObserver);

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

	OnFilesRemovedEvent(removedFiles);
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


// public methods of embedded class DirectoryMonitorObserver

CHotfolderTaskManagerComp::DirectoryMonitorObserver::DirectoryMonitorObserver(CHotfolderTaskManagerComp& parent)
	:m_parent(parent)
{
}


// reimplemented (imod::IObserver)
	
void CHotfolderTaskManagerComp::DirectoryMonitorObserver::AfterUpdate(imod::IModel* modelPtr, int updateFlags, istd::IPolymorphic* /*updateParamsPtr*/)
{
	ifpf::IDirectoryMonitor* monitorPtr = dynamic_cast<ifpf::IDirectoryMonitor*>(modelPtr);
	
	if (monitorPtr != NULL && (updateFlags & ifpf::IDirectoryMonitor::CF_FILES_ADDED) != 0){
		m_parent.OnFilesAddedEvent(monitorPtr->GetChangedFileItems(ifpf::IDirectoryMonitor::CF_FILES_ADDED));
	}

	if (monitorPtr != NULL && (updateFlags & ifpf::IDirectoryMonitor::CF_FILES_REMOVED) != 0){
		m_parent.OnFilesRemovedEvent(monitorPtr->GetChangedFileItems(ifpf::IDirectoryMonitor::CF_FILES_REMOVED));
	}

	if (monitorPtr != NULL && (updateFlags & ifpf::IDirectoryMonitor::CF_FILES_MODIFIED) != 0){
		m_parent.OnFilesModifiedEvent(monitorPtr->GetChangedFileItems(ifpf::IDirectoryMonitor::CF_FILES_MODIFIED));
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


