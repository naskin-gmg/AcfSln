#ifndef ifpf_IMonitoringSession_included
#define ifpf_IMonitoringSession_included


// ACF includes
#include "istd/IChangeable.h"
#include "istd/CString.h"

#include "isys/CFileInfo.h"

#include "iser/ISerializable.h"


namespace ifpf
{


/**
	Common interface for a monitoring session for a directory.
	Each directory monitor can persist its monitoring state in a monitoring session, which is provided by IMonitoringSessionManager.
	\sa IMonitoringSessionManager
*/
class IMonitoringSession: virtual public iser::ISerializable
{
public:
	typedef std::vector<isys::CFileInfo> FileItems;

	/**
		Get the list of file items.
	*/
	virtual const FileItems& GetFileInfoList() const = 0;

	/**
		Set the file list.
	*/
	virtual void SetFileInfoList(const FileItems& fileList) = 0;
};


} // namespace ifpf


#endif // !ifpf_IMonitoringSession_included


