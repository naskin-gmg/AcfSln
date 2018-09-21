#include <icomm/CUdpStreamComp.h>

// ACF includes
#include <iprm/TParamsPtr.h>
#include <iprm/INameParam.h>


namespace icomm
{


// public

CUdpStreamComp::CUdpStreamComp()
:	m_readThread(this)
{
}


// reimplemented (icomm::IBinaryStream)

bool CUdpStreamComp::SendData(const void* /*data*/, int /*size*/, void* /*userContextPtr*/)
{
	// not implemented yet
	return false; 
}


bool CUdpStreamComp::ReadData(void* data, int& size)
{
	return m_readThread.ReadData(data, size);
}


bool CUdpStreamComp::RegisterEventHandler(icomm::IBinaryStream::Handler* handlerPtr, bool allowOtherThread)
{
	return m_readThread.RegisterEventHandler(handlerPtr, allowOtherThread);
}


bool CUdpStreamComp::UnregisterEventHandler(icomm::IBinaryStream::Handler* handlerPtr)
{
	return m_readThread.UnregisterEventHandler(handlerPtr);
}


// protected methods

// reimplemented (imod::TSingleModelObserverBase)

void CUdpStreamComp::OnUpdate(const istd::IChangeable::ChangeSet& /*changeSet*/)
{
	const iprm::IParamsSet* paramsPtr = GetObservedObject();
	Q_ASSERT(paramsPtr);

	iprm::TParamsPtr<iprm::INameParam> hostParam(paramsPtr, "Host");
	if (!hostParam.IsValid()){
		SendErrorMessage(0, tr("Host & port parameters not defined"));
		return;
	}

	QString hostAndPort = hostParam->GetName();
	int portIndex = hostAndPort.indexOf(":");
	QString host = hostAndPort.left(portIndex);
	QString port = hostAndPort.mid(portIndex+1);

	bool isPortOk = false;
	quint16 portValue = 0;
	if (portIndex >= 0){
		portValue = port.toUInt(&isPortOk);
		if (!isPortOk){
			SendErrorMessage(0, tr("Port value is wrong: %1").arg(port));
			return;
		}
	}

	QHostAddress hostValue(host);

	if (m_readThread.Connect(hostValue, portValue)){
		SendInfoMessage(0, tr("Connected to %1:%2").arg(host).arg(portValue));
	}
	else{
		SendErrorMessage(0, tr("Can not connect to %1:%2").arg(host).arg(portValue));
	}
}


// reimplemented (icomp::CComponentBase)

void CUdpStreamComp::OnComponentDestroyed()
{
	m_readThread.Disconnect();

	BaseClass::OnComponentDestroyed();
}


// CUdpStreamComp::ReadThread

CUdpStreamComp::ReadThread::ReadThread(CUdpStreamComp* parent)
:	m_lock(QMutex::Recursive),
	m_stopThread(true),
	m_parent(parent),
	m_handlerPtr(NULL)
{
	m_socket.moveToThread(this);
}


bool CUdpStreamComp::ReadThread::Connect(const QHostAddress &address, quint16 port)
{
	Disconnect();

	if (m_socket.bind(address, port)){
		m_stopThread = false;
		
		start(QThread::TimeCriticalPriority);

		return true;
	}

	return false;
}


void CUdpStreamComp::ReadThread::Disconnect()
{
	m_stopThread = true;
	
	wait();

	m_socket.disconnectFromHost();
}


bool CUdpStreamComp::ReadThread::ReadData(void* dataPtr, int& size)
{
	QMutexLocker lock(&m_lock);

	size = m_lastData.size();

	if (m_lastData.isEmpty() || (dataPtr == NULL)){
		return false;
	}

	memcpy(dataPtr, m_lastData.data(), size);

	return true;
}


bool CUdpStreamComp::ReadThread::RegisterEventHandler(icomm::IBinaryStream::Handler* handlerPtr, bool /*allowOtherThread = false*/)
{
	QMutexLocker lock(&m_lock);

	if (handlerPtr){
		m_handlerPtr = handlerPtr;

		return true;
	}

	return false;
}


bool CUdpStreamComp::ReadThread::UnregisterEventHandler(icomm::IBinaryStream::Handler* handlerPtr)
{
	QMutexLocker lock(&m_lock);

	if ((m_handlerPtr == handlerPtr) || (handlerPtr == NULL)){
		m_handlerPtr = NULL;

		return true;
	}

	return false;
}


// reimplemented (QThread)

void CUdpStreamComp::ReadThread::run()
{
	while (!m_stopThread){
		m_lock.lock();

		while (m_socket.hasPendingDatagrams()){
			m_lastDateTime = QDateTime::currentDateTime();

			QHostAddress sender;
			quint16 senderPort;

			m_lastData.resize(m_socket.pendingDatagramSize());
			m_socket.readDatagram(m_lastData.data(), m_lastData.size(), &sender, &senderPort);

			if (m_handlerPtr){
				m_handlerPtr->OnDataReceived(*m_parent);
			}
		}

		m_lock.unlock();

		usleep(10);
	}
}


} // namespace icomm


