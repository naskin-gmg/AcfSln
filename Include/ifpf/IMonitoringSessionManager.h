#ifndef ifpf_IMonitoringSessionManager_included
#define ifpf_IMonitoringSessionManager_included


// ACF includes
#include "istd/IPolymorphic.h"
#include "istd/CString.h"

#include "ifpf/IDirectoryMonitor.h"
#include "ifpf/IMonitoringSession.h"


namespace ifpf
{


/**
	Common interface for a monitoring session's manager.
	Over this interface a directory monitor can get its monitoring session object for a given directory path.
*/
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


