// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
 #ifndef ihotf_CMonitoringSessionsManager_included
#define ihotf_CMonitoringSessionsManager_included


// Qt includes
#include <QtCore/QMap>

// ACF includes
#include <istd/TDelPtr.h>

// AcfSln includes
#include <ihotf/IMonitoringSessionManager.h>
#include <ihotf/CMonitoringSession.h>


namespace ihotf
{


class CMonitoringSessionsManager:
			virtual public iser::ISerializable,
			virtual public ihotf::IMonitoringSessionManager
{
public:
	// reimplemented (ihotf::IMonitoringSessionManager)
	virtual void ResetSessions() override;
	virtual ihotf::IMonitoringSession* GetSession(const QString& directoryPath) const override;
	virtual void RemoveSession(const QString& directoryPath) override;

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive) override;

private:
	typedef QMap<QString, istd::TDelPtr<ihotf::CMonitoringSession> > MonitoringSessionsMap;
	mutable MonitoringSessionsMap m_monitorSessionsMap;
};


} // namespace ihotf


#endif // !ihotf_CMonitoringSessionsManager_included


