#include "ihotf/CMonitoringSessionsManager.h"


// ACF includes
#include "istd/CChangeNotifier.h"

#include "iser/IArchive.h"
#include "iser/CArchiveTag.h"


namespace ihotf
{	


// public methods

// reimplemented (ihotf::IMonitoringSessionManager)

void CMonitoringSessionsManager::ResetSessions()
{
	istd::CChangeNotifier changePtr(this);

	m_monitorSessionsMap.clear();
}


ihotf::IMonitoringSession* CMonitoringSessionsManager::GetSession(const QString& directoryPath) const
{
	MonitoringSessionsMap::ConstIterator sessionIter = m_monitorSessionsMap.constFind(directoryPath);
	if (sessionIter != m_monitorSessionsMap.constEnd()){
		return sessionIter.value().GetPtr();
	}

	ihotf::CMonitoringSession* newSessionPtr = new ihotf::CMonitoringSession;

	m_monitorSessionsMap[directoryPath] = newSessionPtr;

	return newSessionPtr;
}


void CMonitoringSessionsManager::RemoveSession(const QString& directoryPath)
{
	istd::CChangeNotifier changePtr(this);

	m_monitorSessionsMap.remove(directoryPath);
}


// reimplemented (iser::ISerializable)

bool CMonitoringSessionsManager::Serialize(iser::IArchive& archive)
{	
	bool retVal = true;

	static iser::CArchiveTag monitoringSessionsTag("MonitoringSessionsManager", "Session list", iser::CArchiveTag::TT_MULTIPLE);
	static iser::CArchiveTag monitoringSessionTag("MonitoringSession", "Single session", iser::CArchiveTag::TT_GROUP, &monitoringSessionsTag);
	static iser::CArchiveTag directoryPathTag("MonitoringDirectory", "Monitoring directory path", iser::CArchiveTag::TT_LEAF);

	int sessionsCount = m_monitorSessionsMap.size();
	retVal = retVal && archive.BeginMultiTag(monitoringSessionsTag, monitoringSessionTag, sessionsCount);
	if (archive.IsStoring()){
		for (MonitoringSessionsMap::iterator index = m_monitorSessionsMap.begin(); index != m_monitorSessionsMap.end(); index++){
			QString directoryPath = index.key();

			retVal = retVal && archive.BeginTag(monitoringSessionTag);
		
			retVal = retVal && archive.BeginTag(directoryPathTag);
			retVal = retVal && archive.Process(directoryPath);
			retVal = retVal && archive.EndTag(directoryPathTag);
			
			retVal = retVal && index.value()->Serialize(archive);

			retVal = retVal && archive.EndTag(monitoringSessionTag);		
		}
	}
	else{
		m_monitorSessionsMap.clear();

		for (int fileIndex = 0; fileIndex < sessionsCount; fileIndex++){
			ihotf::CMonitoringSession* sessionPtr = new ihotf::CMonitoringSession;

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


} // namespace ihotf


