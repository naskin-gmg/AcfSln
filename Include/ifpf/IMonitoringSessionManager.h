#ifndef ifpf_IMonitoringSessionManager_included
#define ifpf_IMonitoringSessionManager_included


// ACF includes
#include "istd/IPolymorphic.h"
#include "istd/CString.h"

#include "ifpf/IDirectoryMonitor.h"
#include "ifpf/IMonitoringSession.h"


namespace ifpf
{


class IMonitoringSessionManager: virtual public istd::IPolymorphic
{
public:
	/**
		Get monitoring session for the given monitor and directory path.
	*/
	virtual ifpf::IMonitoringSession* GetSession(const ifpf::IDirectoryMonitor& directoryMonitor, const istd::CString& directoryPath) const = 0;
};


} // namespace ifpf


#endif // !ifpf_IMonitoringSessionManager_included


