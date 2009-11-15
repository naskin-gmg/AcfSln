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


class IMonitoringSession:
			virtual public ibase::IFileListProvider,
			virtual public iser::ISerializable,
			virtual public iprm::IFileNameParam
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


