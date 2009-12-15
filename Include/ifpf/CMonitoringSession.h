#ifndef ifpf_CMonitoringSession_included
#define ifpf_CMonitoringSession_included


// AcfSln includes
#include "ifpf/IMonitoringSession.h"


namespace ifpf
{


/**
	Implementation of the ifpf::IMonitoringSession interface.
*/
class CMonitoringSession: virtual public ifpf::IMonitoringSession
{
public:
	// reimplemented (ifpf::IMonitoringSession)
	virtual void SetFileList(const istd::CStringList& fileList);

	// reimplemented (ibase::IFileListProvider)
	virtual istd::CStringList GetFileList() const;

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);

private:
	istd::CStringList m_sessionFiles;
};


} // namespace ifpf


#endif // !ifpf_CMonitoringSession_included


