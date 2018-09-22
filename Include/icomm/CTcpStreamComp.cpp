#include <icomm/CTcpStreamComp.h>


// Qt includes
#include <QtNetwork/QHostAddress>

// ACF includes
#include <iprm/TParamsPtr.h>
#include <iprm/INameParam.h>


namespace icomm
{


// public methods

CTcpStreamComp::CTcpStreamComp()
	:m_lock(QMutex::Recursive)
{
	qRegisterMetaType<QAbstractSocket::SocketState>();
	qRegisterMetaType<QAbstractSocket::SocketError>();

	connect(&m_connectionTimer, SIGNAL(timeout()), this, SLOT(OnTimeout()));
}


// reimplemented (icomm::IBinaryStream)

bool CTcpStreamComp::SendData(const void* data, int size, void* /*userContextPtr*/)
{
	if (data == NULL || size <= 0){
		return false;
	}

	QMutexLocker lock(&m_lock);

	m_dataToSent.append((const char*)data, size);

	if (m_socketPtr.IsValid()){
		m_socketPtr->write(m_dataToSent);

		if (m_socketPtr->waitForBytesWritten(1000)){
			m_dataToSent.clear();

			return true;
		}
	}

	return false;
}


bool CTcpStreamComp::ReadData(void* data, int& size)
{
	if ((data == NULL) || (!m_socketPtr.IsValid())){
		return false;
	}

	QMutexLocker lock(&m_lock);

	if (m_dataReceived.isEmpty()){
		size = 0;

		return false;
	}

	memcpy(data, m_dataReceived.constData(), size = m_dataReceived.size());

	m_dataReceived.clear();

	return true;
}


bool CTcpStreamComp::RegisterEventHandler(icomm::IBinaryStream::Handler* /*handlerPtr*/, bool /*allowOtherThread*/)
{
	return false;
}


bool CTcpStreamComp::UnregisterEventHandler(icomm::IBinaryStream::Handler* /*handlerPtr*/)
{
	return false;
}


// reimplemented (icomm::IHostConnection)

bool CTcpStreamComp::ConnectToHost(const iprm::IParamsSet* /*paramsPtr*/)
{
	// reconnect if needed
	if (m_socketPtr.IsValid()){
		Disconnect();
	}
	else{
		return false;
	}

	start();

	return isRunning();
}


void CTcpStreamComp::DisconnectFromHost()
{
	Disconnect();
}


bool CTcpStreamComp::IsHostConnected() const
{
	if (m_socketPtr.IsValid()){
		return m_socketPtr->isOpen();
	}
	else{
		return false;
	}
}


bool CTcpStreamComp::WaitHostConnected(double maxTime, bool /*disconnectOnTimeout = true*/)
{
	if (m_socketPtr.IsValid()){
		return m_socketPtr->waitForConnected(maxTime * 1000);
	}
	else{
		return false;
	}
}


bool CTcpStreamComp::RegisterEventHandler(icomm::IHostConnection::Handler* handlerPtr, bool /*allowOtherThread = false*/)
{
	if (handlerPtr != NULL){
		m_handlers << handlerPtr;

		return true;
	}

	return false;
}


bool CTcpStreamComp::UnregisterEventHandler(icomm::IHostConnection::Handler* handlerPtr)
{
	if (handlerPtr){
		return m_handlers.remove(handlerPtr);
	}

	return false;
}


// protected methods

// reimplemented (ibase::TRuntimeStatusHanderCompWrap)

void CTcpStreamComp::OnSystemStarted()
{
	Connect(m_host, m_port);

	// run timer
	if (m_autoReconnectTimeAttr.IsValid()){
		m_connectionTimer.setInterval(*m_autoReconnectTimeAttr);
		m_connectionTimer.start();
	}
}


void CTcpStreamComp::OnSystemShutdown()
{
	m_connectionTimer.stop();
	m_connectionTimer.disconnect();

	Disconnect();
}


// reimplemented (imod::TSingleModelObserverBase)

void CTcpStreamComp::OnUpdate(const istd::IChangeable::ChangeSet& /*changeSet*/)
{
	const iprm::IParamsSet* paramsPtr = GetObservedObject();
	Q_ASSERT(paramsPtr);

	iprm::TParamsPtr<iprm::INameParam> hostParamPtr(paramsPtr, "Host");
	if (!hostParamPtr.IsValid()){
		SendErrorMessage(0, tr("Host & port parameters not defined"));
		return;
	}

	QString hostAndPort = hostParamPtr->GetName();
	int portIndex = hostAndPort.indexOf(":");
	QString host = hostAndPort.left(portIndex);
	QString port = hostAndPort.mid(portIndex + 1);

	bool isPortOk = false;
	quint16 portValue = 0;
	if (portIndex >= 0){
		portValue = port.toUInt(&isPortOk);
		if (!isPortOk){
			SendErrorMessage(0, tr("Port value is wrong: %1").arg(port));
			return;
		}
	}

	m_host = host;
	m_port = portValue;

	Connect(m_host, m_port);
}


// private slots

void CTcpStreamComp::OnStateChanged(QAbstractSocket::SocketState state)
{
	for (ConnectionHandlers::Iterator iter = m_handlers.begin(); iter != m_handlers.end(); ++iter){
		(*iter)->OnStateChanged(*this, state);
	}

	if (!m_socketPtr.IsValid()){
		return;
	}

	switch (state)
	{
	case QAbstractSocket::UnconnectedState:
		SendInfoMessage(0, tr("No more connected to %1:%2").arg(m_socketPtr->peerName()).arg(m_socketPtr->peerPort()));
		return;

	case QAbstractSocket::HostLookupState:
		SendInfoMessage(0, tr("Looking up host %1:%2").arg(m_socketPtr->peerName()).arg(m_socketPtr->peerPort()));
		return;

	case QAbstractSocket::ConnectingState:
		SendInfoMessage(0, tr("Connecting to %1:%2").arg(m_socketPtr->peerName()).arg(m_socketPtr->peerPort()));
		return;

	case QAbstractSocket::ConnectedState:
		SendInfoMessage(0, tr("Connection established to %1:%2").arg(m_socketPtr->peerName()).arg(m_socketPtr->peerPort()));
		return;

	case QAbstractSocket::BoundState:
		SendInfoMessage(0, tr("Bound to %1:%2").arg(m_socketPtr->peerName()).arg(m_socketPtr->peerPort()));
		return;

	case QAbstractSocket::ClosingState:
		SendInfoMessage(0, tr("Closing socked at %1:%2").arg(m_socketPtr->peerName()).arg(m_socketPtr->peerPort()));
		return;

	default:
		SendInfoMessage(0, tr("Listening host %1:%2 (state %3)").arg(m_socketPtr->peerName()).arg(m_socketPtr->peerPort()));
		return;
	}
}


void CTcpStreamComp::OnSocketError(QAbstractSocket::SocketError /*socketError*/)
{
	if (!m_socketPtr.IsValid()){
		return; 
	}

	SendErrorMessage(0, tr("Socket error %1").arg(m_socketPtr->errorString()));

	for (ConnectionHandlers::Iterator iter = m_handlers.begin(); iter != m_handlers.end(); ++iter){
		(*iter)->OnError(*this, m_socketPtr->errorString());
	}
}


void CTcpStreamComp::OnConnected()
{
	if (!m_socketPtr.IsValid()){
		return;
	}

	SendInfoMessage(0, tr("Connected to %1:%2").arg(m_socketPtr->peerName()).arg(m_socketPtr->peerPort()));

	for (ConnectionHandlers::Iterator iter = m_handlers.begin(); iter != m_handlers.end(); ++iter){
		(*iter)->OnAttached(*this);
	}
}


void CTcpStreamComp::OnDisconnected()
{
	if (!m_socketPtr.IsValid()){
		return; 
	}

	SendWarningMessage(0, tr("Disconnected from %1:%2").arg(m_socketPtr->peerName()).arg(m_socketPtr->peerPort()));

	for (ConnectionHandlers::Iterator iter = m_handlers.begin(); iter != m_handlers.end(); ++iter){
		(*iter)->OnDetached(*this);
	}
}


void CTcpStreamComp::OnTimeout()
{
	if (!m_socketPtr.IsValid()){
		Connect(m_host, m_port);

		return;
	}

	int state = m_socketPtr->state();
	if (state == QAbstractSocket::UnconnectedState){
		SendErrorMessage(0, tr("Can not connect to %1:%2, trying again...").arg(m_host).arg(m_port));

		for (ConnectionHandlers::Iterator iter = m_handlers.begin(); iter != m_handlers.end(); ++iter){
			(*iter)->OnError(*this, tr("Timeout"));
		}

		Connect(m_host, m_port);
	}
}


void CTcpStreamComp::OnReadyRead()
{
	if (!m_socketPtr.IsValid()){
		return; 
	}

	QMutexLocker lock(&m_lock);

	m_dataReceived.append(m_socketPtr->readAll());
}


// reimplemented (icomp::CComponentBase)

void CTcpStreamComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();
}


void CTcpStreamComp::OnComponentDestroyed()
{
	Disconnect();

	m_socketPtr->disconnect(this);

	m_socketPtr.Reset();
	
	BaseClass::OnComponentDestroyed();
}


// reimplemented (QThread)

void CTcpStreamComp::run()
{
	// create it here in the main thread
	if (!m_socketPtr.IsValid()){
		m_socketPtr.SetPtr(new QTcpSocket(this));
		m_socketPtr->moveToThread(this);
		m_socketPtr->setSocketOption(QAbstractSocket::KeepAliveOption, 1);

		// connect slots
		connect(m_socketPtr.GetPtr(), SIGNAL(connected()), this, SLOT(OnConnected()));
		connect(m_socketPtr.GetPtr(), SIGNAL(disconnected()), this, SLOT(OnDisconnected()));
		connect(m_socketPtr.GetPtr(), SIGNAL(readyRead()), this, SLOT(OnReadyRead()));
		connect(m_socketPtr.GetPtr(), SIGNAL(stateChanged(QAbstractSocket::SocketState)), this, SLOT(OnStateChanged(QAbstractSocket::SocketState)));
		connect(m_socketPtr.GetPtr(), SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(OnSocketError(QAbstractSocket::SocketError)));
	}

	// try to connect
	{
		QMutexLocker lock(&m_lock);

		m_socketPtr->connectToHost(m_host, m_port, QIODevice::ReadWrite);

		if (!m_socketPtr->waitForConnected(*m_connectionTimeoutAttr)){
			SendErrorMessage(0, tr("Connection to %1:%2 failed").arg(m_host).arg(m_port));
			return;
		}
	}

	// run the loop
	exec();

	// leave the loop
	m_socketPtr->disconnectFromHost();
}


// private methods

void CTcpStreamComp::Connect(const QString &address, quint16 port)
{
	// reconnect if needed
	if (m_socketPtr.IsValid()){
		Disconnect();
	}

	m_host = address;
	m_port = port;

	// run thread
	start();
}


void CTcpStreamComp::Disconnect()
{
	if (!m_socketPtr.IsValid()){
		return;
	}

	quit();
	wait();
}


} // namespace icomm


