#ifndef ihotf_IMonitoringSessionManager_included
#define ihotf_IMonitoringSessionManager_included


// ACF includes
#include "istd/IPolymorphic.h"
#include <QtCore/QString>


// ACF-Solutions includes
#include "ihotf/IMonitoringSession.h"


namespace ihotf
{


/**
	Common interface for a monitoring session's manager.
	Over this interface a directory monitor can get its monitoring session object for a given directory path.
*/
class IMonitoringSessionManager: virtual public istd::IPolymorphic
{
public:

	/**
		Clear all session data.
	*/
	virtual void ResetSessions() = 0;

	/**
		Get monitoring session for the given monitor and directory path.
	*/
	virtual ihotf::IMonitoringSession* GetSession(const QString& directoryPath) const = 0;
};


} // namespace ihotf


#endif // !ihotf_IMonitoringSessionManager_included


