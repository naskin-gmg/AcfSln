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

void CDirectoryMonitorComp::AfterUpdate(imod::IModel* modelPtr, int /*updateFlags*/, istd::IPolymorphic* /*updateParamsPtr*/)
{
	I_ASSERT(modelPtr != NULL);
	if (modelPtr != NULL){
		SynchronizeWithModel(*modelPtr);
	}
}


// reimplemented (icomp::IComponent)

void CDirectoryMonitorComp::OnComponentCreated()
{
	I_ASSERT(m_directoryPathIdAttrPtr.IsValid());
	I_ASSERT(m_directoryMonitorParamsIdAttrPtr.IsValid());
	I_ASSERT(m_paramsSetCompPtr.IsValid())
	I_ASSERT(m_paramsSetModelCompPtr.IsValid())

	BaseClass::OnComponentCreated();

	connect(this, SIGNAL(FolderChanged(int)), this, SLOT(OnFolderChanged(int)), Qt::QueuedConnection);

	m_directoryPathModelPtr = dynamic_cast<const imod::IModel*>(m_paramsSetCompPtr->GetParameter((*m_directoryPathIdAttrPtr).ToString()));
	m_directoryMonitorParamsModelPtr = dynamic_cast<const imod::IModel*>(m_paramsSetCompPtr->GetParameter((*m_directoryMonitorParamsIdAttrPtr).ToString()));
	m_directoryPathPtr = dynamic_cast<const iprm::IFileNameParam*>(m_directoryPathModelPtr);
	m_directoryMonitorParamsPtr = dynamic_cast<const ifpf::IDirectoryMonitorParams*>(m_directoryMonitorParamsModelPtr);

	I_ASSERT(m_directoryPathModelPtr != NULL);
	I_ASSERT(m_directoryMonitorParamsModelPtr != NULL);

	if (m_directoryPathModelPtr != NULL && m_directoryMonitorParamsModelPtr != NULL){
		(const_cast<imod::IModel*>(m_directoryPathModelPtr))->AttachObserver(this);

		(const_cast<imod::IModel*>(m_directoryMonitorParamsModelPtr))->AttachObserver(this);
	}
}


void CDirectoryMonitorComp::OnComponentDestroyed()
{
	StopObserverThread();

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

		// set parameters and do file search:
		m_lock.Enter();

		int observingChanges = m_observingChanges;
		QStringList fileFilters = m_fileFilterExpressions;
		int observingItemTypes = m_observingItemTypes;

		m_lock.Leave();

		QFileInfoList currentFiles = m_currentDirectory.entryInfoList(fileFilters, QDir::Filters(observingItemTypes) | QDir::NoDotAndDotDot);

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

		int changeFlags = CF_MODEL;

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


// private methods

void CDirectoryMonitorComp::SetFolderPath(const QString& folderPath)
{	
	if (m_currentDirectory == QDir(folderPath)){
		return;
	}

	if (folderPath.isEmpty()){
		return;
	}

	StopObserverThread();

	ResetFiles();

	if (m_monitoringSessionManagerCompPtr.IsValid()){
		ifpf::IMonitoringSession* sessionPtr = m_monitoringSessionManagerCompPtr->GetSession(*this, iqt::GetCString(folderPath));
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
		m_fileFilterExpressions = iqt::GetQStringList(m_directoryMonitorParamsPtr->GetFileFilters());
	}

	QFileInfo fileInfo(folderPath);
	if (fileInfo.exists()){
		m_currentDirectory = QDir(folderPath);

		StartObserverThread();
	}
	else{
		SendWarningMessage(0, istd::CString("Directory: ") + iqt::GetCString(folderPath) + istd::CString(" not exists. Observing aborted"), "DirectoryMonitor");
	}
}


void CDirectoryMonitorComp::StartObserverThread()
{
	m_finishThread = false;

	QFileInfo fileInfo(m_currentDirectory.absolutePath());
	if (fileInfo.exists()){
		SendInfoMessage(0, istd::CString("Start observing of: ") + iqt::GetCString(m_currentDirectory.absolutePath()), "DirectoryMonitor");

		BaseClass2::start();
	}
}


void CDirectoryMonitorComp::StopObserverThread()
{
	m_finishThread = true;

	// wait for 30 seconds for finishing of thread: 
	iqt::CTimer timer;
	while (timer.GetElapsed() < 30 && BaseClass2::isRunning()){
		QApplication::processEvents(QEventLoop::ExcludeUserInputEvents);
	}

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
		m_fileFilterExpressions = iqt::GetQStringList(m_directoryMonitorParamsPtr->GetFileFilters());
	}
}


} // namespace iqtfpf


