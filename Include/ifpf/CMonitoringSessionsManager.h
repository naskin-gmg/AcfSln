 #ifndef ifpf_CMonitoringSessionsManager_included
#define ifpf_CMonitoringSessionsManager_included


// STL includes
#include <map>


// ACF includes
#include "istd/TDelPtr.h"


// AcfSln includes
#include "ifpf/IMonitoringSessionManager.h"
#include "ifpf/CMonitoringSession.h"


namespace ifpf
{


class CMonitoringSessionsManager:
			virtual public iser::ISerializable,
			virtual public ifpf::IMonitoringSessionManager
{
public:
	// reimplemented (ifpf::IMonitoringSessionManager)
	virtual void ResetSessions();
	virtual ifpf::IMonitoringSession* GetSession(const istd::CString& directoryPath) const;

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);

private:
	typedef std::map<istd::CString, istd::TDelPtr<ifpf::CMonitoringSession> > MonitoringSessionsMap;
	mutable MonitoringSessionsMap m_monitorSessionsMap;
};


} // namespace ifpf


#endif // !ifpf_CMonitoringSessionsManager_included


