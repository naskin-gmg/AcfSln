#ifndef ihotf_IMonitoringSessionManager_included
#define ihotf_IMonitoringSessionManager_included


// Qt includes
#include <QtCore/QString>

// ACF includes
#include <istd/IChangeable.h>
#include <ihotf/IMonitoringSession.h>


namespace ihotf
{


/**
	Common interface for a monitoring session's manager.
	Over this interface a directory monitor can get its monitoring session object for a given directory path.
*/
class IMonitoringSessionManager: virtual public istd::IChangeable
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

	/**
		Remove the monitoring session for a given directory path.
	*/
	virtual void RemoveSession(const QString& directoryPath) = 0;
};


} // namespace ihotf


#endif // !ihotf_IMonitoringSessionManager_included


