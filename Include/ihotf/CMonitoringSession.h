#ifndef ihotf_CMonitoringSession_included
#define ihotf_CMonitoringSession_included


// AcfSln includes
#include "ihotf/IMonitoringSession.h"


namespace ihotf
{


/**
	Implementation of the ihotf::IMonitoringSession interface.
*/
class CMonitoringSession: virtual public ihotf::IMonitoringSession
{
public:
	// reimplemented (ihotf::IMonitoringSession)
	virtual const FileItems& GetFileInfoList() const;
	virtual void SetFileInfoList(const FileItems& fileList);

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);

private:
	FileItems m_sessionFiles;
};


} // namespace ihotf


#endif // !ihotf_CMonitoringSession_included


