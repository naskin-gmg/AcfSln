#pragma once


// Qt includes
#include <QtCore/QObject>
#include <QtNetwork/QHostInfo>
#include <QtNetwork/QUdpSocket>
#include <QtCore/QTimer>

// ACF includes
#include <icomp/CComponentBase.h>
#include <icomm/IHeartbeatServersManager.h>


namespace icomm
{


class CUdpServersManagerComp:
			public QObject,
			public icomp::CComponentBase,
			public icomm::IHeartbeatServersManager
{
	Q_OBJECT
public:
	typedef icomp::CComponentBase BaseClass;

	I_BEGIN_COMPONENT(CUdpServersManagerComp);
		I_REGISTER_INTERFACE(icomm::IHeartbeatServersManager);
		I_ASSIGN(m_applicationInfoCompPtr, "ApplicationInfo", "ApplicationInfo", true, "ApplicationInfo");
		I_ASSIGN(m_timerPeriodAttrPtr, "HearbeatTimerPeriod", "HearbeatTimerPeriod", true, 1000);
		I_ASSIGN(m_maxLostHeartbeatsAttrPtr, "MaxLostHeartbeats", "MaxLostHeartbeats", true, 10);
		I_ASSIGN(m_includedeartbeatServerPortAttrPtr, "HeartbeatServerPort", "HeartbeatServerPort", true, 0);
		I_ASSIGN(m_includedeartbeatClientPortAttrPtr, "HeartbeatClientPort", "HeartbeatClientPort", true, 0);
		I_ASSIGN(m_searchTimeAttrPtr, "SearchTime", "SearchTime", false, 1000);
	I_END_COMPONENT;

	CUdpServersManagerComp();

	// reimplemented (icomm::IHeartbeatServersManager)
	virtual bool FindServers(FoundServers& result);
	virtual icomm::CHeartbeatAnswer& GetServerInfoImpl(int index) const;
	virtual int CreateServerInfo(int index = -1, int copyFromIndex = -1);
	virtual bool RemoveServerInfo(int index);

	// reimplemented (icomm::IServersManager)
	virtual int GetServersCount() const;
	virtual const icomm::IServerInfo& GetServerInfo(int index) const;

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);

public Q_SLOTS:
	void OnHeartbeatRead();
	void OnTimerTick();
	void OnHostInfoFound(const QHostInfo& info);

protected:
	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated() override;
	virtual void OnComponentDestroyed() override;

private:
	struct ServerInfo{
		icomm::CHeartbeatAnswer stationInfo;
		QHostAddress address;
		int lookupId;
		int lostHeartbeats;

		ServerInfo(): lookupId(-1), lostHeartbeats(0){}
	};

	typedef QVector<ServerInfo> ServerInfos;

	FoundServers m_answers;

	QUdpSocket m_heartbeatSocket;
	QTimer m_timer;

	I_REF(ibase::IApplicationInfo, m_applicationInfoCompPtr);

	I_ATTR(double, m_timerPeriodAttrPtr);
	I_ATTR(int, m_maxLostHeartbeatsAttrPtr);
	I_ATTR(int, m_includedeartbeatServerPortAttrPtr);
	I_ATTR(int, m_includedeartbeatClientPortAttrPtr);
	I_ATTR(double, m_searchTimeAttrPtr);

	ServerInfos m_servers;

	// shadows of attributes
	int m_maxLostHeartbeats;
};


} // namespace icomm


