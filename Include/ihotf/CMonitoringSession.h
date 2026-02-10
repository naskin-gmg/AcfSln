// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#ifndef ihotf_CMonitoringSession_included
#define ihotf_CMonitoringSession_included


// AcfSln includes
#include <ihotf/IMonitoringSession.h>


namespace ihotf
{


/**
	Implementation of the ihotf::IMonitoringSession interface.
*/
class CMonitoringSession: virtual public ihotf::IMonitoringSession
{
public:
	// reimplemented (ihotf::IMonitoringSession)
	virtual const FileItems& GetFileInfoList() const override;
	virtual void SetFileInfoList(const FileItems& fileList) override;
	virtual void RemoveFile(const QString& filePath) override;

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive) override;

private:
	FileItems m_sessionFiles;
};


} // namespace ihotf


#endif // !ihotf_CMonitoringSession_included


