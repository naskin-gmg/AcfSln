#include "iqtfpf/CHotfolderProcessingComp.h"


// STL includes
#include <algorithm>


// Qt includes
#include <QDir>
#include <QApplication>


// ACF includes
#include "istd/INamed.h"
#include "istd/TChangeNotifier.h"
#include "istd/TChangeDelegator.h"
#include "istd/CStaticServicesProvider.h"

#include "isys/IApplicationEnvironment.h"
#include "isys/CSectionBlocker.h"

#include "iser/CXmlFileWriteArchive.h"

#include "iprm/IFileNameParam.h"

#include "iproc/IProcessor.h"

#include "iqt/CTimer.h"
#include "iqt/CSafeNotifier.h"


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
	I_ASSERT(m_hotfolderParamsModelCompPtr.IsValid());
	if (m_hotfolderParamsModelCompPtr.IsValid()){
		m_hotfolderParamsModelCompPtr->AttachObserver(&m_parametersObserver);
	}

	I_ASSERT(m_hotfolderProcessingModelCompPtr.IsValid());
	if (m_hotfolderProcessingModelCompPtr.IsValid()){
		m_hotfolderProcessingModelCompPtr->AttachObserver(&m_stateObserver);
	}

	connect(this,
				SIGNAL(EmitItemState(ifpf::IHotfolderProcessingItem*, int)),
				this,
				SLOT(OnItemState(ifpf::IHotfolderProcessingItem*, int)), Qt::QueuedConnection);

	connect(&m_filesQueueTimer, SIGNAL(timeout()), this, SLOT(OnUpdateQueueTimer()));

	m_filesQueueTimer.start(250);

	BaseClass::OnComponentCreated();

	m_isInitialized = true;
}


void CHotfolderProcessingComp::OnComponentDestroyed()
{
	StopHotfolder();

	I_ASSERT(m_hotfolderParamsModelCompPtr.IsValid());
	if (m_hotfolderParamsModelCompPtr.IsValid()){
		m_hotfolderParamsModelCompPtr->DetachObserver(&m_parametersObserver);
	}

	m_filesQueueTimer.stop();

	BaseClass::OnComponentDestroyed();
}


// protected methods

bool CHotfolderProcessingComp::OnInputFileEvent(const ifpf::IDirectoryMonitor& directoryMonitor)
{
	if (!m_fileConvertCompPtr.IsValid()){
		SendErrorMessage(0, "File conversion component was not set", "Hotfolder");

		return false;
	}

	if (!m_fileNamingCompPtr.IsValid()){
		SendErrorMessage(0, "File naming component was not set", "Hotfolder");

		return false;
	}

	istd::CStringList files = directoryMonitor.GetChangedFileItems(ifpf::IDirectoryMonitor::CF_FILES_ADDED);

	m_filesQueue.insert(m_filesQueue.begin(), files.begin(), files.end());

	return true;
}


// reimplemented (QThread)

void CHotfolderProcessingComp::run()
{
	I_ASSERT(m_hotfolderProcessingInfoCompPtr.IsValid());

	int workingIntervall = 100; // ms

	while (!m_finishThread){
		if (!m_hotfolderProcessingInfoCompPtr->IsWorking()){
			msleep(workingIntervall);
			continue;					
		}

		isys::CSectionBlocker processingQueueLock(&m_processingQueueLock);
		
		// get available file to process:
		ifpf::IHotfolderProcessingItem* processingItemPtr = GetNextProcessingFile();
		if (processingItemPtr != NULL){
			istd::CString inputFile = processingItemPtr->GetInputFile();
			istd::CString outputFile = processingItemPtr->GetOutputFile();
			processingQueueLock.Reset();

			Q_EMIT EmitItemState(processingItemPtr, iproc::IProcessor::TS_WAIT);
			
			int processingState = ProcessFile(inputFile, outputFile);
		
			Q_EMIT EmitItemState(processingItemPtr, processingState);
		}

		msleep(workingIntervall);
	}
}


// protected slots

void CHotfolderProcessingComp::OnItemState(ifpf::IHotfolderProcessingItem* itemPtr, int itemState)
{
	UpdateProcessingState(itemPtr, itemState);
}


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

	isys::CSectionBlocker queueBlocker(&m_processingQueueLock);

	I_ASSERT(m_hotfolderProcessingInfoCompPtr.IsValid());

	// add file to hotfolder's model:
	m_hotfolderProcessingInfoCompPtr->AddProcessingItem(inputFilePath, outputFilePath);

	m_filesQueue.pop_back();
}


// private methods

void CHotfolderProcessingComp::StartHotfolder()
{
	I_ASSERT(!BaseClass2::isRunning());

	m_finishThread = false;

	BaseClass2::start();
}


void CHotfolderProcessingComp::StopHotfolder()
{
	m_finishThread = true;

	// wait for 30 seconds for finishing of thread: 
	iqt::CTimer timer;
	while (timer.GetElapsed() < 30 && BaseClass2::isRunning());

	if (BaseClass2::isRunning()){
		BaseClass2::terminate();
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
	}

	isys::CSectionBlocker parameterLock(&m_parameterLock);
	if (m_processingParamsSetCompPtr.IsValid()){
		m_runParameterPtr.SetCastedOrRemove(m_processingParamsSetCompPtr->CloneMe());
	}

	if (!BaseClass2::isRunning()){
		StartHotfolder();
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


ifpf::IHotfolderProcessingItem* CHotfolderProcessingComp::GetNextProcessingFile() const
{
	I_ASSERT(m_hotfolderProcessingInfoCompPtr.IsValid());
	if (!m_hotfolderProcessingInfoCompPtr.IsValid()){
		return NULL;
	}
		
	int itemsCount = m_hotfolderProcessingInfoCompPtr->GetProcessingItemsCount();
	for (int itemIndex = 0; itemIndex < itemsCount; itemIndex++){
		ifpf::IHotfolderProcessingItem* processingItemPtr = m_hotfolderProcessingInfoCompPtr->GetProcessingItem(itemIndex);

		if (processingItemPtr->GetProcessingState() == iproc::IProcessor::TS_NONE){
			return processingItemPtr;
		}
	}

	return NULL;
}


int CHotfolderProcessingComp::ProcessFile(const istd::CString& inputFile, const istd::CString& outputFile)
{
	I_ASSERT(m_fileConvertCompPtr.IsValid());
	if (!m_fileConvertCompPtr.IsValid()){
		return iproc::IProcessor::TS_NONE;
	}

	istd::TDelPtr<iprm::IParamsSet> processingParameterPtr;

	m_parameterLock.Enter();
	if (m_runParameterPtr.IsValid()){
		processingParameterPtr.SetCastedOrRemove(m_runParameterPtr->CloneMe());
	}
	m_parameterLock.Leave();

	if (!m_fileConvertCompPtr->CopyFile(inputFile, outputFile, processingParameterPtr.GetPtr())){
		istd::CString message = istd::CString("Processing of ") + inputFile + " failed";
		SendErrorMessage(0, message, "Hotfolder");

		return iproc::IProcessor::TS_INVALID;
	}

	return iproc::IProcessor::TS_OK;
}


void CHotfolderProcessingComp::UpdateProcessingState(ifpf::IHotfolderProcessingItem* processingItemPtr, int processingState) const
{
	if (processingItemPtr != NULL){
		processingItemPtr->SetProcessingState(processingState);
	}
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
	
	if (monitorPtr != NULL && updateFlags & ifpf::IDirectoryMonitor::CF_FILES_ADDED){
		m_parent.OnInputFileEvent(*monitorPtr);
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

void CHotfolderProcessingComp::StateObserver::BeforeUpdate(imod::IModel* /*modelPtr*/, int updateFlags, istd::IPolymorphic* /*updateParamsPtr*/)
{
	if ((updateFlags & ifpf::IHotfolderProcessingInfo::CF_FILE_REMOVED) != 0){
		m_parent.m_processingQueueLock.Enter();
	}
}


void CHotfolderProcessingComp::StateObserver::AfterUpdate(imod::IModel* /*modelPtr*/, int updateFlags, istd::IPolymorphic* /*updateParamsPtr*/)
{
	if ((updateFlags & ifpf::IHotfolderProcessingInfo::CF_FILE_REMOVED) != 0 || (updateFlags & istd::IChangeable::CF_ABORTED) != 0){
		m_parent.m_processingQueueLock.Leave();
	}
}


} // namespace iqtfpf


