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
	virtual const FileItems& GetFileInfoList() const;
	virtual void SetFileInfoList(const FileItems& fileList);

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);

private:
	FileItems m_sessionFiles;
};


} // namespace ifpf


#endif // !ifpf_CMonitoringSession_included


