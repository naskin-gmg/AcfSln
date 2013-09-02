#include "ihotfgui/CDirectoryMonitorComp.h"


// Qt includes
#include <QtCore/QMutexLocker>

// ACF includes
#include "istd/TChangeNotifier.h"
#include "istd/CGeneralTimeStamp.h"
#include "imod/IModel.h"


namespace ihotfgui
{


CDirectoryMonitorComp::CDirectoryMonitorComp()
:	m_finishThread(false),
	m_directoryPendingChangesCounter(1),
	m_poolingFrequency(5.0),
	m_observingItemTypes(ihotf::IDirectoryMonitorParams::OI_ALL),
	m_observingChanges(ihotf::IDirectoryMonitorParams::OC_ALL),
	m_monitoringParamsObserver(*this),
	m_directoryParamsObserver(*this),
	m_lockChanges(false)
{
	connect(&m_directoryWatcher, SIGNAL(directoryChanged(const QString&)), this, SLOT(OnDirectoryChangeNotification(const QString&)));
}


// reimplemented (ihotf::IDirectoryMonitor)

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
	Q_ASSERT(parameterModelPtr != NULL);

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
	Q_ASSERT(m_directoryPathIdAttrPtr.IsValid());
	Q_ASSERT(m_directoryMonitorParamsIdAttrPtr.IsValid());

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
    istd::CGeneralTimeStamp updateTimer;

    istd::CGeneralTimeStamp measurementTimer;

	while (!m_finishThread){
		bool needStateUpdate = updateTimer.GetElapsed() > m_poolingFrequency;
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

						I_IF_DEBUG(SendInfoMessage(0, QObject::tr("File %1 was removed").arg(filePath)));
					}
					else{
						++fileIter;
					}
				}
			}
		}

		// check previously not accessed files:
		for (FilesSet::iterator fileIter = m_nonAccessedFiles.begin(); fileIter != m_nonAccessedFiles.end(); fileIter++){
			const QString& filePath = *fileIter;

			QFile file(filePath);

			bool hasAccess = file.open(QIODevice::ReadOnly);
			if (hasAccess){
				QFileInfo fileInfo(filePath);

				m_directoryFiles[filePath] = fileInfo.lastModified();

				addedFiles.push_back(filePath);

				fileIter = m_nonAccessedFiles.erase(fileIter);
			}
		}

		if ((observingChanges & ihotf::IDirectoryMonitorParams::OC_ADD) != 0 && pendingChangesCounter > 0){
			QStringList currentFiles = m_currentDirectory.entryList(acceptPatterns, QDir::Filters(observingItemTypes) | QDir::NoDotAndDotDot);

			FilesSet currentFileItems;
			for (int fileIndex = 0; fileIndex < int(currentFiles.size()); fileIndex++){
				QString currentFilePath = m_currentDirectory.absoluteFilePath(currentFiles[fileIndex]);

				QFileInfo fileInfo(currentFilePath);

				currentFileItems.insert(fileInfo.canonicalFilePath());
			}

			for (FilesSet::Iterator currentFileIter = currentFileItems.begin(); currentFileIter != currentFileItems.end(); ++currentFileIter){
				// abort processing, if the thread must be finished:
				if (m_finishThread){
					return;
				}

				const QString& currentFilePath = *currentFileIter;

				if (!m_directoryFiles.contains(currentFilePath)){
					QFile file(currentFilePath);
					bool hasAccess = file.open(QIODevice::ReadOnly);
					if (hasAccess){
						addedFiles.push_back(currentFilePath);

						m_directoryFiles[currentFilePath] = QFileInfo(file).lastModified();

						I_IF_DEBUG(SendInfoMessage(0, QObject::tr("File %1 was added").arg(currentFilePath)));
					}
					else{
						m_nonAccessedFiles.insert(currentFilePath);
					}
				}
			}
		}

		if (		((observingChanges & ihotf::IDirectoryMonitorParams::OC_MODIFIED) != 0) ||
					((observingChanges & ihotf::IDirectoryMonitorParams::OC_ATTR_CHANGED) != 0)){
			
			for (		ihotf::IMonitoringSession::FileItems::ConstIterator fileIter = m_directoryFiles.constBegin();
						fileIter != m_directoryFiles.constEnd();
						fileIter++){
				const QString& filePath = fileIter.key();
				QFileInfo fileInfo(filePath);
				if (!fileInfo.exists()){
					continue;
				}

				if ((observingChanges & ihotf::IDirectoryMonitorParams::OC_MODIFIED) != 0){
					QDateTime currentModifiedTime = fileInfo.lastModified();
					QDateTime previousModifiedTime = fileIter.value();
					if (previousModifiedTime != currentModifiedTime){
						QString filePath = fileInfo.canonicalFilePath();
						modifiedFiles.push_back(filePath);

						previousModifiedTime = currentModifiedTime;

						I_IF_DEBUG(SendInfoMessage(0, QObject::tr("File % 1 was modified").arg(filePath)));
					}
				}
/*
				if ((observingChanges & ihotf::IDirectoryMonitorParams::OC_ATTR_CHANGED) != 0){
					QFile::Permissions currentPermissions = fileInfo.permissions();
					QFile::Permissions previousPermissions = (QFile::Permissions)fileItem.GetPermissions();
					if (currentPermissions != previousPermissions){
						attributeChangedFiles.push_back(fileInfo.absoluteFilePath());

						I_IF_DEBUG(SendInfoMessage(0, QString("Attributes of") + fileInfo.absoluteFilePath() + " have been changed"));
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

		int changeFlags = istd::IChangeable::CF_MODEL;

		if (!addedFiles.isEmpty()){
			changeFlags |= ihotf::IFileSystemChangeStorage::CF_NEW;
		}

		if (!removedFiles.isEmpty()){
			changeFlags |= ihotf::IFileSystemChangeStorage::CF_REMOVED;
		}

		if (!modifiedFiles.isEmpty()){
			changeFlags |= ihotf::IFileSystemChangeStorage::CF_MODIFIED;
		}

		if (!attributeChangedFiles.isEmpty()){
			changeFlags |= ihotf::IFileSystemChangeStorage::CF_ATTRIBUTE_CHANGED;
		}

		Q_EMIT FolderChanged(changeFlags);
		updateTimer.Start();

		I_IF_DEBUG(
			double processingTime = measurementTimer.GetElapsed();

			SendInfoMessage(0,
				QString("Folder monitoring of ") +
				m_currentDirectory.absolutePath() +
				QString(": ") +
				QString().setNum(processingTime) + " seconds");
		)
	}
}


// private slots

void CDirectoryMonitorComp::OnFolderChanged(int changeFlags)
{
	QMutexLocker locker(&m_mutex);

	Q_ASSERT(m_fileSystemChangeStorageCompPtr.IsValid());
	if (m_fileSystemChangeStorageCompPtr.IsValid()){
		if ((changeFlags & ihotf::IFileSystemChangeStorage::CF_NEW) != 0){
			istd::CChangeNotifier changePtr(m_fileSystemChangeStorageCompPtr.GetPtr(), ihotf::IFileSystemChangeStorage::CF_NEW);

			for (int fileIndex = 0; fileIndex < m_folderChanges.addedFiles.size(); fileIndex++){
				m_fileSystemChangeStorageCompPtr->UpdateStorageItem(m_folderChanges.addedFiles[fileIndex], ihotf::IFileSystemChangeStorage::CF_NEW);
			}
		}

		if ((changeFlags & ihotf::IFileSystemChangeStorage::CF_REMOVED) != 0){
			istd::CChangeNotifier changePtr(m_fileSystemChangeStorageCompPtr.GetPtr(), ihotf::IFileSystemChangeStorage::CF_REMOVED);

			for (int fileIndex = 0; fileIndex < m_folderChanges.removedFiles.size(); fileIndex++){
				m_fileSystemChangeStorageCompPtr->UpdateStorageItem(m_folderChanges.removedFiles[fileIndex], ihotf::IFileSystemChangeStorage::CF_REMOVED);
			}
		}

		if ((changeFlags & ihotf::IFileSystemChangeStorage::CF_MODIFIED) != 0){
			istd::CChangeNotifier changePtr(m_fileSystemChangeStorageCompPtr.GetPtr(), ihotf::IFileSystemChangeStorage::CF_MODIFIED);

			for (int fileIndex = 0; fileIndex < m_folderChanges.modifiedFiles.size(); fileIndex++){
				m_fileSystemChangeStorageCompPtr->UpdateStorageItem(m_folderChanges.modifiedFiles[fileIndex], ihotf::IFileSystemChangeStorage::CF_MODIFIED);
			}
		}

		if ((changeFlags & ihotf::IFileSystemChangeStorage::CF_ATTRIBUTE_CHANGED) != 0){
			istd::CChangeNotifier changePtr(m_fileSystemChangeStorageCompPtr.GetPtr(), ihotf::IFileSystemChangeStorage::CF_ATTRIBUTE_CHANGED);

			for (int fileIndex = 0; fileIndex < m_folderChanges.attributeChangedFiles.size(); fileIndex++){
				m_fileSystemChangeStorageCompPtr->UpdateStorageItem(m_folderChanges.attributeChangedFiles[fileIndex], ihotf::IFileSystemChangeStorage::CF_MODIFIED);
			}
		}
	}

	if ((changeFlags & ihotf::IFileSystemChangeStorage::CF_SOME_CHANGES) != 0){
		UpdateMonitoringSession();
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
		SendInfoMessage(0, QString("Start observing of: ") + m_currentDirectory.absolutePath(), "DirectoryMonitor");

		BaseClass2::start(QThread::LowPriority);
	}
}


void CDirectoryMonitorComp::StopObserverThread()
{
	m_finishThread = true;

	// wait for 30 seconds for finishing of thread:
    istd::CGeneralTimeStamp timer;
    while ((timer.GetElapsed() < 30) && BaseClass2::isRunning());

	if (BaseClass2::isRunning()){
		BaseClass2::terminate();
	}
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
	else{
		return false;
	}
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


// public methods of the embedded class MonitoringParamsObserver

CDirectoryMonitorComp::MonitoringParamsObserver::MonitoringParamsObserver(CDirectoryMonitorComp& parent)
:	m_parent(parent)
{
}


// reimplemented (imod::IObserver)

void CDirectoryMonitorComp::MonitoringParamsObserver::AfterUpdate(imod::IModel* modelPtr, int updateFlags, istd::IPolymorphic* updateParamsPtr)
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
			m_parent.m_fileFilterExpressions = (directoryMonitorParamsPtr->GetAcceptPatterns());
		}
	}

	BaseClass::AfterUpdate(modelPtr, updateFlags, updateParamsPtr);
}


// public methods of the embedded class DirectoryParamsObserver

CDirectoryMonitorComp::DirectoryParamsObserver::DirectoryParamsObserver(CDirectoryMonitorComp& parent)
:	m_parent(parent)
{
}


// reimplemented (imod::IObserver)

void CDirectoryMonitorComp::DirectoryParamsObserver::AfterUpdate(imod::IModel* modelPtr, int updateFlags, istd::IPolymorphic* updateParamsPtr)
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

	BaseClass::AfterUpdate(modelPtr, updateFlags, updateParamsPtr);
}


} // namespace ihotfgui


