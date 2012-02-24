#include "ifpf/CMonitoringSessionsManager.h"


// ACF includes
#include "istd/TChangeNotifier.h"

#include "iser/IArchive.h"
#include "iser/CArchiveTag.h"


namespace ifpf
{	


// public methods

// reimplemented (ifpf::IMonitoringSessionManager)

void CMonitoringSessionsManager::ResetSessions()
{
	m_monitorSessionsMap.clear();
}


ifpf::IMonitoringSession* CMonitoringSessionsManager::GetSession(const QString& directoryPath) const
{
	MonitoringSessionsMap::const_iterator sessionIter = m_monitorSessionsMap.find(directoryPath);
	if (sessionIter != m_monitorSessionsMap.end()){
		return sessionIter->second.GetPtr();
	}

	ifpf::CMonitoringSession* newSessionPtr = new ifpf::CMonitoringSession;

	m_monitorSessionsMap[directoryPath] = newSessionPtr;

	return newSessionPtr;
}


// reimplemented (iser::ISerializable)

bool CMonitoringSessionsManager::Serialize(iser::IArchive& archive)
{	
	bool retVal = true;

	static iser::CArchiveTag monitoringSessionsTag("MonitoringSessionsManager", "Session list");
	static iser::CArchiveTag monitoringSessionTag("MonitoringSession", "Single session");
	static iser::CArchiveTag directoryPathTag("MonitoringDirectory", "Monitoring directory path");

	int sessionsCount = m_monitorSessionsMap.size();
	retVal = retVal && archive.BeginMultiTag(monitoringSessionsTag, monitoringSessionTag, sessionsCount);
	if (archive.IsStoring()){
		for (MonitoringSessionsMap::iterator index = m_monitorSessionsMap.begin(); index != m_monitorSessionsMap.end(); index++){
			QString directoryPath = index->first;

			retVal = retVal && archive.BeginTag(monitoringSessionTag);
		
			retVal = retVal && archive.BeginTag(directoryPathTag);
			retVal = retVal && archive.Process(directoryPath);
			retVal = retVal && archive.EndTag(directoryPathTag);
			
			retVal = retVal && index->second->Serialize(archive);

			retVal = retVal && archive.EndTag(monitoringSessionTag);		
		}
	}
	else{
		m_monitorSessionsMap.clear();

		for (int fileIndex = 0; fileIndex < sessionsCount; fileIndex++){
			ifpf::CMonitoringSession* sessionPtr = new ifpf::CMonitoringSession;

			retVal = retVal && archive.BeginTag(monitoringSessionTag);

			QString directoryPath;
			retVal = retVal && archive.BeginTag(directoryPathTag);
			retVal = retVal && archive.Process(directoryPath);
			retVal = retVal && archive.EndTag(directoryPathTag);
			

			retVal = retVal && sessionPtr->Serialize(archive);

			retVal = retVal && archive.EndTag(monitoringSessionTag);

			if (retVal){
				m_monitorSessionsMap[directoryPath] = sessionPtr;
			}
		}	
	}

	retVal = retVal && archive.EndTag(monitoringSessionsTag);

	return retVal;
}


} // namespace ifpf


