#ifndef ifpf_CMonitoringSession_included
#define ifpf_CMonitoringSession_included


// AcfSln includes
#include "ifpf/IMonitoringSession.h"


namespace ifpf
{


class CMonitoringSession: virtual public ifpf::IMonitoringSession
{
public:
	// reimplemented (ifpf::IMonitoringSession)
	virtual istd::CString GetSessionId() const;
	virtual void SetSessionId(const istd::CString& sessionId);
	virtual void SetFileList(const istd::CStringList& fileList);

	// reimplemented (ibase::IFileListProvider)
	virtual istd::CStringList GetFileList() const;

	// reimplemented (iprm::IFileNameParam)
	virtual int GetPathType() const;
	virtual const istd::CString& GetPath() const;
	virtual void SetPath(const istd::CString& path);

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);

private:
	istd::CString m_sessionId;
	istd::CStringList m_sessionFiles;
	istd::CString m_monitoredDirectoryPath;
};


} // namespace ifpf


#endif // !ifpf_CMonitoringSession_included


