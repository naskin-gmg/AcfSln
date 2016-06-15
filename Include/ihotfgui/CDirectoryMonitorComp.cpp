#include "ihotfgui/CDirectoryMonitorComp.h"


// Qt includes
#include <QtCore/QMutexLocker>
#include <QtCore/QMetaType>

// ACF includes
#include "istd/CChangeNotifier.h"
#include "istd/CGeneralTimeStamp.h"
#include "ifile/CFileListProviderComp.h"


namespace ihotfgui
{


CDirectoryMonitorComp::CDirectoryMonitorComp()
:	m_finishThread(false),
	m_directoryPendingChangesCounter(1),
	m_poolingFrequency(5.0),
	m_observingItemTypes(ihotf::IDirectoryMonitorParams::OI_ALL),
	m_observingChanges(ihotf::IDirectoryMonitorParams::OC_ALL),
	m_lastModificationMinDifference(30),
	m_folderDepth(0),
	m_timestampMode(ihotf::IDirectoryMonitorParams::FTM_MODIFIED),
	m_monitoringParamsObserver(*this),
	m_directoryParamsObserver(*this),
	m_lockChanges(false)
{
	qRegisterMetaType<istd::IChangeable::ChangeSet>("istd::IChangeable::ChangeSet");

	connect(&m_directoryWatcher, SIGNAL(directoryChanged(const QString&)), this, SLOT(OnDirectoryChangeNotification(const QString&)));
}


// reimplemented (ihotf::IDirectoryMonitor)

bool CDirectoryMonitorComp::IsRunning() const
{
	return BaseClass2::isRunning();
}


bool CDirectoryMonitorComp::StartObserving(const iprm::IParamsSet* paramsSetPtr)
{
	bool wasRunning = BaseClass2::isRunning();
	if (wasRunning){
		StopObserverThread();
	}

	if ((paramsSetPtr == NULL) && (!m_paramsSetCompPtr.IsValid() || !m_paramsSetModelCompPtr.IsValid())){
		SendVerboseMessage("Directory monitoring parameters are invalid or not set");

		return false;
	}

	// if external parameter model is used, connect to it, otherwise use the default parameter set:
	const iprm::IParamsSet* parameterModelPtr = (paramsSetPtr == NULL) ? m_paramsSetCompPtr.GetPtr() : paramsSetPtr;
	Q_ASSERT(parameterModelPtr != NULL);

	// connect to own parameter model:
	if (ConnectToParameterModel(*parameterModelPtr)){
		StartObserverThread();

		return true;
	}

	SendErrorMessage(0, "Directory monitoring parameters could not be set", "Folder monitor");

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
	Q_ASSERT(m_directoryPathIdAttrPtr.IsValid());
	Q_ASSERT(m_directoryMonitorParamsIdAttrPtr.IsValid());

	BaseClass::OnComponentCreated();

	connect(this, SIGNAL(FolderChanged(istd::IChangeable::ChangeSet)), this, SLOT(OnFolderChanged(istd::IChangeable::ChangeSet)), Qt::QueuedConnection);

	if (m_autoStartAttrPtr.IsValid() && *m_autoStartAttrPtr){
		StartObserving();
	}
}


void CDirectoryMonitorComp::OnComponentDestroyed()
{
	StopObserving();

	DisconnectFromParameterModel();

	UpdateMonitoringSession();

	Q_ASSERT(m_finishThread);

	BaseClass::OnComponentDestroyed();
}


// reimplemented (QThread)

void CDirectoryMonitorComp::run()
{
	istd::CGeneralTimeStamp updateTimer;

	m_lockChanges = false;

	while (!m_finishThread){
		bool needStateUpdate = updateTimer.GetElapsed() > m_poolingFrequency;
		if (!needStateUpdate || m_lockChanges){
			msleep(100);

			continue;
		}

		QFileInfo currentDirectoryInfo(m_currentDirectory.absolutePath());
		if (!currentDirectoryInfo.exists()){
			msleep(100);

			continue;
		}

		QMutexLocker locker(&m_mutex);

		// set parameters and do file search:
		int observingChanges = m_observingChanges;
		QStringList acceptPatterns = m_fileFilterExpressions;
		int observingItemTypes = m_observingItemTypes;
		int pendingChangesCounter = m_directoryPendingChangesCounter;
		m_directoryPendingChangesCounter = 0;
		m_lockChanges = true;

		locker.unlock();

		QStringList addedFiles;
		QStringList removedFiles;
		QStringList modifiedFiles;
		QStringList attributeChangedFiles;

		// check for changes:
		if ((observingChanges & ihotf::IDirectoryMonitorParams::OC_REMOVE) != 0){
			if (pendingChangesCounter > 0){
				for (		ihotf::IMonitoringSession::FileItems::Iterator fileIter = m_directoryFiles.begin();
							fileIter != m_directoryFiles.end();){
					QString filePath = fileIter.key();
					QFileInfo fileInfo(filePath);
					if (!fileInfo.exists()){
						removedFiles.push_back(filePath);
						fileIter = m_directoryFiles.erase(fileIter);

						I_IF_DEBUG(SendVerboseMessage(QObject::tr("File %1 was removed").arg(filePath)));
					}
					else{
						++fileIter;
					}
				}
			}
		}

		// Check access for previously added files:
		UpdateNonAccessedFiles(m_nonAccessedAddedFiles, addedFiles);

		// Check access for previously modified files:
		UpdateNonAccessedFiles(m_nonAccessedModifiedFiles, modifiedFiles);

		if ((((observingChanges & ihotf::IDirectoryMonitorParams::OC_ADD) != 0) && (m_folderDepth != 0)) || (pendingChangesCounter > 0)){
			QFileInfoList currentFileInfos;
			if (observingItemTypes & QDir::Files){
				ifile::CFileListProviderComp::CreateFileList(
							m_currentDirectory,
							0,
							m_folderDepth,
							acceptPatterns,
							QDir::Name,
							currentFileInfos,
							this);
			}

			if ((observingItemTypes & QDir::Dirs) || (observingItemTypes & QDir::Drives)){
				QFileInfoList directoriesList;
				ifile::CFileListProviderComp::CreateDirectoryList(
							m_currentDirectory,
							0,
							m_folderDepth,
							acceptPatterns,
							QDir::Name,
							currentFileInfos,
							this);

				currentFileInfos += directoriesList;
			}

			FileAccessMap currentFileItems;
			for (int fileIndex = 0; fileIndex < currentFileInfos.count(); fileIndex++){
				FileAccessInfo accessInfo;

				accessInfo.fileSize = currentFileInfos[fileIndex].size();
				accessInfo.checkTimeStamp = QDateTime::currentDateTime();
				accessInfo.lastAccessTimeStamp = GetLastAccessTime(currentFileInfos[fileIndex]);
				currentFileItems[currentFileInfos[fileIndex].canonicalFilePath()] = accessInfo;
			}

			for (FileAccessMap::Iterator currentFileIter = currentFileItems.begin(); currentFileIter != currentFileItems.end(); ++currentFileIter){
				// abort processing, if the thread must be finished:
				if (m_finishThread){
					return;
				}

				const QString& currentFilePath = currentFileIter.key();

				if (!m_directoryFiles.contains(currentFilePath) && !m_nonAccessedAddedFiles.contains(currentFilePath)){
					m_nonAccessedAddedFiles[currentFilePath] = currentFileIter.value();
				}
			}
		}

		if (		((observingChanges & ihotf::IDirectoryMonitorParams::OC_MODIFIED) != 0) ||
					((observingChanges & ihotf::IDirectoryMonitorParams::OC_ATTR_CHANGED) != 0)){
			
			for (		ihotf::IMonitoringSession::FileItems::Iterator fileIter = m_directoryFiles.begin();
						fileIter != m_directoryFiles.end();
						fileIter++){
				const QString& filePath = fileIter.key();
				QFileInfo fileInfo(filePath);
				if (!fileInfo.exists()){
					continue;
				}

				if ((observingChanges & ihotf::IDirectoryMonitorParams::OC_MODIFIED) != 0){
					QDateTime currentModifiedTime = GetLastAccessTime(fileInfo);
					QDateTime& previousModifiedTime = fileIter.value();
					if (previousModifiedTime != currentModifiedTime){
						// Check if the file was just added now:
						if (!addedFiles.contains(filePath) && !m_nonAccessedModifiedFiles.contains(filePath)){
							FileAccessInfo accessInfo;

							QFileInfo fileInfo(filePath);
							accessInfo.fileSize = fileInfo.size();
							accessInfo.checkTimeStamp = QDateTime::currentDateTime();
							accessInfo.lastAccessTimeStamp = GetLastAccessTime(fileInfo);

							m_nonAccessedModifiedFiles[filePath] = accessInfo;
						}
						// File was just added but already changed its modification time, also update modification time:
						else{
							m_directoryFiles[filePath] = currentModifiedTime;
						}
					}
				}
/*
				if ((observingChanges & ihotf::IDirectoryMonitorParams::OC_ATTR_CHANGED) != 0){
					QFile::Permissions currentPermissions = fileInfo.permissions();
					QFile::Permissions previousPermissions = (QFile::Permissions)fileItem.GetPermissions();
					if (currentPermissions != previousPermissions){
						attributeChangedFiles.push_back(fileInfo.absoluteFilePath());

						I_IF_DEBUG(SendVerboseMessage(QString("Attributes of") + fileInfo.absoluteFilePath() + " have been changed"));
					}
				}
*/
			}
		}

		locker.relock();

		m_folderChanges.addedFiles = addedFiles;
		m_folderChanges.removedFiles = removedFiles;
		m_folderChanges.modifiedFiles = modifiedFiles;
		m_folderChanges.attributeChangedFiles = attributeChangedFiles;

		istd::IChangeable::ChangeSet changeSet;

		if (!addedFiles.isEmpty()){
			changeSet += ihotf::IFileSystemChangeStorage::CF_NEW;
		}

		if (!removedFiles.isEmpty()){
			changeSet += ihotf::IFileSystemChangeStorage::CF_REMOVED;
		}

		if (!modifiedFiles.isEmpty()){
			changeSet += ihotf::IFileSystemChangeStorage::CF_MODIFIED;
		}

		if (!attributeChangedFiles.isEmpty()){
			changeSet += ihotf::IFileSystemChangeStorage::CF_ATTRIBUTE_CHANGED;
		}

		Q_EMIT FolderChanged(changeSet);
		updateTimer.Start();
	}
}


// private slots

void CDirectoryMonitorComp::OnFolderChanged(const istd::IChangeable::ChangeSet& changeSet)
{
	QMutexLocker locker(&m_mutex);

	istd::IChangeable::ChangeSet possibleChanges(
				ihotf::IFileSystemChangeStorage::CF_NEW,
				ihotf::IFileSystemChangeStorage::CF_REMOVED,
				ihotf::IFileSystemChangeStorage::CF_MODIFIED,
				ihotf::IFileSystemChangeStorage::CF_ATTRIBUTE_CHANGED);

	if (changeSet.ContainsAny(possibleChanges)){
		UpdateMonitoringSession();
	}

	Q_ASSERT(m_fileSystemChangeStorageCompPtr.IsValid());
	if (m_fileSystemChangeStorageCompPtr.IsValid()){
		if (changeSet.Contains(ihotf::IFileSystemChangeStorage::CF_NEW)){
			static const istd::IChangeable::ChangeSet changeSet(ihotf::IFileSystemChangeStorage::CF_NEW);
			istd::CChangeNotifier notifier(m_fileSystemChangeStorageCompPtr.GetPtr(), &changeSet);
			Q_UNUSED(notifier);

			for (int fileIndex = 0; fileIndex < m_folderChanges.addedFiles.size(); fileIndex++){
				m_fileSystemChangeStorageCompPtr->UpdateStorageItem(m_folderChanges.addedFiles[fileIndex], ihotf::IFileSystemChangeStorage::CF_NEW);
			}
		}

		if (changeSet.Contains(ihotf::IFileSystemChangeStorage::CF_REMOVED)){
			static const istd::IChangeable::ChangeSet changeSet(ihotf::IFileSystemChangeStorage::CF_REMOVED);
			istd::CChangeNotifier storageNotifier(m_fileSystemChangeStorageCompPtr.GetPtr(), &changeSet);
			Q_UNUSED(storageNotifier);

			for (int fileIndex = 0; fileIndex < m_folderChanges.removedFiles.size(); fileIndex++){
				m_fileSystemChangeStorageCompPtr->UpdateStorageItem(m_folderChanges.removedFiles[fileIndex], ihotf::IFileSystemChangeStorage::CF_REMOVED);
			}
		}

		if (changeSet.Contains(ihotf::IFileSystemChangeStorage::CF_MODIFIED)){
			static const istd::IChangeable::ChangeSet changeSet(ihotf::IFileSystemChangeStorage::CF_MODIFIED);
			istd::CChangeNotifier storageNotifier(m_fileSystemChangeStorageCompPtr.GetPtr(), &changeSet);
			Q_UNUSED(storageNotifier);

			for (int fileIndex = 0; fileIndex < m_folderChanges.modifiedFiles.size(); fileIndex++){
				m_fileSystemChangeStorageCompPtr->UpdateStorageItem(m_folderChanges.modifiedFiles[fileIndex], ihotf::IFileSystemChangeStorage::CF_MODIFIED);
			}
		}

		if (changeSet.Contains(ihotf::IFileSystemChangeStorage::CF_ATTRIBUTE_CHANGED)){
			static const istd::IChangeable::ChangeSet changeSet(ihotf::IFileSystemChangeStorage::CF_ATTRIBUTE_CHANGED);
			istd::CChangeNotifier storageNotifier(m_fileSystemChangeStorageCompPtr.GetPtr(), &changeSet);
			Q_UNUSED(storageNotifier);

			for (int fileIndex = 0; fileIndex < m_folderChanges.attributeChangedFiles.size(); fileIndex++){
				m_fileSystemChangeStorageCompPtr->UpdateStorageItem(m_folderChanges.attributeChangedFiles[fileIndex], ihotf::IFileSystemChangeStorage::CF_MODIFIED);
			}
		}
	}

	m_lockChanges = false;
}


void CDirectoryMonitorComp::OnDirectoryChangeNotification(const QString& /*directory*/)
{
	QMutexLocker locker(&m_mutex);

	++m_directoryPendingChangesCounter;
}



// private methods

void CDirectoryMonitorComp::SetFolderPath(const QString& folderPath)
{
	Q_ASSERT(!BaseClass2::isRunning());

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
		ihotf::IMonitoringSession* sessionPtr = m_monitoringSessionManagerCompPtr->GetSession(m_currentDirectory.absolutePath());
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
		SendVerboseMessage(QString("Start observing of: ") + m_currentDirectory.absolutePath(), "DirectoryMonitor");

		BaseClass2::start(QThread::LowPriority);
	}
	else{
		SendWarningMessage(0, QString("Start observing of '%1' failed. Folder doesn't exist").arg(m_currentDirectory.absolutePath()), "DirectoryMonitor");
	}
}


void CDirectoryMonitorComp::StopObserverThread()
{
	m_finishThread = true;

	SendVerboseMessage(QString("Stop observing of: ") + m_currentDirectory.absolutePath(), "DirectoryMonitor");

	BaseClass2::wait();
}


void CDirectoryMonitorComp::ResetFiles()
{
	QMutexLocker locker(&m_mutex);

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

	imod::IModel* pathModelPtr = const_cast<imod::IModel*>(dynamic_cast<const imod::IModel*>(paramsSet.GetParameter(*m_directoryPathIdAttrPtr)));
	imod::IModel* monitorParamsModelPtr = const_cast<imod::IModel*>(dynamic_cast<const imod::IModel*>(paramsSet.GetParameter(*m_directoryMonitorParamsIdAttrPtr)));

	if ((pathModelPtr != NULL) && (monitorParamsModelPtr != NULL)){
		return		pathModelPtr->AttachObserver(&m_directoryParamsObserver) &&
					monitorParamsModelPtr->AttachObserver(&m_monitoringParamsObserver);
	}

	return false;
}


void CDirectoryMonitorComp::DisconnectFromParameterModel()
{
	m_directoryParamsObserver.EnsureModelDetached();
	m_monitoringParamsObserver.EnsureModelDetached();
}


void CDirectoryMonitorComp::UpdateMonitoringSession() const
{
	// update current session:
	if (m_monitoringSessionManagerCompPtr.IsValid()){
		ihotf::IMonitoringSession* sessionPtr = m_monitoringSessionManagerCompPtr->GetSession(m_currentDirectory.absolutePath());
		if (sessionPtr != NULL){
			sessionPtr->SetFileInfoList(m_directoryFiles);
		}
	}
}


bool CDirectoryMonitorComp::HasFileAccess(const QString& filePath, FileAccessInfo& lastFileAccessInfo) const
{
	QFileInfo fileInfo(filePath);

	QDateTime currentDateTime = QDateTime::currentDateTime();
	QDateTime lastModifiedAt = GetLastAccessTime(fileInfo);
	
	int checkTimeDiff = lastFileAccessInfo.checkTimeStamp.secsTo(currentDateTime);
	if (checkTimeDiff > m_lastModificationMinDifference){
		int modificationTimeDiff = lastModifiedAt.secsTo(currentDateTime);
		if ((modificationTimeDiff > m_lastModificationMinDifference) || (modificationTimeDiff < 0)){
			qint64 currentFileSize = fileInfo.size();
			if ((lastModifiedAt == lastFileAccessInfo.lastAccessTimeStamp) && (currentFileSize == lastFileAccessInfo.fileSize)){
				return CheckFileAccess(filePath);
			}
			else{
				lastFileAccessInfo.lastAccessTimeStamp = lastModifiedAt;
				lastFileAccessInfo.fileSize = currentFileSize;
				lastFileAccessInfo.checkTimeStamp = QDateTime::currentDateTime();

				return false;
			}
		}
	}

	return false;
}



bool CDirectoryMonitorComp::CheckFileAccess(const QString& filePath) const
{
	QFile file(filePath);

	// Try to access the file:
	bool retVal = file.open(QIODevice::ReadOnly);

	// Close file and restore original permissions:
	file.close();

	return retVal;
}


QDateTime CDirectoryMonitorComp::GetLastAccessTime(const QFileInfo& fileInfo) const
{
	QDateTime lastAccessTime = QDateTime::fromMSecsSinceEpoch(0);

	if (m_timestampMode & ihotf::IDirectoryMonitorParams::FTM_MODIFIED){
		lastAccessTime = fileInfo.lastModified();
	}

	if (m_timestampMode & ihotf::IDirectoryMonitorParams::FTM_CREATED){
		lastAccessTime = qMax(fileInfo.created(), lastAccessTime);
	}

	return lastAccessTime;
}


void CDirectoryMonitorComp::UpdateNonAccessedFiles(FileAccessMap& accessMap, QStringList& fileList)
{
	QMutableMapIterator<QString, FileAccessInfo> nonAccessedFilesIter(accessMap);
	while (nonAccessedFilesIter.hasNext()){
		FileAccessMap::Iterator fileAccessIter = nonAccessedFilesIter.next();

		QString filePath = fileAccessIter.key();

		if (HasFileAccess(filePath, fileAccessIter.value())){
			QFileInfo fileInfo(filePath);

			m_directoryFiles[filePath] = GetLastAccessTime(fileInfo);

			fileList.push_back(filePath);

			nonAccessedFilesIter.remove();
		}
	}
}


// public methods of the embedded class MonitoringParamsObserver

CDirectoryMonitorComp::MonitoringParamsObserver::MonitoringParamsObserver(CDirectoryMonitorComp& parent)
:	m_parent(parent)
{
}


// reimplemented (imod::IObserver)

void CDirectoryMonitorComp::MonitoringParamsObserver::AfterUpdate(imod::IModel* modelPtr, const istd::IChangeable::ChangeSet& changeSet)
{
	Q_ASSERT(modelPtr != NULL);
	if (modelPtr != NULL){
		QMutexLocker locker(&m_parent.m_mutex);

		const ihotf::IDirectoryMonitorParams* directoryMonitorParamsPtr = dynamic_cast<const ihotf::IDirectoryMonitorParams*>(modelPtr);
		Q_ASSERT(directoryMonitorParamsPtr != NULL);
		if (directoryMonitorParamsPtr != NULL){
			m_parent.m_poolingFrequency = directoryMonitorParamsPtr->GetPoolingIntervall();
			m_parent.m_observingItemTypes = directoryMonitorParamsPtr->GetObservedItemTypes();
			m_parent.m_observingChanges = directoryMonitorParamsPtr->GetObservedChanges();
			m_parent.m_fileFilterExpressions = directoryMonitorParamsPtr->GetAcceptPatterns();
			m_parent.m_folderDepth = directoryMonitorParamsPtr->GetFolderDepth();
			m_parent.m_timestampMode = directoryMonitorParamsPtr->GetFileTimeStampMode();
			m_parent.m_lastModificationMinDifference = directoryMonitorParamsPtr->GetMinLastModificationTimeDifference();
		}
	}

	BaseClass::AfterUpdate(modelPtr, changeSet);
}


// public methods of the embedded class DirectoryParamsObserver

CDirectoryMonitorComp::DirectoryParamsObserver::DirectoryParamsObserver(CDirectoryMonitorComp& parent)
:	m_parent(parent)
{
}


// reimplemented (imod::IObserver)

void CDirectoryMonitorComp::DirectoryParamsObserver::AfterUpdate(imod::IModel* modelPtr, const istd::IChangeable::ChangeSet& changeSet)
{
	Q_ASSERT(modelPtr != NULL);
	if (modelPtr != NULL){
		const ifile::IFileNameParam* directoryPathPtr = dynamic_cast<const ifile::IFileNameParam*>(modelPtr);
		Q_ASSERT(directoryPathPtr != NULL);
		if (directoryPathPtr != NULL){
			bool needRestart = m_parent.isRunning();
			if (needRestart){
				m_parent.StopObserverThread();
			}

			QString currentPath = directoryPathPtr->GetPath();
			if (m_parent.m_currentDirectory != QDir(currentPath)){
				m_parent.SetFolderPath(currentPath);
			}

			// if observing thread was running -> restart:
			if (needRestart){
				m_parent.StartObserverThread();
			}
		}
	}

	BaseClass::AfterUpdate(modelPtr, changeSet);
}


} // namespace ihotfgui


