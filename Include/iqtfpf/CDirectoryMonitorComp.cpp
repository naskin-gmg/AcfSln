#include "iqtfpf/CDirectoryMonitorComp.h"


// Qt includes
#include <QDir>
#include <QApplication>


// ACF includes
#include "istd/TChangeNotifier.h"

#include "imod/IModel.h"

#include "isys/CSectionBlocker.h"

#include "iqt/CTimer.h"


namespace iqtfpf
{


CDirectoryMonitorComp::CDirectoryMonitorComp()
	:m_finishThread(false),
	m_poolingFrequency(5.0),
	m_observingItemTypes(ifpf::IDirectoryMonitorParams::OI_ALL),
	m_observingChanges(ifpf::IDirectoryMonitorParams::OC_ALL),
	m_directoryPathModelPtr(NULL),
	m_directoryMonitorParamsModelPtr(NULL),
	m_directoryPathPtr(NULL),
	m_directoryMonitorParamsPtr(NULL)
{
}


// reimplemented (ifpf::IDirectoryMonitor)

istd::CStringList CDirectoryMonitorComp::GetChangedFileItems(int changeFlags) const
{
	isys::CSectionBlocker block(&m_lock);
	
	istd::CStringList changedFilesList;
	if ((changeFlags & CF_FILES_ADDED) != 0){
		istd::CStringList addedFiles = iqt::GetCStringList(m_folderChanges.addedFiles);
		changedFilesList.insert(changedFilesList.end(), addedFiles.begin(), addedFiles.end()); 
	}

	if ((changeFlags & CF_FILES_REMOVED) != 0){
		istd::CStringList removedFiles = iqt::GetCStringList(m_folderChanges.removedFiles);
		changedFilesList.insert(changedFilesList.end(), removedFiles.begin(), removedFiles.end()); 
	}

	if ((changeFlags & CF_FILES_MODIFIED) != 0){
		istd::CStringList modifiedFiles = iqt::GetCStringList(m_folderChanges.modifiedFiles);
		changedFilesList.insert(changedFilesList.end(), modifiedFiles.begin(), modifiedFiles.end()); 
	}

	if ((changeFlags & CF_FILES_ATTRIBUTE_CHANGED) != 0){
		istd::CStringList attributeChangedFiles = iqt::GetCStringList(m_folderChanges.attributeChangedFiles);
		changedFilesList.insert(changedFilesList.end(), attributeChangedFiles.begin(), attributeChangedFiles.end()); 
	}

	return changedFilesList;
}


bool CDirectoryMonitorComp::StartObserving(const iprm::IParamsSet* paramsSetPtr)
{
	bool wasRunning = BaseClass2::isRunning(); 
	if (wasRunning){
		StopObserverThread();
	}

	if ((paramsSetPtr == NULL) && (!m_paramsSetCompPtr.IsValid() || !m_paramsSetModelCompPtr.IsValid())){
		SendInfoMessage(0, "Directory monitoring parameters are invalid or not set");
	
		return false;
	}

	// if external parameter model is used, connect to it, otherwise use the default parameter set:
	const iprm::IParamsSet* parameterModelPtr = paramsSetPtr == NULL ? m_paramsSetCompPtr.GetPtr() : paramsSetPtr;
	I_ASSERT(parameterModelPtr != NULL);

	// connect to own parameter model:
	if (ConnectToParameterModel(*parameterModelPtr)){
		SynchronizeWithModel(*m_directoryPathModelPtr);
		SynchronizeWithModel(*m_directoryMonitorParamsModelPtr);

		StartObserverThread();

		return true;
	}

	SendInfoMessage(0, "Directory monitoring parameters could not be set");

	return false;
}


void CDirectoryMonitorComp::StopObserving()
{
	StopObserverThread();
}


// reimplemented (ibase::IFileListProvider)

istd::CStringList CDirectoryMonitorComp::GetFileList() const
{
	istd::CStringList fileList;

	isys::CSectionBlocker block(&m_lock);

	for (int fileIndex = 0; fileIndex < m_directoryFiles.count(); fileIndex++){
		fileList.push_back(iqt::GetCString(m_directoryFiles[fileIndex].absoluteFilePath()));
	}

	return fileList;
}


// reimplemented (imod::IObserver)

bool CDirectoryMonitorComp::OnDetached(imod::IModel* modelPtr)
{
	if (modelPtr == m_directoryPathModelPtr){
		m_directoryPathModelPtr = NULL;
	}

	if (modelPtr == m_directoryMonitorParamsModelPtr){
		m_directoryMonitorParamsModelPtr = NULL;
	}

	return BaseClass3::OnDetached(modelPtr);
}


void CDirectoryMonitorComp::AfterUpdate(imod::IModel* modelPtr, int /*updateFlags*/, istd::IPolymorphic* /*updateParamsPtr*/)
{
	I_ASSERT(modelPtr != NULL);
	if (modelPtr != NULL){
		bool needRestart = false; 
		if (modelPtr == m_directoryPathModelPtr){
			needRestart = BaseClass2::isRunning();
			if (needRestart){
				StopObserverThread();
			}
		}

		SynchronizeWithModel(*modelPtr);

		// if observing thread was running -> restart:
		if (needRestart){
			StartObserverThread();
		}
	}
}


// reimplemented (icomp::IComponent)

void CDirectoryMonitorComp::OnComponentCreated()
{
	I_ASSERT(m_directoryPathIdAttrPtr.IsValid());
	I_ASSERT(m_directoryMonitorParamsIdAttrPtr.IsValid());


	BaseClass::OnComponentCreated();

	connect(this, SIGNAL(FolderChanged(int)), this, SLOT(OnFolderChanged(int)), Qt::QueuedConnection);

	if (m_autoStartAttrPtr.IsValid() && *m_autoStartAttrPtr){
		StartObserving();
	}
}


void CDirectoryMonitorComp::OnComponentDestroyed()
{
	StopObserving();

	DisconnectFromParameterModel();

	BaseClass::OnComponentDestroyed();
}


// protected methods

// reimplemented (QThread)

void CDirectoryMonitorComp::run()
{
	iqt::CTimer updateTimer;
	
	while (!m_finishThread){
		bool needStateUpdate = (updateTimer.GetElapsed() >= m_poolingFrequency);
		if (!needStateUpdate){
			msleep(100);

			continue;
		}

		QFileInfo fileInfo(m_currentDirectory.absolutePath());
		if (!fileInfo.exists()){
			msleep(100);

			continue;
		}

		// set parameters and do file search:
		m_lock.Enter();

		int observingChanges = m_observingChanges;
		QStringList acceptPatterns = m_fileFilterExpressions;
		int observingItemTypes = m_observingItemTypes;

		m_lock.Leave();

		QFileInfoList currentFiles = m_currentDirectory.entryInfoList(acceptPatterns, QDir::Filters(observingItemTypes) | QDir::NoDotAndDotDot);

		QStringList addedFiles;
		QStringList removedFiles;
		QStringList modifiedFiles;
		QStringList attributeChangedFiles;

		// check for changes:
		if ((observingChanges & ifpf::IDirectoryMonitorParams::OC_REMOVE) != 0){
			for (int fileIndex = 0; fileIndex < int(m_directoryFiles.count()); fileIndex++){
				QFileInfo fileInfo = m_directoryFiles[fileIndex];				
				fileInfo.refresh();

				if (!fileInfo.exists()){
					removedFiles.push_back(fileInfo.absoluteFilePath());
					SendInfoMessage(0, iqt::GetCString(fileInfo.absoluteFilePath() + " was removed"));
				}
			}
		}

		if (observingChanges != ifpf::IDirectoryMonitorParams::OC_REMOVE){
			for (int currentFileIndex = 0; currentFileIndex < int(currentFiles.count()); currentFileIndex++){
				const QFileInfo& currentFileInfo = currentFiles[currentFileIndex];

				QFileInfoList::iterator foundFileIter = qFind(m_directoryFiles.begin(), m_directoryFiles.end(), currentFileInfo);
				if (foundFileIter == m_directoryFiles.end()){
					if ((observingChanges & ifpf::IDirectoryMonitorParams::OC_ADD) != 0){
						addedFiles.push_back(currentFileInfo.absoluteFilePath());
						SendInfoMessage(0, iqt::GetCString(currentFileInfo.absoluteFilePath() + " was added"));
					}
				}
				else{
					(*foundFileIter).refresh();

					if (foundFileIter->lastModified() != currentFileInfo.lastModified()){
						if ((observingChanges & ifpf::IDirectoryMonitorParams::OC_MODIFIED) != 0){
							modifiedFiles.push_back(currentFileInfo.absoluteFilePath());
							SendInfoMessage(0, iqt::GetCString(currentFileInfo.absoluteFilePath() + " was modified"));
						}
					}
					if (foundFileIter->permissions() != currentFileInfo.permissions()){
						if ((observingChanges & ifpf::IDirectoryMonitorParams::OC_ATTR_CHANGED) != 0){
							attributeChangedFiles.push_back(currentFileInfo.absoluteFilePath());
							SendInfoMessage(0, istd::CString("Attributes of") + iqt::GetCString(currentFileInfo.absoluteFilePath() + " have been changed"));
						}
					}
				}
			}
		}

		isys::CSectionBlocker block(&m_lock);

		m_directoryFiles = currentFiles;

		m_folderChanges.addedFiles = addedFiles;
		m_folderChanges.removedFiles = removedFiles;
		m_folderChanges.modifiedFiles = modifiedFiles;
		m_folderChanges.attributeChangedFiles = attributeChangedFiles;

		int changeFlags = 0;

		if (!addedFiles.isEmpty()){
			changeFlags |= CF_FILES_ADDED;
		}

		if (!removedFiles.isEmpty()){
			changeFlags |= CF_FILES_REMOVED;
		}

		if (!modifiedFiles.isEmpty()){
			changeFlags |= CF_FILES_MODIFIED;
		}

		if (!attributeChangedFiles.isEmpty()){
			changeFlags |= CF_FILES_ATTRIBUTE_CHANGED;
		}

		Q_EMIT FolderChanged(changeFlags);
		
		updateTimer.Start();
	}
}


// private slots

void CDirectoryMonitorComp::OnFolderChanged(int changeFlags)
{
	if ((changeFlags & CF_SOME_CHANGES) != 0){
		// notify observers:
		istd::CChangeNotifier changePtr(this, changeFlags);

		changePtr.Reset();

		// update current session:
		if (m_monitoringSessionManagerCompPtr.IsValid()){
			ifpf::IMonitoringSession* sessionPtr = m_monitoringSessionManagerCompPtr->GetSession(*this, iqt::GetCString(m_currentDirectory.absolutePath()));
			if (sessionPtr != NULL){
				sessionPtr->SetFileList(GetFileList());
			}
		}
	}
}


// private methods

void CDirectoryMonitorComp::SetFolderPath(const QString& folderPath)
{	
	I_ASSERT(!BaseClass2::isRunning());

	if (m_currentDirectory == QDir(folderPath)){
		return;
	}

	if (folderPath.isEmpty()){
		return;
	}

	ResetFiles();

	m_currentDirectory = QDir(folderPath);

	if (m_monitoringSessionManagerCompPtr.IsValid()){
		ifpf::IMonitoringSession* sessionPtr = m_monitoringSessionManagerCompPtr->GetSession(*this, iqt::GetCString(m_currentDirectory.absolutePath()));
		if (sessionPtr != NULL){
			istd::CStringList sessionFiles = sessionPtr->GetFileList();
			for (int fileIndex = 0; fileIndex < int(sessionFiles.size()); fileIndex++){
				m_directoryFiles.push_back(QFileInfo(iqt::GetQString(sessionFiles[fileIndex])));
			}
		}
	}

	if (m_directoryMonitorParamsPtr != NULL){
		m_poolingFrequency = m_directoryMonitorParamsPtr->GetPoolingIntervall();
		m_observingItemTypes = m_directoryMonitorParamsPtr->GetObservedItemTypes();
		m_observingChanges = m_directoryMonitorParamsPtr->GetObservedChanges();
		m_fileFilterExpressions = iqt::GetQStringList(m_directoryMonitorParamsPtr->GetAcceptPatterns());
	}
}


void CDirectoryMonitorComp::StartObserverThread()
{
	m_finishThread = false;

	QFileInfo fileInfo(m_currentDirectory.absolutePath());
	if (fileInfo.exists()){
		SendInfoMessage(0, istd::CString("Start observing of: ") + iqt::GetCString(m_currentDirectory.absolutePath()), "DirectoryMonitor");

		BaseClass2::start(QThread::LowPriority);
	}
}


void CDirectoryMonitorComp::StopObserverThread()
{
	m_finishThread = true;

	// wait for 30 seconds for finishing of thread: 
	iqt::CTimer timer;
	while (timer.GetElapsed() < 30 && BaseClass2::isRunning());
	if (BaseClass2::isRunning()){
		BaseClass2::terminate();
	}
}


void CDirectoryMonitorComp::ResetFiles()
{
	m_directoryFiles.clear();
	m_folderChanges.addedFiles.clear();
	m_folderChanges.attributeChangedFiles.clear();
	m_folderChanges.modifiedFiles.clear();
	m_folderChanges.removedFiles.clear();
}


void CDirectoryMonitorComp::SynchronizeWithModel(const imod::IModel& paramsModel)
{
	I_ASSERT(m_directoryPathPtr != NULL);
	I_ASSERT(m_directoryPathModelPtr != NULL);
	I_ASSERT(m_directoryMonitorParamsPtr != NULL);
	I_ASSERT(m_directoryMonitorParamsModelPtr != NULL);

	if (&paramsModel == m_directoryPathModelPtr){
		QString currentPath = iqt::GetQString(m_directoryPathPtr->GetPath());

		if (m_currentDirectory != QDir(currentPath)){
			SetFolderPath(currentPath);
		}
	}

	if (&paramsModel == m_directoryMonitorParamsModelPtr){
		isys::CSectionBlocker block(&m_lock);

		m_poolingFrequency = m_directoryMonitorParamsPtr->GetPoolingIntervall();
		m_observingItemTypes = m_directoryMonitorParamsPtr->GetObservedItemTypes();
		m_observingChanges = m_directoryMonitorParamsPtr->GetObservedChanges();
		m_fileFilterExpressions = iqt::GetQStringList(m_directoryMonitorParamsPtr->GetAcceptPatterns());
	}
}


bool CDirectoryMonitorComp::ConnectToParameterModel(const iprm::IParamsSet& paramsSet)
{
	DisconnectFromParameterModel();

	m_directoryPathModelPtr = dynamic_cast<const imod::IModel*>(paramsSet.GetParameter((*m_directoryPathIdAttrPtr).ToString()));
	m_directoryMonitorParamsModelPtr = dynamic_cast<const imod::IModel*>(paramsSet.GetParameter((*m_directoryMonitorParamsIdAttrPtr).ToString()));
	m_directoryPathPtr = dynamic_cast<const iprm::IFileNameParam*>(m_directoryPathModelPtr);
	m_directoryMonitorParamsPtr = dynamic_cast<const ifpf::IDirectoryMonitorParams*>(m_directoryMonitorParamsModelPtr);

	I_ASSERT(m_directoryPathModelPtr != NULL);
	I_ASSERT(m_directoryMonitorParamsModelPtr != NULL);

	if (m_directoryPathModelPtr != NULL && m_directoryMonitorParamsModelPtr != NULL){
		bool retVal = (const_cast<imod::IModel*>(m_directoryPathModelPtr))->AttachObserver(this);

		retVal = retVal && (const_cast<imod::IModel*>(m_directoryMonitorParamsModelPtr))->AttachObserver(this);

		return retVal;
	}

	return false;
}


void CDirectoryMonitorComp::DisconnectFromParameterModel()
{
	if (m_directoryPathModelPtr != NULL){
		if (m_directoryPathModelPtr->IsAttached(this)){
			(const_cast<imod::IModel*>(m_directoryPathModelPtr))->DetachObserver(this);
		}
	}

	if (m_directoryMonitorParamsModelPtr != NULL){
		if (m_directoryMonitorParamsModelPtr->IsAttached(this)){
			(const_cast<imod::IModel*>(m_directoryMonitorParamsModelPtr))->DetachObserver(this);
		}
	}
}


} // namespace iqtfpf


