#include "iqtfpf/CFolderMonitorComp.h"


// Qt includes
#include <QDir>
#include <QApplication>


// ACF includes
#include "istd/TChangeNotifier.h"

#include "imod/IModel.h"

#include "iser/IArchive.h"
#include "iser/CArchivetag.h"

#include "isys/CSectionBlocker.h"

#include "iqt/CTimer.h"


namespace iqtfpf
{


CFolderMonitorComp::CFolderMonitorComp()
	:m_finishThread(false),
	m_poolingFrequency(5.0),
	m_observingItemTypes(OI_ALL),
	m_observingChanges(OC_ALL)
{
}


// reimplemented (ifpf::IDirectoryMonitor)

istd::CStringList CFolderMonitorComp::GetChangedFileItems(int changeFlags) const
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

istd::CStringList CFolderMonitorComp::GetFileList() const
{
	istd::CStringList fileList;

	for (int fileIndex = 0; fileIndex < m_directoryFiles.count(); fileIndex++){
		fileList.push_back(iqt::GetCString(m_directoryFiles[fileIndex].absoluteFilePath()));
	}

	return fileList;
}


// reimplemented (istd::IChangeable)

void CFolderMonitorComp::OnEndChanges(int changeFlags, istd::IPolymorphic* changeParamsPtr)
{
	QString currentPath = iqt::GetQString(GetPath());

	if (m_currentDirectory != QDir(currentPath)){
		SetFolderPath(currentPath);
	}

	isys::CSectionBlocker block(&m_lock);

	m_poolingFrequency = GetPoolingIntervall();
	m_observingItemTypes = GetObservedItemTypes();
	m_observingChanges = GetObservedChanges();
	m_fileFilterExpressions = iqt::GetQStringList(GetFileFilters());
	
	BaseClass3::OnEndChanges(changeFlags, changeParamsPtr);
}


// reimplemented (icomp::IComponent)

void CFolderMonitorComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	connect(this, SIGNAL(FolderChanged(int)), this, SLOT(OnFolderChanged(int)), Qt::QueuedConnection);
}


void CFolderMonitorComp::OnComponentDestroyed()
{
	StopObserverThread();

	BaseClass::OnComponentDestroyed();
}


// reimplemented (iser::ISerializable)

bool CFolderMonitorComp::Serialize(iser::IArchive& archive)
{	
	StopObserverThread();

	if (!archive.IsStoring()){
		ResetFiles();
	}

	bool retVal = BaseClass3::Serialize(archive);

	static iser::CArchiveTag directorySnapShotTag("DirectorySnapshot", "List of already monitored files");
	static iser::CArchiveTag processedFileTag("MonitoredFile", "Already monitored file");

	int processedFileCount = m_directoryFiles.count();
	retVal = retVal && archive.BeginMultiTag(directorySnapShotTag, processedFileTag, processedFileCount);
	if (archive.IsStoring()){
		for (int fileIndex = 0; fileIndex < processedFileCount; fileIndex++){
			istd::CString filePath = iqt::GetCString(m_directoryFiles[fileIndex].absoluteFilePath());
			retVal = retVal && archive.BeginTag(processedFileTag);
			retVal = retVal && archive.Process(filePath);
			retVal = retVal && archive.EndTag(processedFileTag);		
		}
	}
	else{
		for (int fileIndex = 0; fileIndex < processedFileCount; fileIndex++){
			istd::CString filePath;
			retVal = retVal && archive.BeginTag(processedFileTag);
			retVal = retVal && archive.Process(filePath);
			retVal = retVal && archive.EndTag(processedFileTag);

			if (retVal){
				m_directoryFiles.push_back(QFileInfo(iqt::GetQString(filePath)));
			}
		}	
	}

	retVal = retVal && archive.EndTag(directorySnapShotTag);

	m_currentDirectory = QDir(iqt::GetQString(GetPath()));

	StartObserverThread();
	
	return retVal;
}


// protected methods

// reimplemented (QThread)

void CFolderMonitorComp::run()
{
	iqt::CTimer updateTimer;
	
	while (!m_finishThread){
		bool needStateUpdate = (updateTimer.GetElapsed() >= m_poolingFrequency);
		if (!needStateUpdate){
			msleep(100);

			continue;
		}

		QDir::Filters itemFilter = m_observingItemTypes;

		QFileInfoList currentFiles = m_currentDirectory.entryInfoList(m_fileFilterExpressions, itemFilter | QDir::NoDotAndDotDot);

		QStringList addedFiles;
		QStringList removedFiles;
		QStringList modifiedFiles;
		QStringList attributeChangedFiles;

		// check for changes:

		if ((m_observingChanges & ifpf::IDirectoryMonitorParams::OC_REMOVE) != 0){
			for (int fileIndex = 0; fileIndex < int(m_directoryFiles.count()); fileIndex++){
				QFileInfo fileInfo = m_directoryFiles[fileIndex];
				if (!fileInfo.exists()){
					removedFiles.push_back(fileInfo.absoluteFilePath());
					SendInfoMessage(0, iqt::GetCString(fileInfo.absoluteFilePath() + " was removed"));
				}
			}
		}

		if (m_observingChanges != ifpf::IDirectoryMonitorParams::OC_REMOVE){
			for (int currentFileIndex = 0; currentFileIndex < int(currentFiles.count()); currentFileIndex++){
				const QFileInfo& currentFileInfo = currentFiles[currentFileIndex];

				QFileInfoList::iterator foundFileIter = qFind(m_directoryFiles.begin(), m_directoryFiles.end(), currentFileInfo);
				if (foundFileIter == m_directoryFiles.end()){
					if ((m_observingChanges & ifpf::IDirectoryMonitorParams::OC_ADD) != 0){
						addedFiles.push_back(currentFileInfo.absoluteFilePath());
						SendInfoMessage(0, iqt::GetCString(currentFileInfo.absoluteFilePath() + " was added"));
					}
				}
				else{
					if (foundFileIter->lastModified() != currentFileInfo.lastModified()){
						if ((m_observingChanges & ifpf::IDirectoryMonitorParams::OC_MODIFIED) != 0){
							modifiedFiles.push_back(currentFileInfo.absoluteFilePath());
							SendInfoMessage(0, iqt::GetCString(currentFileInfo.absoluteFilePath() + " was modified"));
						}
					}
					if (foundFileIter->permissions() != currentFileInfo.permissions()){
						if ((m_observingChanges & ifpf::IDirectoryMonitorParams::OC_ATTR_CHANGED) != 0){
							attributeChangedFiles.push_back(currentFileInfo.absoluteFilePath());
							SendInfoMessage(0, istd::CString("Attributes of") + iqt::GetCString(currentFileInfo.absoluteFilePath() + " have been changed"));
						}
					}
				}
			}
		}

		m_directoryFiles = currentFiles;

		isys::CSectionBlocker block(&m_lock);

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

void CFolderMonitorComp::OnFolderChanged(int changeFlags)
{
	istd::CChangeNotifier changePtr(this, changeFlags);

	changePtr.Reset();
}


// private methods

void CFolderMonitorComp::SetFolderPath(const QString& folderPath)
{	
	if (m_currentDirectory == QDir(folderPath)){
		return;
	}

	if (folderPath.isEmpty()){
		return;
	}

	StopObserverThread();

	ResetFiles();

	QFileInfo fileInfo(folderPath);
	if (fileInfo.exists()){
		m_currentDirectory = QDir(folderPath);

		StartObserverThread();
	}
	else{
		SendWarningMessage(0, istd::CString("Directory: ") + iqt::GetCString(folderPath) + istd::CString(" not exists. Observing aborted"), "FolderMonitor");
	}
}


void CFolderMonitorComp::StartObserverThread()
{
	m_finishThread = false;

	SendInfoMessage(0, istd::CString("Start observing of: ") + GetPath(), "FolderMonitor");

	BaseClass2::start();
}


void CFolderMonitorComp::StopObserverThread()
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


void CFolderMonitorComp::ResetFiles()
{
	m_directoryFiles.clear();
	m_folderChanges.addedFiles.clear();
	m_folderChanges.attributeChangedFiles.clear();
	m_folderChanges.modifiedFiles.clear();
	m_folderChanges.removedFiles.clear();
}


} // namespace iqtfpf


