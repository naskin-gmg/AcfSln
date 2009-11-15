#include "ifpf/CMonitoringSession.h"


// ACF includes
#include "istd/TChangeNotifier.h"

#include "iser/IArchive.h"
#include "iser/CArchivetag.h"


namespace ifpf
{	


// public methods

// reimplemented (ifpf::IMonitoringSession)

istd::CString CMonitoringSession::GetSessionId() const
{
	return m_sessionId;
}


void CMonitoringSession::SetSessionId(const istd::CString& sessionId)
{
	if (sessionId != m_sessionId){
		istd::CChangeNotifier changePtr(this);

		m_sessionId = sessionId;
	}
}


void CMonitoringSession::SetFileList(const istd::CStringList& fileList)
{
	if (m_sessionFiles != fileList){
		istd::CChangeNotifier changePtr(this);

		m_sessionFiles = fileList;
	}
}


// reimplemented (ibase::IFileListProvider)

istd::CStringList CMonitoringSession::GetFileList() const
{
	return m_sessionFiles;
}


// reimplemented (iprm::IFileNameParam)

int CMonitoringSession::GetPathType() const
{
	return PT_DIRECTORY;
}


const istd::CString& CMonitoringSession::GetPath() const
{
	return m_monitoredDirectoryPath;
}


void CMonitoringSession::SetPath(const istd::CString& path)
{
	if (m_monitoredDirectoryPath != path){
		istd::CChangeNotifier changePtr(this);

		m_monitoredDirectoryPath = path;
	}
}


// reimplemented (iser::ISerializable)

bool CMonitoringSession::Serialize(iser::IArchive& archive)
{	
	static iser::CArchiveTag monitoredDirectoryPathTag("MonitoredDirectoryPath", "Path of the currently monitored directory");
	bool retVal = archive.BeginTag(monitoredDirectoryPathTag);
	retVal = retVal && archive.Process(m_monitoredDirectoryPath);
	retVal = retVal && archive.EndTag(monitoredDirectoryPathTag);

	static iser::CArchiveTag sessionIdTag("SessionId", "Session ID");
	retVal = retVal && archive.BeginTag(sessionIdTag);
	retVal = retVal && archive.Process(m_sessionId);
	retVal = retVal && archive.EndTag(sessionIdTag);
	
	static iser::CArchiveTag directorySnapShotTag("DirectorySnapshot", "List of already monitored files");
	static iser::CArchiveTag processedFileTag("MonitoredFile", "Already monitored file");

	int processedFileCount = m_sessionFiles.size();
	retVal = retVal && archive.BeginMultiTag(directorySnapShotTag, processedFileTag, processedFileCount);
	if (archive.IsStoring()){
		for (int fileIndex = 0; fileIndex < processedFileCount; fileIndex++){
			retVal = retVal && archive.BeginTag(processedFileTag);
			retVal = retVal && archive.Process(m_sessionFiles[fileIndex]);
			retVal = retVal && archive.EndTag(processedFileTag);		
		}
	}
	else{
		m_sessionFiles.clear();

		for (int fileIndex = 0; fileIndex < processedFileCount; fileIndex++){
			istd::CString filePath;
			retVal = retVal && archive.BeginTag(processedFileTag);
			retVal = retVal && archive.Process(filePath);
			retVal = retVal && archive.EndTag(processedFileTag);

			if (retVal){
				m_sessionFiles.push_back(filePath);
			}
		}	
	}

	retVal = retVal && archive.EndTag(directorySnapShotTag);

	return retVal;
}


} // namespace ifpf


