#ifndef ifpf_IMonitoringSession_included
#define ifpf_IMonitoringSession_included


// ACF includes
#include "istd/IChangeable.h"
#include "istd/CString.h"

#include "iser/ISerializable.h"

#include "ibase/IFileListProvider.h"

#include "iprm/IFileNameParam.h"


namespace ifpf
{


/**
	Common interface for a monitoring session for a directory.
	Each directory monitor can persist its monitoring state in a monitoring session, which is provided by IMonitoringSessionManager.
	\sa IDirectoryMonitor, IMonitoringSessionManager
*/
class IMonitoringSession:
			virtual public iprm::IFileNameParam,
			virtual public ibase::IFileListProvider,
			virtual public iser::ISerializable
{
public:
	/**
		Get id of the session.
	*/
	virtual istd::CString GetSessionId() const = 0;

	/**
		Set session id.
	*/
	virtual void SetSessionId(const istd::CString& sessionId) = 0;

	/**
		Set the file list.
	*/
	virtual void SetFileList(const istd::CStringList& fileList) = 0;
};


} // namespace ifpf


#endif // !ifpf_IMonitoringSession_included


