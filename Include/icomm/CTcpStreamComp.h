#pragma once


// Qt includes
#include <QtNetwork/QTcpSocket>
#include <QtCore/QTimer>
#include <QtCore/QThread>
#include <QtCore/QMutex>

// ACF includes
#include <iprm/IParamsSet.h>
#include <imod/TSingleModelObserverBase.h>
#include <imod/TModelWrap.h>
#include <ilog/TLoggerCompWrap.h>
#include <icomm/IBinaryStream.h>
#include <icomm/IHostConnection.h>
#include <ibase/TRuntimeStatusHanderCompWrap.h>


namespace icomm
{


class CTcpStreamComp: 
			public QThread,
			public ibase::TRuntimeStatusHanderCompWrap<ilog::CLoggerComponentBase>,
			public imod::TSingleModelObserverBase<iprm::IParamsSet>,
			public icomm::IBinaryStream,
			public icomm::IHostConnection
{
	Q_OBJECT

public:
	typedef ibase::TRuntimeStatusHanderCompWrap<ilog::CLoggerComponentBase> BaseClass;
	typedef imod::TSingleModelObserverBase<iprm::IParamsSet> BaseClass2;

	I_BEGIN_COMPONENT(CTcpStreamComp);
		I_REGISTER_INTERFACE(icomm::IBinaryStream);
		I_REGISTER_INTERFACE(icomm::IHostConnection);
		I_ASSIGN(m_autoReconnectTimeAttr, "AutoReconnectTime", "Check for reconnection every N ms", true, 5000);
		I_ASSIGN(m_connectionTimeoutAttr, "ConnectionTimeout", "Wait up to N ms for connection", true, 2000);
	I_END_COMPONENT;

	CTcpStreamComp();

	// reimplemented (icomm::IBinaryStream)
	virtual bool SendData(const void* data, int size, void* userContextPtr = NULL);
	virtual bool ReadData(void* data, int& size);
	virtual bool RegisterEventHandler(icomm::IBinaryStream::Handler* handlerPtr, bool /*allowOtherThread = false*/);
	virtual bool UnregisterEventHandler(icomm::IBinaryStream::Handler* handlerPtr);

	// reimplemented (icomm::IHostConnection)
	virtual QString GetHostUrl() const;
	virtual bool ConnectToHost(const iprm::IParamsSet* paramsPtr);
	virtual void DisconnectFromHost();
	virtual bool IsHostConnected() const;
	virtual bool WaitHostConnected(double maxTime, bool disconnectOnTimeout = true);
	virtual bool RegisterEventHandler(icomm::IHostConnection::Handler* handlerPtr, bool /*allowOtherThread = false*/);
	virtual bool UnregisterEventHandler(icomm::IHostConnection::Handler* handlerPtr);

protected:
	// reimplemented (ibase::TRuntimeStatusHanderCompWrap)
	virtual void OnSystemStarted();
	virtual void OnSystemShutdown();

	// reimplemented (imod::TSingleModelObserverBase)
	virtual void OnUpdate(const istd::IChangeable::ChangeSet& changeSet);

	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated();
	virtual void OnComponentDestroyed();

	// reimplemented (QThread)
	virtual void run();

private Q_SLOTS:
	void OnStateChanged(QAbstractSocket::SocketState);
	void OnSocketError(QAbstractSocket::SocketError);
	void OnConnected();
	void OnDisconnected();
	void OnTimeout();
	void OnReadyRead();

private:
	void Connect(const QString &address, quint16 port = 0);
	void Disconnect();

	istd::TDelPtr<QTcpSocket> m_socketPtr;
	QTimer m_connectionTimer;
	QString m_host;
	quint16 m_port;
	QByteArray m_dataReceived;
	QByteArray m_dataToSent;
	QMutex m_lock;

	typedef QSet<icomm::IHostConnection::Handler*> ConnectionHandlers;
	ConnectionHandlers m_handlers;

	I_ATTR(unsigned, m_autoReconnectTimeAttr);
	I_ATTR(unsigned, m_connectionTimeoutAttr);
};


} // namespace icomm


