#include "iqtfpf/CDirectoryMonitorComp.h"


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
	m_directoryPendingChangesCounter(1),
	m_lockChanges(false)
{
	connect(&m_directoryWatcher, SIGNAL(directoryChanged(const QString&)), this, SLOT(OnDirectoryChangeNotification(const QString&)));
}


// reimplemented (ifpf::IDirectoryMonitor)

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
	const iprm::IParamsSet* parameterModelPtr = (paramsSetPtr == NULL) ? m_paramsSetCompPtr.GetPtr() : paramsSetPtr;
	I_ASSERT(parameterModelPtr != NULL);

	// connect to own parameter model:
	if (ConnectToParameterModel(*parameterModelPtr)){
		StartObserverThread();

		return true;
	}

	SendErrorMessage(0, "Directory monitoring parameters could not be set");

	return false;
}


void CDirectoryMonitorComp::StopObserving()
{
	StopObserverThread();
}


// protected methods

// reimplemented (icomp::CComponentBase)

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


// reimplemented (QThread)

void CDirectoryMonitorComp::run()
{
	iqt::CTimer updateTimer;

	iqt::CTimer measurementTimer;
	
	while (!m_finishThread){
		bool needStateUpdate = (updateTimer.GetElapsed() >= m_poolingFrequency);
		if (!needStateUpdate || m_lockChanges){
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
		m_lockChanges = true;

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
					const isys::CFileInfo& fileItem = m_directoryFiles[fileIndex];

					QFileInfo fileInfo(iqt::GetQFileInfo(fileItem));				
					if (!fileInfo.exists()){
						removedFiles.push_back(fileInfo.absoluteFilePath());
						
						I_IF_DEBUG(SendInfoMessage(0, iqt::GetCString(fileInfo.absoluteFilePath() + " was removed"));)
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

				m_directoryFiles.push_back(iqt::GetCFileInfo(fileInfo));

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
				QString currentFilePath = m_currentDirectory.absolutePath() + QString("/") + currentFiles[fileIndex];

				QFileInfo fileInfo(currentFilePath);

				currentFileItems.push_back(iqt::GetCFileInfo(fileInfo));
			}


			for (int currentFileIndex = 0; currentFileIndex < int(currentFileItems.size()); currentFileIndex++){
				// abort processing, if the thread must be finished:
				if (m_finishThread){
					return;
				}

				isys::CFileInfo& currentFileItem = currentFileItems[currentFileIndex];
				FileItems::iterator foundFileIter = qFind(m_directoryFiles.begin(), m_directoryFiles.end(), currentFileItem);
					
				if (foundFileIter == m_directoryFiles.end()){
					QFile file(iqt::GetQString(currentFileItem.GetFilePath()));
					bool hasAccess = file.open(QIODevice::ReadOnly);
					if (hasAccess){
						addedFiles.push_back(iqt::GetQString(currentFileItem.GetFilePath()));

						m_directoryFiles.push_back(currentFileItem);

						I_IF_DEBUG(SendInfoMessage(0, currentFileItem.GetFilePath() + " was added"));

						file.close();
					}
					else{
						m_nonAccessedFiles.insert(iqt::GetQString(currentFileItem.GetFilePath()));
					}
				}
			}
		}

		if (		(observingChanges & ifpf::IDirectoryMonitorParams::OC_MODIFIED) != 0 || 
					(observingChanges & ifpf::IDirectoryMonitorParams::OC_ATTR_CHANGED)){
			for (int fileIndex = 0; fileIndex < int(m_directoryFiles.size()); fileIndex++){
				isys::CFileInfo& fileItem = m_directoryFiles[fileIndex];
				QFileInfo fileInfo(iqt::GetQFileInfo(m_directoryFiles[fileIndex]));
				if (!fileInfo.exists()){
					continue;
				}

				if ((observingChanges & ifpf::IDirectoryMonitorParams::OC_MODIFIED) != 0){
					isys::CSimpleDateTime currentModifiedTime = iqt::GetCSimpleDateTime(fileInfo.lastModified());
					isys::CSimpleDateTime previousModifiedTime = fileItem.GetModificationTime();
					if (previousModifiedTime != currentModifiedTime){
						modifiedFiles.push_back(fileInfo.absoluteFilePath());

						fileItem.SetModificationTime(currentModifiedTime);
						
						I_IF_DEBUG(SendInfoMessage(0, iqt::GetCString(fileInfo.absoluteFilePath() + " was modified")));
					}
				}

				if ((observingChanges & ifpf::IDirectoryMonitorParams::OC_ATTR_CHANGED) != 0){
					QFile::Permissions currentPermissions = fileInfo.permissions();
					QFile::Permissions previousPermissions = fileItem.GetPermissions();
					if (currentPermissions != previousPermissions){
						attributeChangedFiles.push_back(fileInfo.absoluteFilePath());

						fileItem.SetPermissions(currentPermissions);
						
						I_IF_DEBUG(SendInfoMessage(0, istd::CString("Attributes of") + iqt::GetCString(fileInfo.absoluteFilePath() + " have been changed")));
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
			changeFlags |= ifpf::IFileSystemChangeStorage::CF_NEW;
		}

		if (!removedFiles.isEmpty()){
			changeFlags |= ifpf::IFileSystemChangeStorage::CF_REMOVED;
		}

		if (!modifiedFiles.isEmpty()){
			changeFlags |= ifpf::IFileSystemChangeStorage::CF_MODIFIED;
		}

		if (!attributeChangedFiles.isEmpty()){
			changeFlags |= ifpf::IFileSystemChangeStorage::CF_ATTRIBUTE_CHANGED;
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
	isys::CSectionBlocker block(&m_lock);

	I_ASSERT(m_fileSystemChangeStorageCompPtr.IsValid());
	if (m_fileSystemChangeStorageCompPtr.IsValid()){
		if ((changeFlags & ifpf::IFileSystemChangeStorage::CF_NEW) != 0){
			istd::CChangeNotifier changePtr(m_fileSystemChangeStorageCompPtr.GetPtr(), ifpf::IFileSystemChangeStorage::CF_NEW);

			for (int fileIndex = 0; fileIndex < m_folderChanges.addedFiles.size(); fileIndex++){
				m_fileSystemChangeStorageCompPtr->UpdateStorageItem(iqt::GetCString(m_folderChanges.addedFiles[fileIndex]), ifpf::IFileSystemChangeStorage::CF_NEW);
			}
		}

		if ((changeFlags & ifpf::IFileSystemChangeStorage::CF_REMOVED) != 0){
			istd::CChangeNotifier changePtr(m_fileSystemChangeStorageCompPtr.GetPtr(), ifpf::IFileSystemChangeStorage::CF_REMOVED);

			for (int fileIndex = 0; fileIndex < m_folderChanges.removedFiles.size(); fileIndex++){
				m_fileSystemChangeStorageCompPtr->UpdateStorageItem(iqt::GetCString(m_folderChanges.removedFiles[fileIndex]), ifpf::IFileSystemChangeStorage::CF_REMOVED);
			}
		}

		if ((changeFlags & ifpf::IFileSystemChangeStorage::CF_MODIFIED) != 0){
			istd::CChangeNotifier changePtr(m_fileSystemChangeStorageCompPtr.GetPtr(), ifpf::IFileSystemChangeStorage::CF_MODIFIED);

			for (int fileIndex = 0; fileIndex < m_folderChanges.modifiedFiles.size(); fileIndex++){
				m_fileSystemChangeStorageCompPtr->UpdateStorageItem(iqt::GetCString(m_folderChanges.modifiedFiles[fileIndex]), ifpf::IFileSystemChangeStorage::CF_MODIFIED);
			}
		}

		if ((changeFlags & ifpf::IFileSystemChangeStorage::CF_ATTRIBUTE_CHANGED) != 0){
			istd::CChangeNotifier changePtr(m_fileSystemChangeStorageCompPtr.GetPtr(), ifpf::IFileSystemChangeStorage::CF_ATTRIBUTE_CHANGED);

			for (int fileIndex = 0; fileIndex < m_folderChanges.attributeChangedFiles.size(); fileIndex++){
				m_fileSystemChangeStorageCompPtr->UpdateStorageItem(iqt::GetCString(m_folderChanges.attributeChangedFiles[fileIndex]), ifpf::IFileSystemChangeStorage::CF_MODIFIED);
			}
		}
	}

	if ((changeFlags & ifpf::IFileSystemChangeStorage::CF_SOME_CHANGES) != 0){
		UpdateMonitoringSession();
	}

	m_lockChanges = false;
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
		ifpf::IMonitoringSession* sessionPtr = m_monitoringSessionManagerCompPtr->GetSession(iqt::GetCString(m_currentDirectory.absolutePath()));
		if (sessionPtr != NULL){
			m_directoryFiles = sessionPtr->GetFileInfoList();
		}
	}
}


void CDirectoryMonitorComp::StartObserverThread()
{
	m_finishThread = false;

	if (!m_fileSystemChangeStorageCompPtr.IsValid()){
		SendWarningMessage(0, "Directory observation can not be started, because the file storage component coud not be initialized");

		return;
	}

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
	isys::CSectionBlocker block(&m_lock);

	m_directoryFiles.clear();

	m_folderChanges.addedFiles.clear();
	m_folderChanges.attributeChangedFiles.clear();
	m_folderChanges.modifiedFiles.clear();
	m_folderChanges.removedFiles.clear();

	if (m_fileSystemChangeStorageCompPtr.IsValid()){
		m_fileSystemChangeStorageCompPtr->ResetStorage();
	}
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
		ifpf::IMonitoringSession* sessionPtr = m_monitoringSessionManagerCompPtr->GetSession(iqt::GetCString(m_currentDirectory.absolutePath()));
		if (sessionPtr != NULL){
			sessionPtr->SetFileInfoList(m_directoryFiles);
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


