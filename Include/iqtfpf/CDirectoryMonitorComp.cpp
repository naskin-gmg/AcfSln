#include "iqtfpf/CDirectoryMonitorComp.h"


#include <io.h>


// Qt includes
#include <QDir>
#include <QApplication>
#include <QUuid>


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
	m_monitoringParamsObserver(*this),
	m_directoryParamsObserver(*this),
	m_directoryPendingChangesCounter(1)
{
	connect(&m_directoryWatcher, SIGNAL(directoryChanged(const QString&)), this, SLOT(OnDirectoryChangeNotification(const QString&)));
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

	for (int fileIndex = 0; fileIndex < int(m_directoryFiles.size()); fileIndex++){
		fileList.push_back(iqt::GetCString(m_directoryFiles[fileIndex].filePath));
	}

	return fileList;
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

	UpdateMonitoringSession();

	BaseClass::OnComponentDestroyed();
}


// protected methods

// reimplemented (QThread)

void CDirectoryMonitorComp::run()
{
	iqt::CTimer updateTimer;

	iqt::CTimer measurementTimer;
	
	while (!m_finishThread){
		bool needStateUpdate = (updateTimer.GetElapsed() >= m_poolingFrequency);
		if (!needStateUpdate){
			msleep(100);

			continue;
		}

		measurementTimer.Start();

		QFileInfo currentDirectoryInfo(m_currentDirectory.absolutePath());
		if (!currentDirectoryInfo.exists()){
			msleep(100);

			continue;
		}

		// set parameters and do file search:
		m_lock.Enter();

		int observingChanges = m_observingChanges;
		QStringList acceptPatterns = m_fileFilterExpressions;
		int observingItemTypes = m_observingItemTypes;
		int pendingChangesCounter = m_directoryPendingChangesCounter;
		m_directoryPendingChangesCounter = 0;

		m_lock.Leave();

		QStringList addedFiles;
		QStringList removedFiles;
		QStringList modifiedFiles;
		QStringList attributeChangedFiles;

		// check for changes:
		if ((observingChanges & ifpf::IDirectoryMonitorParams::OC_REMOVE) != 0){
			if (pendingChangesCounter > 0){
				FileItems newFileItems;
				for (int fileIndex = 0; fileIndex < int(m_directoryFiles.size()); fileIndex++){
					const FileItem& fileItem = m_directoryFiles[fileIndex];

					QFileInfo fileInfo(fileItem.filePath);				
					if (!fileInfo.exists()){
						removedFiles.push_back(fileInfo.absoluteFilePath());
						
						SendInfoMessage(0, iqt::GetCString(fileInfo.absoluteFilePath() + " was removed"));
					}
					else{
						newFileItems.push_back(fileItem);
					}
				}

				m_directoryFiles = newFileItems;
			}
		}

		// check previously not accessed files:
		for (FilesSet::iterator fileIter = m_nonAccessedFiles.begin(); fileIter != m_nonAccessedFiles.end(); fileIter++){
			QFile file(*fileIter);
					
			bool hasAccess = file.open(QIODevice::ReadOnly);
			if (hasAccess){
				QFileInfo fileInfo(*fileIter);

				FileItem fileItem;

				fileItem.filePath = fileInfo.absoluteFilePath();
				fileItem.modifiedTime = fileInfo.lastModified();
				fileItem.permissions = fileInfo.permissions();

				m_directoryFiles.push_back(fileItem);

				addedFiles.push_back(*fileIter);

				m_nonAccessedFiles.erase(fileIter);

				break;
			}

			file.close();
		}

		if ((observingChanges & ifpf::IDirectoryMonitorParams::OC_ADD) != 0 && pendingChangesCounter > 0){
			QStringList currentFiles = m_currentDirectory.entryList(acceptPatterns, QDir::Filters(observingItemTypes) | QDir::NoDotAndDotDot);

			FileItems currentFileItems;
			for (int fileIndex = 0; fileIndex < int(currentFiles.size()); fileIndex++){
				FileItem fileItem;

				QString currentFilePath = m_currentDirectory.absolutePath() + QString("/") + currentFiles[fileIndex];

				QFileInfo fileInfo(currentFilePath);

				fileItem.filePath = fileInfo.absoluteFilePath();
				fileItem.modifiedTime = fileInfo.lastModified();
				fileItem.permissions = fileInfo.permissions();

				currentFileItems.push_back(fileItem);
			}


			for (int currentFileIndex = 0; currentFileIndex < int(currentFileItems.size()); currentFileIndex++){
				// abort processing, if the thread must be finished:
				if (m_finishThread){
					return;
				}

				FileItem& currentFileItem = currentFileItems[currentFileIndex];
				FileItems::iterator foundFileIter = qFind(m_directoryFiles.begin(), m_directoryFiles.end(), currentFileItem);
					
				if (foundFileIter == m_directoryFiles.end()){
					QFile file(currentFileItem.filePath);
					bool hasAccess = file.open(QIODevice::ReadOnly);
					if (hasAccess){
						addedFiles.push_back(currentFileItem.filePath);

						m_directoryFiles.push_back(currentFileItem);

						SendInfoMessage(0, iqt::GetCString(currentFileItem.filePath + " was added"));

						file.close();
					}
					else{
						m_nonAccessedFiles.insert(currentFileItem.filePath);
					}
				}
			}
		}

		if (		(observingChanges & ifpf::IDirectoryMonitorParams::OC_MODIFIED) != 0 || 
					(observingChanges & ifpf::IDirectoryMonitorParams::OC_ATTR_CHANGED)){
			for (int fileIndex = 0; fileIndex < int(m_directoryFiles.size()); fileIndex++){
				FileItem& fileItem = m_directoryFiles[fileIndex];
				QFileInfo fileInfo(m_directoryFiles[fileIndex].filePath);
				if (!fileInfo.exists()){
					continue;
				}

				if ((observingChanges & ifpf::IDirectoryMonitorParams::OC_MODIFIED) != 0){
					QDateTime currentModifiedTime = fileInfo.lastModified();
					QDateTime previousModifiedTime = fileItem.modifiedTime;
					if (previousModifiedTime != currentModifiedTime){
						modifiedFiles.push_back(fileInfo.absoluteFilePath());

						fileItem.modifiedTime = currentModifiedTime;
						
						SendInfoMessage(0, iqt::GetCString(fileInfo.absoluteFilePath() + " was modified"));
					}
				}

				if ((observingChanges & ifpf::IDirectoryMonitorParams::OC_ATTR_CHANGED) != 0){
					QFile::Permissions currentPermissions = fileInfo.permissions();
					QFile::Permissions previousPermissions = fileItem.permissions;
					if (currentPermissions != previousPermissions){
						attributeChangedFiles.push_back(fileInfo.absoluteFilePath());

						fileItem.permissions = currentPermissions;
						
						SendInfoMessage(0, istd::CString("Attributes of") + iqt::GetCString(fileInfo.absoluteFilePath() + " have been changed"));
					}
				}
			}
		}

		isys::CSectionBlocker block(&m_lock);

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

		I_IF_DEBUG(
			double processingTime = measurementTimer.GetElapsed();

			SendInfoMessage(0, 
				istd::CString("Folder monitoring of ") + 
				iqt::GetCString(m_currentDirectory.absolutePath()) + 
				istd::CString(": ") + 
				istd::CString().FromNumber(processingTime) + " seconds");
		)
	}
}


// private slots

void CDirectoryMonitorComp::OnFolderChanged(int changeFlags)
{
	if ((changeFlags & CF_SOME_CHANGES) != 0){
		// notify observers:
		istd::CChangeNotifier changePtr(this, changeFlags);

		changePtr.Reset();

		UpdateMonitoringSession();
	}
}


void CDirectoryMonitorComp::OnDirectoryChangeNotification(const QString& /*directory*/)
{
	isys::CSectionBlocker block(&m_lock);

	++m_directoryPendingChangesCounter;
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

	m_directoryWatcher.removePaths(m_directoryWatcher.directories());
	m_directoryWatcher.addPath(folderPath);

	if (m_monitoringSessionManagerCompPtr.IsValid()){
		ifpf::IMonitoringSession* sessionPtr = m_monitoringSessionManagerCompPtr->GetSession(*this, iqt::GetCString(m_currentDirectory.absolutePath()));
		if (sessionPtr != NULL){
			istd::CStringList sessionFiles = sessionPtr->GetFileList();
			for (int fileIndex = 0; fileIndex < int(sessionFiles.size()); fileIndex++){
				FileItem fileItem;

				fileItem.filePath = iqt::GetQString(sessionFiles[fileIndex]);

				m_directoryFiles.push_back(fileItem);
			}
		}
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


bool CDirectoryMonitorComp::ConnectToParameterModel(const iprm::IParamsSet& paramsSet)
{
	DisconnectFromParameterModel();

	m_directoryPathModelPtr = dynamic_cast<const imod::IModel*>(paramsSet.GetParameter((*m_directoryPathIdAttrPtr).ToString()));
	m_directoryMonitorParamsModelPtr = dynamic_cast<const imod::IModel*>(paramsSet.GetParameter((*m_directoryMonitorParamsIdAttrPtr).ToString()));

	I_ASSERT(m_directoryPathModelPtr != NULL);
	I_ASSERT(m_directoryMonitorParamsModelPtr != NULL);

	if (m_directoryPathModelPtr != NULL && m_directoryMonitorParamsModelPtr != NULL){
		bool retVal = (const_cast<imod::IModel*>(m_directoryPathModelPtr))->AttachObserver(&m_directoryParamsObserver);

		retVal = retVal && (const_cast<imod::IModel*>(m_directoryMonitorParamsModelPtr))->AttachObserver(&m_monitoringParamsObserver);

		return retVal;
	}

	return false;
}


void CDirectoryMonitorComp::DisconnectFromParameterModel()
{
	if (m_directoryPathModelPtr != NULL){
		if (m_directoryPathModelPtr->IsAttached(&m_directoryParamsObserver)){
			(const_cast<imod::IModel*>(m_directoryPathModelPtr))->DetachObserver(&m_directoryParamsObserver);
		}
	}

	if (m_directoryMonitorParamsModelPtr != NULL){
		if (m_directoryMonitorParamsModelPtr->IsAttached(&m_monitoringParamsObserver)){
			(const_cast<imod::IModel*>(m_directoryMonitorParamsModelPtr))->DetachObserver(&m_monitoringParamsObserver);
		}
	}
}


void CDirectoryMonitorComp::UpdateMonitoringSession() const
{
	// update current session:
	if (m_monitoringSessionManagerCompPtr.IsValid()){
		ifpf::IMonitoringSession* sessionPtr = m_monitoringSessionManagerCompPtr->GetSession(*this, iqt::GetCString(m_currentDirectory.absolutePath()));
		if (sessionPtr != NULL){
			sessionPtr->SetFileList(GetFileList());
		}
	}
}


// public methods of the embedded class MonitoringParamsObserver

CDirectoryMonitorComp::MonitoringParamsObserver::MonitoringParamsObserver(CDirectoryMonitorComp& parent)
	:m_parent(parent)
{
}


// reimplemented (imod::IObserver)

bool CDirectoryMonitorComp::MonitoringParamsObserver::OnDetached(imod::IModel* modelPtr)
{
	m_parent.m_directoryMonitorParamsModelPtr = NULL;
	
	return BaseClass::OnDetached(modelPtr);
}


void CDirectoryMonitorComp::MonitoringParamsObserver::AfterUpdate(imod::IModel* modelPtr, int updateFlags, istd::IPolymorphic* updateParamsPtr)
{
	I_ASSERT(modelPtr != NULL);
	if (modelPtr != NULL){
		isys::CSectionBlocker block(&m_parent.m_lock);

		const ifpf::IDirectoryMonitorParams* directoryMonitorParamsPtr = dynamic_cast<const ifpf::IDirectoryMonitorParams*>(modelPtr);
		I_ASSERT(directoryMonitorParamsPtr != NULL);
		if (directoryMonitorParamsPtr != NULL){
			m_parent.m_poolingFrequency = directoryMonitorParamsPtr->GetPoolingIntervall();
			m_parent.m_observingItemTypes = directoryMonitorParamsPtr->GetObservedItemTypes();
			m_parent.m_observingChanges = directoryMonitorParamsPtr->GetObservedChanges();
			m_parent.m_fileFilterExpressions = iqt::GetQStringList(directoryMonitorParamsPtr->GetAcceptPatterns());
		}
	}

	BaseClass::AfterUpdate(modelPtr, updateFlags, updateParamsPtr);
}


// public methods of the embedded class DirectoryParamsObserver

CDirectoryMonitorComp::DirectoryParamsObserver::DirectoryParamsObserver(CDirectoryMonitorComp& parent)
	:m_parent(parent)
{
}


// reimplemented (imod::IObserver)

bool CDirectoryMonitorComp::DirectoryParamsObserver::OnDetached(imod::IModel* modelPtr)
{
	m_parent.m_directoryPathModelPtr = NULL;

	return BaseClass::OnDetached(modelPtr);
}


void CDirectoryMonitorComp::DirectoryParamsObserver::AfterUpdate(imod::IModel* modelPtr, int updateFlags, istd::IPolymorphic* updateParamsPtr)
{
	I_ASSERT(modelPtr != NULL);
	if (modelPtr != NULL){
		const iprm::IFileNameParam* directoryPathPtr = dynamic_cast<const iprm::IFileNameParam*>(modelPtr);
		I_ASSERT(directoryPathPtr != NULL);
		if (directoryPathPtr != NULL){
			bool needRestart = m_parent.isRunning();
			if (needRestart){
				m_parent.StopObserverThread();
			}

			QString currentPath = iqt::GetQString(directoryPathPtr->GetPath());
			if (m_parent.m_currentDirectory != QDir(currentPath)){
				m_parent.SetFolderPath(currentPath);
			}

			// if observing thread was running -> restart:
			if (needRestart){
				m_parent.StartObserverThread();
			}
		}
	}

	BaseClass::AfterUpdate(modelPtr, updateFlags, updateParamsPtr);
}


} // namespace iqtfpf


