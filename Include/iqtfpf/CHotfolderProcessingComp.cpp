#include "iqtfpf/CHotfolderProcessingComp.h"


// STL includes
#include <algorithm>


// Qt includes
#include <QDir>
#include <QApplication>


// ACF includes
#include "istd/TChangeDelegator.h"
#include "istd/TChangeNotifier.h"
#include "istd/CStaticServicesProvider.h"

#include "isys/IApplicationEnvironment.h"
#include "isys/CSectionBlocker.h"

#include "iprm/IFileNameParam.h"

#include "iproc/IProcessor.h"

#include "iqt/CTimer.h"


namespace iqtfpf
{


// public methods

CHotfolderProcessingComp::CHotfolderProcessingComp()
	:m_directoryMonitorObserver(*this),
	m_parametersObserver(*this),
	m_stateObserver(*this),
	m_isInitialized(false)
{
}


// reimplemeneted (icomp::IComponent)

void CHotfolderProcessingComp::OnComponentCreated()
{
	I_ASSERT(m_hotfolderSettingsModelCompPtr.IsValid());
	if (m_hotfolderSettingsModelCompPtr.IsValid()){
		m_hotfolderSettingsModelCompPtr->AttachObserver(&m_parametersObserver);
	}

	I_ASSERT(m_hotfolderProcessingModelCompPtr.IsValid());
	if (m_hotfolderProcessingModelCompPtr.IsValid()){
		m_hotfolderProcessingModelCompPtr->AttachObserver(&m_stateObserver);
	}

	connect(&m_filesQueueTimer, SIGNAL(timeout()), this, SLOT(OnUpdateQueueTimer()));
	connect(&m_processingTimer, SIGNAL(timeout()), this, SLOT(OnProcessingTimer()));

	m_filesQueueTimer.start(100);
	m_processingTimer.start(500);

	I_ASSERT(m_filesQueueTimer.isActive());
	I_ASSERT(m_processingTimer.isActive());
	
	BaseClass::OnComponentCreated();

	m_isInitialized = true;
}


void CHotfolderProcessingComp::OnComponentDestroyed()
{
	I_ASSERT(m_hotfolderSettingsModelCompPtr.IsValid());
	if (m_hotfolderSettingsModelCompPtr.IsValid()){
		m_hotfolderSettingsModelCompPtr->DetachObserver(&m_parametersObserver);
	}

	m_directoryMonitorsMap.clear();

	m_filesQueueTimer.stop();

	m_processingTimer.stop();

	CancelAllProcessingItems();

	BaseClass::OnComponentDestroyed();
}


// protected methods

void CHotfolderProcessingComp::OnFilesAddedEvent(const istd::CStringList& addedFiles)
{
	m_filesQueue.insert(m_filesQueue.begin(), addedFiles.begin(), addedFiles.end());
}


void CHotfolderProcessingComp::OnFilesRemovedEvent(const istd::CStringList& removedFiles)
{
	bool workDone = false;

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

	while (!workDone){
		workDone = true;
		for (int fileIndex = 0; fileIndex < int(removedFiles.size()); fileIndex++){
			FilesQueue::iterator fileIter = std::find(m_filesQueue.begin(), m_filesQueue.end(), removedFiles[fileIndex]);
			if (fileIter != m_filesQueue.end()){
				m_filesQueue.erase(fileIter);

				workDone = false;

				break;
			}
		}
	}
}


void CHotfolderProcessingComp::OnFilesModifiedEvent(const istd::CStringList& modifiedFiles)
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


// protected slots

void CHotfolderProcessingComp::OnUpdateQueueTimer()
{
	if (m_filesQueue.empty()){
		return;
	}

	const istd::CString& inputFilePath = m_filesQueue.back();
	QFileInfo fileInfo(iqt::GetQString(inputFilePath));
	if (!fileInfo.exists()){
		return;
	}

	istd::CString outputFilePath = m_fileNamingCompPtr->GetFilePath(inputFilePath);

	// add file to hotfolder's model:
	m_hotfolderProcessingInfoCompPtr->AddProcessingItem(inputFilePath, outputFilePath);

	m_filesQueue.pop_back();
}


void CHotfolderProcessingComp::OnProcessingTimer()
{
	if (!m_hotfolderProcessingInfoCompPtr->IsWorking()){
		return;
	}

	bool updateProcessedItemsDone = false;
	while (!updateProcessedItemsDone){
		updateProcessedItemsDone = true;
		for (int processingFutureIndex = 0; processingFutureIndex < m_pendingProcessors.GetCount(); processingFutureIndex++){
			ItemProcessor* processorPtr = m_pendingProcessors.GetAt(processingFutureIndex);
			if (processorPtr->isFinished()){
				OnProcessingItemFinished(*processorPtr);

				m_pendingProcessors.RemoveAt(processingFutureIndex);
				updateProcessedItemsDone = false;
				break;
			}
		}
	}

	// get available file to process:
	ifpf::IHotfolderProcessingItem* processingItemPtr = GetNextProcessingFile();
	if (processingItemPtr != NULL){
		int runningThreadCount = 0;

		for (int processingFutureIndex = 0; processingFutureIndex < m_pendingProcessors.GetCount(); processingFutureIndex++){
			if (m_pendingProcessors.GetAt(processingFutureIndex)->isRunning()){
				runningThreadCount++;
			}
		}

		if (runningThreadCount < QThread::idealThreadCount()){
			istd::CChangeNotifier changePtr(processingItemPtr);

			processingItemPtr->SetProcessingState(iproc::IProcessor::TS_WAIT);
			if (m_fileNamingCompPtr.IsValid()){
				istd::CString outputFile = m_fileNamingCompPtr->GetFilePath(processingItemPtr->GetInputFile());

				processingItemPtr->SetOutputFile(outputFile);
			}

			changePtr.Reset();

			istd::CString inputFilePath = processingItemPtr->GetInputFile();
			istd::CString outputFilePath = processingItemPtr->GetOutputFile();
			std::string itemUuid = processingItemPtr->GetItemUuid();

			m_pendingProcessors.PushBack(new ItemProcessor(*this, inputFilePath, outputFilePath, itemUuid));
		}
	}
}


void CHotfolderProcessingComp::OnProcessingItemFinished(const ItemProcessor& processor)
{
	ifpf::IHotfolderProcessingItem* itemPtr = GetItemFromId(processor.GetItemUuid());
	if (itemPtr->GetProcessingState() != iproc::IProcessor::TS_CANCELED){
		istd::CChangeNotifier changePtr(itemPtr);

		itemPtr->SetProcessingState(processor.GetProcessingState());

		isys::CSimpleDateTime startTime = iqt::GetCSimpleDateTime(processor.GetStartTime());
		itemPtr->SetStartTime(startTime);

		itemPtr->SetProcessingTime(processor.GetProcessingTime());
	}
}


void CHotfolderProcessingComp::SynchronizeWithModel(bool /*applyToPendingTasks*/)
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


istd::CStringList CHotfolderProcessingComp::GetInputDirectories() const
{
	istd::CStringList inputDirectories;

	if (m_inputDirectoriesManagerCompPtr.IsValid() ){
		int inputDirectoriesCount = m_inputDirectoriesManagerCompPtr->GetSetsCount();
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


const iprm::IParamsSet* CHotfolderProcessingComp::GetMonitoringParamsSet(const istd::CString& directoryPath) const
{
	if (m_inputDirectoriesManagerCompPtr.IsValid() ){
		int inputDirectoriesCount = m_inputDirectoriesManagerCompPtr->GetSetsCount();
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


istd::CStringList CHotfolderProcessingComp::GetAddedInputDirectories() const
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


istd::CStringList CHotfolderProcessingComp::GetRemovedInputDirectories() const
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


ifpf::IDirectoryMonitor* CHotfolderProcessingComp::AddDirectoryMonitor(const istd::CString& directoryPath, const iprm::IParamsSet* monitoringParamsPtr)
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

	
void CHotfolderProcessingComp::RemoveDirectoryMonitor(const istd::CString& directoryPath)
{
	DirectoryMonitorsMap::iterator monitorIter = m_directoryMonitorsMap.find(directoryPath);
	if (monitorIter != m_directoryMonitorsMap.end()){
		monitorIter->second->StopObserving();

		m_directoryMonitorsMap.erase(monitorIter);
	}
}


void CHotfolderProcessingComp::RemoveDirectoryItems(const istd::CString& directoryPath)
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

	for (FilesQueue::const_iterator index = m_filesQueue.begin(); index != m_filesQueue.end(); index++){
		QString filePath = iqt::GetQString(*index);
		QFileInfo fileInfo(filePath);
		if (fileInfo.canonicalPath() == iqt::GetQString(directoryPath)){
			removedFiles.push_back(*index);
		}
	}

	OnFilesRemovedEvent(removedFiles);
}


ifpf::IHotfolderProcessingItem* CHotfolderProcessingComp::GetNextProcessingFile() const
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
			return processingItemPtr;
		}
	}

	return NULL;
}


ifpf::IHotfolderProcessingItem* CHotfolderProcessingComp::FindProcessingItem(const istd::CString& fileName) const
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


void CHotfolderProcessingComp::OnCancelProcessingItem(const ifpf::IHotfolderProcessingItem* processingItemPtr)
{
	for (int pendingProcessorIndex = 0; pendingProcessorIndex < m_pendingProcessors.GetCount(); pendingProcessorIndex++){
		ItemProcessor* processorPtr = m_pendingProcessors.GetAt(pendingProcessorIndex);
		I_ASSERT(processorPtr != NULL);

		if (processorPtr->GetItemUuid() == processingItemPtr->GetItemUuid()){
			processorPtr->Cancel();

			m_pendingProcessors.RemoveAt(pendingProcessorIndex);

			break;
		}
	}
}


void CHotfolderProcessingComp::CancelAllProcessingItems()
{
	for (int pendingProcessorIndex = 0; pendingProcessorIndex < m_pendingProcessors.GetCount(); pendingProcessorIndex++){
		m_pendingProcessors.GetAt(pendingProcessorIndex)->Cancel();		
	}

	m_pendingProcessors.Reset();

	I_ASSERT(m_hotfolderProcessingInfoCompPtr.IsValid());
	if (!m_hotfolderProcessingInfoCompPtr.IsValid()){
		return;
	}

	int itemsCount = m_hotfolderProcessingInfoCompPtr->GetProcessingItemsCount();
	for (int itemIndex = 0; itemIndex < itemsCount; itemIndex++){
		ifpf::IHotfolderProcessingItem* processingItemPtr = m_hotfolderProcessingInfoCompPtr->GetProcessingItem(itemIndex);

		if (processingItemPtr->GetProcessingState() == iproc::IProcessor::TS_WAIT){
			processingItemPtr->SetProcessingState(iproc::IProcessor::TS_NONE);
		}
	}
}


ifpf::IHotfolderProcessingItem* CHotfolderProcessingComp::GetItemFromId(const std::string& itemUuid) const
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

CHotfolderProcessingComp::DirectoryMonitorObserver::DirectoryMonitorObserver(CHotfolderProcessingComp& parent)
	:m_parent(parent)
{
}


// reimplemented (imod::IObserver)
	
void CHotfolderProcessingComp::DirectoryMonitorObserver::AfterUpdate(imod::IModel* modelPtr, int updateFlags, istd::IPolymorphic* /*updateParamsPtr*/)
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

CHotfolderProcessingComp::ParametersObserver::ParametersObserver(CHotfolderProcessingComp& parent)
	:m_parent(parent)
{
}


// reimplemented (imod::IObserver)
	
void CHotfolderProcessingComp::ParametersObserver::AfterUpdate(imod::IModel* /*modelPtr*/, int /*updateFlags*/, istd::IPolymorphic* /*updateParamsPtr*/)
{
	m_parent.SynchronizeWithModel();
}


// public methods of embedded class StateObserver

CHotfolderProcessingComp::StateObserver::StateObserver(CHotfolderProcessingComp& parent)
	:m_parent(parent)
{
}


// reimplemented (imod::IObserver)

void CHotfolderProcessingComp::StateObserver::AfterUpdate(imod::IModel* modelPtr, int updateFlags, istd::IPolymorphic* updateParamsPtr)
{
	if ((updateFlags & ifpf::IHotfolderProcessingItem::CF_STATE_CHANGED) != 0){
		ifpf::IHotfolderProcessingItem* processingItemPtr = dynamic_cast<ifpf::IHotfolderProcessingItem*>(updateParamsPtr);
		if (processingItemPtr != NULL && processingItemPtr->GetProcessingState() == iproc::IProcessor::TS_CANCELED){
			m_parent.OnCancelProcessingItem(processingItemPtr);
		}
	}

	BaseClass::AfterUpdate(modelPtr, updateFlags, updateParamsPtr);
}


// public methods of embedded class ItemProcessor

CHotfolderProcessingComp::ItemProcessor::ItemProcessor(
			CHotfolderProcessingComp& parent,
			const istd::CString& inputFilePath,
			const istd::CString& outputFilePath,
			const std::string& itemUuid)
	:m_parent(parent),
	m_inputFilePath(inputFilePath),
	m_outputFilePath(outputFilePath),
	m_itemUuid(itemUuid),
	m_processingState(iproc::IProcessor::TS_INVALID)
{
	start();
}


int CHotfolderProcessingComp::ItemProcessor::GetProcessingState() const
{
	return m_processingState;
}


std::string CHotfolderProcessingComp::ItemProcessor::GetItemUuid() const
{
	return m_itemUuid;
}


QDateTime CHotfolderProcessingComp::ItemProcessor::GetStartTime() const
{
	return m_startTime;
}

double CHotfolderProcessingComp::ItemProcessor::GetProcessingTime() const
{
	return m_processingTime;
}


void CHotfolderProcessingComp::ItemProcessor::Cancel()
{
	if (BaseClass::isFinished()){
		return;
	}

	iqt::CTimer timer;
	while (isRunning() && timer.GetElapsed() < 5000){
		qApp->processEvents();
	}

	if (isRunning()){
		BaseClass::terminate();

		BaseClass::wait();
	}
}


// protected methods

// reimplemented (QThread)

void CHotfolderProcessingComp::ItemProcessor::run()
{
	m_startTime = QDateTime::currentDateTime(); 
	
	I_ASSERT(m_parent.m_fileConvertCompPtr.IsValid());
	if (!m_parent.m_fileConvertCompPtr.IsValid()){
		return;
	}

	iqt::CTimer m_timer;
	
	if (!m_parent.m_fileConvertCompPtr->CopyFile(m_inputFilePath, m_outputFilePath)){
		istd::CString message = istd::CString("Processing of ") + m_inputFilePath + " failed";
		m_parent.SendErrorMessage(0, message, "Hotfolder");

		m_processingState = iproc::IProcessor::TS_INVALID;
	}
	else{
		m_processingState = iproc::IProcessor::TS_OK;
	}

	m_processingTime = m_timer.GetElapsed();
}


} // namespace iqtfpf


