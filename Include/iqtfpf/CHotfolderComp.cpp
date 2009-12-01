#include "iqtfpf/CHotfolderComp.h"


// Qt includes
#include <QApplication>


// ACF includes
#include "istd/TChangeDelegator.h"
#include "istd/CStaticServicesProvider.h"

#include "isys/IFileSystem.h"
#include "isys/CSectionBlocker.h"

#include "iprm/IFileNameParam.h"
#include "iprm/IParamsManager.h"

#include "iproc/IProcessor.h"

#include "iqt/CTimer.h"


namespace iqtfpf
{


// public methods

CHotfolderComp::CHotfolderComp()
	:m_directoryMonitorObserver(*this),
	m_parametersObserver(*this)
{
}


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

	BaseClass2::SetParams(m_paramsSetCompPtr.GetPtr());

	I_ASSERT(m_paramsSetModelCompPtr.IsValid());
	if (m_paramsSetModelCompPtr.IsValid()){
		m_paramsSetModelCompPtr->AttachObserver(&m_parametersObserver);
	}
}


void CHotfolderComp::OnComponentDestroyed()
{
	BaseClass::OnComponentDestroyed();

	StopHotfolder();

	I_ASSERT(m_paramsSetModelCompPtr.IsValid());
	if (m_paramsSetModelCompPtr.IsValid()){
		m_paramsSetModelCompPtr->DetachObserver(&m_parametersObserver);
	}
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

		ProcessingItem* processingItemPtr = new ProcessingItem;

		processingItemPtr->SetInputFile(inputFilePath);
		processingItemPtr->SetOutputFile(outputFilePath);
		processingItemPtr->SetProcessingState(iproc::IProcessor::TS_NONE);

		isys::CSectionBlocker queueBlocker(&m_processingQueueLock);

		// add file to hotfolder model:
		AddFile(processingItemPtr);
	}

	return true;
}


// reimplemented (QThread)

void CHotfolderComp::run()
{
	while (!m_finishThread){
		m_processingQueueLock.Enter();
		if (GetProcessingItemsCount() == 0){
			m_processingQueueLock.Leave();

			msleep(50);
			continue;
		}

		// get available file to process:
		const ifpf::IHotfolderProcessingItem* processingItemPtr = GetNextProcessingFile();
		if (processingItemPtr != NULL){
			istd::CString inputFile = processingItemPtr->GetInputFile();
			istd::CString outputFile = processingItemPtr->GetOutputFile();
			
			m_processingQueueLock.Leave();
		
			isys::CSectionBlocker parameterLock(&m_parameterLock);

			if (!m_fileConvertCompPtr->CopyFile(inputFile, outputFile, m_runParameterPtr.GetPtr())){
				istd::CString message = istd::CString("Processing of ") + outputFile + "failed";

				SendErrorMessage(0, message, "Hotfolder");
			}
		}

		m_processingQueueLock.Leave();

		msleep(10);
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
	m_finishThread = true;

	// wait for 30 seconds for finishing of thread: 
	iqt::CTimer timer;
	while (timer.GetElapsed() < 30 && BaseClass3::isRunning());

	if (BaseClass3::isRunning()){
		BaseClass3::terminate();
	}
}


void CHotfolderComp::SynchronizeWithModel(bool /*applyToPendingTasks*/)
{
	I_ASSERT(m_inputPathParamsManagerIdAttrPtr.IsValid());
	I_ASSERT(m_monitoringParamsIdAttrPtr.IsValid());
	I_ASSERT(m_paramsSetCompPtr.IsValid());

	StopHotfolder();

	// Initialize the directory monitoring:
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

				istd::CString monitoringDirectoryPath = monitoringDirectoryPtr->GetPath();

				// create monitor:
				istd::TDelPtr<icomp::IComponent> monitorCompPtr(m_monitorFactCompPtr.CreateComponent());
				if (monitorCompPtr.IsValid()){
					ifpf::IDirectoryMonitor* directoryMonitorPtr = m_monitorFactCompPtr.ExtractInterface(monitorCompPtr.GetPtr());
					if (directoryMonitorPtr != NULL){
						m_directoryMonitorsMap[monitoringDirectoryPath] = directoryMonitorPtr;
					
						monitorCompPtr.PopPtr();

						imod::IModel* directoryMonitorModelPtr = dynamic_cast<imod::IModel*>(directoryMonitorPtr);
						I_ASSERT(directoryMonitorModelPtr != NULL);

						directoryMonitorModelPtr->AttachObserver(&m_directoryMonitorObserver);

						directoryMonitorPtr->StartObserving(inputDirectoryParamPtr);
					}
				}
			}
		}
	}

	StartHotfolder();
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


// public methods of embedded class ParametersObserver

CHotfolderComp::ParametersObserver::ParametersObserver(CHotfolderComp& parent)
	:m_parent(parent)
{
}


// reimplemented (imod::IObserver)
	
void CHotfolderComp::ParametersObserver::AfterUpdate(imod::IModel* /*modelPtr*/, int updateFlags, istd::IPolymorphic* /*updateParamsPtr*/)
{
	if ((updateFlags & istd::CChangeDelegator::CF_DELEGATED) != 0){
		m_parent.SynchronizeWithModel();
	}
}


} // namespace iqtfpf


