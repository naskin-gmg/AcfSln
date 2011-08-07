#include "ifpf/CMonitoringSession.h"


// ACF includes
#include "istd/TChangeNotifier.h"

#include "iser/IArchive.h"
#include "iser/CArchiveTag.h"


namespace ifpf
{	


// public methods

// reimplemented (ifpf::IMonitoringSession)

const CMonitoringSession::FileItems& CMonitoringSession::GetFileInfoList() const
{
	return m_sessionFiles;
}


void CMonitoringSession::SetFileInfoList(const FileItems& fileList)
{
	if (m_sessionFiles != fileList){
		istd::CChangeNotifier changePtr(this);

		m_sessionFiles = fileList;
	}
}


// reimplemented (iser::ISerializable)

bool CMonitoringSession::Serialize(iser::IArchive& archive)
{	
	bool retVal = true;

	static iser::CArchiveTag directorySnapShotTag("DirectorySnapshot", "List of already monitored files");
	static iser::CArchiveTag monitoredFileTag("MonitoredFile", "Already monitored file");
	
	int filesCount = m_sessionFiles.size();
	retVal = retVal && archive.BeginMultiTag(directorySnapShotTag, monitoredFileTag, filesCount);
	if (archive.IsStoring()){
		for (int fileIndex = 0; fileIndex < filesCount; fileIndex++){
			retVal = retVal && archive.BeginTag(monitoredFileTag);

			retVal = retVal && m_sessionFiles[fileIndex].Serialize(archive);
			
			retVal = retVal && archive.EndTag(monitoredFileTag);		
		}
	}
	else{
		m_sessionFiles.clear();

		for (int fileIndex = 0; fileIndex < filesCount; fileIndex++){
			retVal = retVal && archive.BeginTag(monitoredFileTag);

			isys::CFileInfo fileItem;
			retVal = retVal && fileItem.Serialize(archive);
				
			if (retVal){
				m_sessionFiles.push_back(fileItem);
			}

			retVal = retVal && archive.EndTag(monitoredFileTag);		
		}	
	}

	retVal = retVal && archive.EndTag(directorySnapShotTag);

	return retVal;
}


} // namespace ifpf


