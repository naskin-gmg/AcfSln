#include "ifpf/CMonitoringSession.h"


// ACF includes
#include "istd/TChangeNotifier.h"

#include "iser/IArchive.h"
#include "iser/CArchivetag.h"


namespace ifpf
{	


// public methods

// reimplemented (ifpf::IMonitoringSession)

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


// reimplemented (iser::ISerializable)

bool CMonitoringSession::Serialize(iser::IArchive& archive)
{	
	bool retVal = true;

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


