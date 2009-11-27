#include "iqtfpf/CHotfolderComp.h"


// Qt includes
#include <QApplication>


// ACF includes
#include "istd/CStaticServicesProvider.h"

#include "isys/IFileSystem.h"
#include "isys/CSectionBlocker.h"

#include "iprm/IFileNameParam.h"

#include "iqt/CTimer.h"


namespace iqtfpf
{


// public methods

// reimplemented (ifpf::IFileNamingStrategy)

istd::CString CHotfolderComp::GetFileName(const istd::CString& inputFileName) const
{
	std::string paramId;
	if (m_outputDirectoryParamsIdAttrPtr.IsValid()){
		paramId = (*m_outputDirectoryParamsIdAttrPtr).ToString();
	}

	const iprm::IFileNameParam* outputDirectoryPtr = dynamic_cast<const iprm::IFileNameParam*>(m_paramsSetCompPtr->GetParameter(paramId));
	if (outputDirectoryPtr != NULL && outputDirectoryPtr->GetPathType() == iprm::IFileNameParam::PT_DIRECTORY){
		isys::IFileSystem* fileSystemPtr = istd::GetService<isys::IFileSystem>();
		if (fileSystemPtr != NULL){
			istd::CString fileName = fileSystemPtr->GetFileName(inputFileName);

			return outputDirectoryPtr->GetPath() + "/" + fileName;
		}
	}

	return istd::CString();
}


// reimplemented (ifpf::IMonitoringSessionManager)

ifpf::IMonitoringSession* CHotfolderComp::GetSession(const ifpf::IDirectoryMonitor& directoryMonitor, const istd::CString& directoryPath) const
{
	DirectoryMonitorsMap::const_iterator foundIter = m_directoryMonitorsMap.find(directoryPath);
	if (foundIter == m_directoryMonitorsMap.end()){
		return NULL;
	}

	if (foundIter->second.GetPtr() != &directoryMonitor){
		return NULL;
	}

	MonitoringSessionsMap::const_iterator sessionIter = m_monitoringSessionsMap.find(directoryPath);
	if (sessionIter == m_monitoringSessionsMap.end()){
		return NULL;
	}

	return sessionIter->second.GetPtr();
}


// reimplemeneted (icomp::IComponent)

void CHotfolderComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	SynchronizeWithModel();
}


void CHotfolderComp::OnComponentDestroyed()
{
	BaseClass::OnComponentDestroyed();

	StopHotfolder();
}



// protected methods

bool CHotfolderComp::OnIncommingInputFileEvent(const ifpf::IDirectoryMonitor& directoryMonitor)
{
	if (!m_fileConvertCompPtr.IsValid()){
		SendErrorMessage(0, "File conversion component was not set", "Hotfolder");

		return false;
	}

	if (!m_fileNamingStrategyCompPtr.IsValid()){
		SendErrorMessage(0, "File naming component was not set", "Hotfolder");

		return false;
	}

	istd::CStringList files = directoryMonitor.GetChangedFileItems(ifpf::IDirectoryMonitor::CF_FILES_ADDED);

	for (int fileIndex = 0; fileIndex < int(files.size()); fileIndex++){
		const istd::CString& inputFilePath = files[fileIndex];

		isys::IFileSystem* fileSystemPtr = istd::GetService<isys::IFileSystem>();
		if (fileSystemPtr != NULL){
			if (!fileSystemPtr->IsPresent(inputFilePath)){
				continue;
			}
		}
		istd::CString outputFilePath = m_fileNamingStrategyCompPtr->GetFileName(inputFilePath);

		QueueItem newProcessingItem;
		newProcessingItem.inputFile = inputFilePath;
		newProcessingItem.outputFile = outputFilePath;

		isys::CSectionBlocker queueBlocker(&m_processingQueueLock);
		
		m_processingQueue.push_front(newProcessingItem);
	}

	return true;
}


// reimplemented (QThread)

void CHotfolderComp::run()
{
	while (!m_finishThread){
		m_processingQueueLock.Enter();
		if (m_processingQueue.empty()){
			m_processingQueueLock.Leave();

			msleep(50);
			continue;
		}

		QueueItem processingItem = m_processingQueue.back();
		m_processingQueue.pop_back();
			
		m_processingQueueLock.Leave();
		
		isys::CSectionBlocker parameterLock(&m_parameterLock);

		if (!m_fileConvertCompPtr->CopyFile(processingItem.inputFile, processingItem.outputFile, m_runParameterPtr.GetPtr())){
			istd::CString message = istd::CString("Processing of ") + processingItem.outputFile + "failed";

			SendErrorMessage(0, message, "Hotfolder");
		}
	}
}


// private methods

void CHotfolderComp::StartHotfolder()
{
	m_finishThread = false;

	BaseClass3::start();
}


void CHotfolderComp::StopHotfolder()
{
	m_finishThread = false;

	// wait for 30 seconds for finishing of thread: 
	iqt::CTimer timer;
	while (timer.GetElapsed() < 30 && BaseClass3::isRunning()){
		QApplication::processEvents(QEventLoop::ExcludeUserInputEvents);
	}

	if (BaseClass3::isRunning()){
		BaseClass3::terminate();
	}
}


void CHotfolderComp::SynchronizeWithModel(bool /*applyToPendingTasks*/)
{
	
}


// public methods of embedded class DirectoryMonitorObserver

CHotfolderComp::DirectoryMonitorObserver::DirectoryMonitorObserver(CHotfolderComp& parent)
	:m_parent(parent)
{
}


// reimplemented (imod::IObserver)
	
void CHotfolderComp::DirectoryMonitorObserver::AfterUpdate(imod::IModel* modelPtr, int updateFlags, istd::IPolymorphic* /*updateParamsPtr*/)
{
	ifpf::IDirectoryMonitor* monitorPtr = dynamic_cast<ifpf::IDirectoryMonitor*>(modelPtr);
	
	if (monitorPtr != NULL && updateFlags & ifpf::IDirectoryMonitor::CF_FILES_ADDED){
		m_parent.OnIncommingInputFileEvent(*monitorPtr);
	}
}


} // namespace iqtfpf


