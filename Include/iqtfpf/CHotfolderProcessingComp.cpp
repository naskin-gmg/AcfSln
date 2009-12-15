#include "iqtfpf/CHotfolderProcessingComp.h"


// STL includes
#include <algorithm>


// Qt includes
#include <QDir>


// ACF includes
#include "istd/INamed.h"
#include "istd/TChangeNotifier.h"
#include "istd/TChangeDelegator.h"
#include "istd/CStaticServicesProvider.h"

#include "isys/IApplicationEnvironment.h"
#include "isys/CSectionBlocker.h"

#include "iser/CXmlFileWriteArchive.h"

#include "iprm/IFileNameParam.h"
#include "iprm/IParamsManager.h"

#include "iproc/IProcessor.h"

#include "iqt/CTimer.h"
#include "iqt/CSafeNotifier.h"


namespace iqtfpf
{


// public methods

CHotfolderProcessingComp::CHotfolderProcessingComp()
	:m_directoryMonitorObserver(*this),
	m_parametersObserver(*this)
{
}


// reimplemeneted (icomp::IComponent)

void CHotfolderProcessingComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	I_ASSERT(m_paramsSetModelCompPtr.IsValid());
	if (m_paramsSetModelCompPtr.IsValid()){
		m_paramsSetModelCompPtr->AttachObserver(&m_parametersObserver);
	}

	connect(this,
				SIGNAL(EmitItemState(ifpf::IHotfolderProcessingItem*, int)),
				this,
				SLOT(OnItemState(ifpf::IHotfolderProcessingItem*, int)), Qt::QueuedConnection);

	connect(&m_filesQueueTimer, SIGNAL(timeout()), this, SLOT(OnUpdateQueueTimer()));

	m_filesQueueTimer.start(100);
}


void CHotfolderProcessingComp::OnComponentDestroyed()
{
	BaseClass::OnComponentDestroyed();

	StopHotfolder();

	I_ASSERT(m_paramsSetModelCompPtr.IsValid());
	if (m_paramsSetModelCompPtr.IsValid()){
		m_paramsSetModelCompPtr->DetachObserver(&m_parametersObserver);
	}

	m_filesQueueTimer.stop();
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
	I_ASSERT(m_hotfolderCompPtr.IsValid());

	int workingIntervall = 500; // ms

	while (!m_finishThread){
		if (!m_hotfolderCompPtr->IsWorking()){
			msleep(workingIntervall);
			continue;					
		}

		isys::CSectionBlocker processingQueueLock(&m_processingQueueLock);
		
		// get available file to process:
		ifpf::IHotfolderProcessingItem* processingItemPtr = m_hotfolderCompPtr->GetNextProcessingFile();
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

	istd::CString outputFilePath = m_fileNamingCompPtr->GetFilePath(inputFilePath, GetOutputDirectory());

	isys::CSectionBlocker queueBlocker(&m_processingQueueLock);

	I_ASSERT(m_hotfolderCompPtr.IsValid());

	// add file to hotfolder's model:
	m_hotfolderCompPtr->AddProcessingItem(inputFilePath, outputFilePath);

	m_filesQueue.pop_back();
}


// private methods

void CHotfolderProcessingComp::StartHotfolder()
{
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
	I_ASSERT(m_inputPathParamsManagerIdAttrPtr.IsValid());
	I_ASSERT(m_monitoringParamsIdAttrPtr.IsValid());
	I_ASSERT(m_paramsSetCompPtr.IsValid());

	StopHotfolder();

	// setup directory monitoring:
	istd::CStringList addedDirectories = GetAddedInputDirectories();
	for (int pathIndex = 0; pathIndex < int(addedDirectories.size()); pathIndex++){
		const iprm::IParamsSet* monitoringParamsPtr = GetMonitoringParamsSet(pathIndex);
		if (monitoringParamsPtr != NULL){
			AddDirectoryMonitor(addedDirectories[pathIndex], monitoringParamsPtr);
		}
	}
	
	istd::CStringList removedDirectories = GetRemovedInputDirectories();
	for (int pathIndex = 0; pathIndex < int(removedDirectories.size()); pathIndex++){
		RemoveDirectoryMonitor(removedDirectories[pathIndex]);
	}

	StartHotfolder();
}


istd::CString CHotfolderProcessingComp::GetOutputDirectory() const
{
	std::string paramId;
	if (m_outputDirectoryParamsIdAttrPtr.IsValid()){
		paramId = (*m_outputDirectoryParamsIdAttrPtr).ToString();
	}

	const iprm::IFileNameParam* outputDirectoryPtr = dynamic_cast<const iprm::IFileNameParam*>(m_paramsSetCompPtr->GetParameter(paramId));
	if (outputDirectoryPtr != NULL && outputDirectoryPtr->GetPathType() == iprm::IFileNameParam::PT_DIRECTORY){
		return outputDirectoryPtr->GetPath();
	}

	return istd::CString();
}


istd::CStringList CHotfolderProcessingComp::GetInputDirectories() const
{
	istd::CStringList inputDirectories;

	if (m_inputPathParamsManagerIdAttrPtr.IsValid() && m_paramsSetCompPtr.IsValid()){
		const iprm::IParamsManager* directoryParamsManagerPtr = 
					dynamic_cast<const iprm::IParamsManager*>(m_paramsSetCompPtr->GetParameter((*m_inputPathParamsManagerIdAttrPtr).ToString()));

		const iprm::IParamsSet* directoryMonitoringParamsPtr = 
					dynamic_cast<const iprm::IParamsSet*>(m_paramsSetCompPtr->GetParameter((*m_monitoringParamsIdAttrPtr).ToString()));

		if (directoryParamsManagerPtr != NULL && directoryMonitoringParamsPtr != NULL){			
			int inputDirectoriesCount = directoryParamsManagerPtr->GetSetsCount();
			for (int inputIndex = 0; inputIndex < inputDirectoriesCount; inputIndex++){
				iprm::IParamsSet* inputDirectoryParamPtr = directoryParamsManagerPtr->GetParamsSet(inputIndex);
				I_ASSERT(inputDirectoryParamPtr != NULL);

				const iprm::IFileNameParam* monitoringDirectoryPtr = dynamic_cast<const iprm::IFileNameParam*>(inputDirectoryParamPtr->GetParameter("DirectoryPath"));
				I_ASSERT(monitoringDirectoryPtr != NULL);
				I_ASSERT(monitoringDirectoryPtr->GetPathType() == iprm::IFileNameParam::PT_DIRECTORY);

				inputDirectories.push_back(monitoringDirectoryPtr->GetPath());
			}
		}
	}

	return inputDirectories;
}


const iprm::IParamsSet* CHotfolderProcessingComp::GetMonitoringParamsSet(int index) const
{
	if (m_inputPathParamsManagerIdAttrPtr.IsValid() && m_paramsSetCompPtr.IsValid()){
		const iprm::IParamsManager* directoryParamsManagerPtr = 
					dynamic_cast<const iprm::IParamsManager*>(m_paramsSetCompPtr->GetParameter((*m_inputPathParamsManagerIdAttrPtr).ToString()));

		const iprm::IParamsSet* directoryMonitoringParamsPtr = 
					dynamic_cast<const iprm::IParamsSet*>(m_paramsSetCompPtr->GetParameter((*m_monitoringParamsIdAttrPtr).ToString()));

		if (directoryParamsManagerPtr != NULL && directoryMonitoringParamsPtr != NULL){			
			int inputDirectoriesCount = directoryParamsManagerPtr->GetSetsCount();
			I_ASSERT(index < inputDirectoriesCount);
			I_ASSERT(index >= 0);

			return directoryParamsManagerPtr->GetParamsSet(index);
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


istd::CString CHotfolderProcessingComp::GetHotfolderId() const
{
	if (m_paramsSetCompPtr.IsValid()){
		const iprm::IParamsSet* uuidParamsSet = dynamic_cast<const iprm::IParamsSet*>(m_paramsSetCompPtr->GetParameter("HotfolderId"));
		if (uuidParamsSet != NULL){
			const istd::INamed* uuidPtr = dynamic_cast<const istd::INamed*>(uuidParamsSet->GetParameter("HotfolderId"));
			if (uuidPtr != NULL){
				return uuidPtr->GetName();
			}
		}
	}

	return istd::CString();
}


int CHotfolderProcessingComp::ProcessFile(const istd::CString& inputFile, const istd::CString& outputFile)
{
	I_ASSERT(m_fileConvertCompPtr.IsValid());
	if (!m_fileConvertCompPtr.IsValid()){
		return iproc::IProcessor::TS_NONE;
	}

	isys::CSectionBlocker parameterLock(&m_parameterLock);

	if (!m_fileConvertCompPtr->CopyFile(inputFile, outputFile, m_runParameterPtr.GetPtr())){
		istd::CString message = istd::CString("Processing of ") + inputFile + " failed";
		SendErrorMessage(0, message, "Hotfolder");

		return iproc::IProcessor::TS_INVALID;
	}

	return iproc::IProcessor::TS_OK;
}


void CHotfolderProcessingComp::UpdateProcessingState(ifpf::IHotfolderProcessingItem* processingItemPtr, int processingState) const
{
	if (processingItemPtr != NULL){
		istd::CChangeNotifier changePtr(processingItemPtr);

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
	
void CHotfolderProcessingComp::ParametersObserver::AfterUpdate(imod::IModel* /*modelPtr*/, int updateFlags, istd::IPolymorphic* /*updateParamsPtr*/)
{
	if ((updateFlags & istd::CChangeDelegator::CF_DELEGATED) != 0){
		m_parent.SynchronizeWithModel();
	}
}


} // namespace iqtfpf


