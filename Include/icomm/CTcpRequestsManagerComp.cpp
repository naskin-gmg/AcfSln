#include <icomm/CTcpRequestsManagerComp.h>


// Qt includes
#include <QtCore/QCoreApplication>
#include <QtCore/QElapsedTimer>
#include <QtCore/QUrl>
#include <QtNetwork/QHostAddress>

// ACF include
#include <iser/CMemoryReadArchive.h>
#include <iser/CMemoryWriteArchive.h>
#include <iprm/TParamsPtr.h>


#include <icomm/IRequest.h>


namespace icomm
{


CTcpRequestsManagerComp::CTcpRequestsManagerComp()
:	m_isConnected(false)
{
	connect(&m_socket, SIGNAL(readyRead()), this, SLOT(OnReadyRead()));
	connect(&m_socket, SIGNAL(connected()), this, SLOT(OnSocketConnected()));
	connect(&m_socket, SIGNAL(disconnected()), this, SLOT(OnSocketDisconnected()));
	connect(&m_socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(OnSocketError(QAbstractSocket::SocketError)));

	m_isConnected = false;
}


// reimplemented (icomm::CRequestsManagerBase)

iser::IArchive* CTcpRequestsManagerComp::CreateSendArchive()
{
	if (m_isConnected){
		return new iser::CMemoryWriteArchive(&m_commonVersionInfo);
	}

	return NULL;
}


bool CTcpRequestsManagerComp::RemoveSendArchive(iser::IArchive& archive, bool wasError)
{
	iser::CMemoryWriteArchive* archiveImplPtr = dynamic_cast<iser::CMemoryWriteArchive*>(&archive);
	Q_ASSERT(archiveImplPtr != NULL);	// only archive created using CreateSendArchive() can be used as parameter!

	bool retVal = true;
	if (!wasError){
		CTcpMessageHeader header;
		header.size = quint32(archiveImplPtr->GetBufferSize());
		if (header.size > 0){
			header.command = CTcpMessageHeader::CC_REQUEST_COMMAND;

			m_socket.write((char*)&header, sizeof(header));
			m_socket.write((char*)archiveImplPtr->GetBuffer(), header.size);
			m_socket.flush();
		}
	}

	delete archiveImplPtr;

	return retVal;
}


// reimplemented (icomm::IRequestsManager)

bool CTcpRequestsManagerComp::IsRequestsManagerActive()
{
	return IsHostConnected();
}


void CTcpRequestsManagerComp::DispatchMessagesQueue() const
{
	QCoreApplication::processEvents(QEventLoop::ExcludeUserInputEvents);
}


// reimplemented (icomm::TPopCumulatorBase)

int CTcpRequestsManagerComp::ReadToBuffer(quint8* bufferPtr, int bytes)
{
	return int(m_socket.read((char*)bufferPtr, bytes));
}


quint32 CTcpRequestsManagerComp::CalculateMessageSize(const CTcpMessageHeader& header)
{
	if (header.size < 1000000){
		return header.size;
	}
	else{
		SendErrorMessage(MI_WRONG_SIZE, QObject::tr("Wrong size of data package! Connection will be closed!"));

		m_socket.disconnectFromHost();

		return 0;	// TODO: Add error to log, if the size of message is not correct
	}
}


bool CTcpRequestsManagerComp::ProcessMessage(const CTcpMessageHeader& header, const quint8* dataPtr, int length)
{
	iser::CMemoryReadArchive request(dataPtr, length);

	if (header.command == CTcpMessageHeader::CC_VERSION_COMMAND){
		m_commonVersionInfo.MergeVersions(request.GetVersionInfo());
	}
	else if (header.command == CTcpMessageHeader::CC_RESPONSE_COMMAND){
		return BaseClass2::OnReceived(request);
	}

	return false;
}


// reimplemented (icomm::IHostConnection)

QString CTcpRequestsManagerComp::GetHostUrl() const
{
	return m_socket.peerAddress().toString();
}


bool CTcpRequestsManagerComp::ConnectToHost(const iprm::IParamsSet* paramsPtr)
{
	iprm::TParamsPtr<ifile::IFileNameParam> urlParamsPtr(paramsPtr, m_hostUrlParamIdAttrPtr, m_defaultHostUrlParamCompPtr, true);
	if (urlParamsPtr.IsValid()){
		QUrl hostUrl(urlParamsPtr->GetPath());

		if (hostUrl.isValid()){
			m_socket.connectToHost(hostUrl.host(), hostUrl.port(*m_defaultHostPortAttrPtr));

			return true;
		}
	}

	return false;
}


void CTcpRequestsManagerComp::DisconnectFromHost()
{
	m_socket.disconnectFromHost();
}


bool CTcpRequestsManagerComp::IsHostConnected() const
{
	return m_isConnected;
}


bool CTcpRequestsManagerComp::WaitHostConnected(double maxTime, bool disconnectOnTimeout)
{
	QElapsedTimer timer;
	do{
		if (m_isConnected){
			return true;
		}
		DispatchMessagesQueue();
	} while (!timer.hasExpired(quint64(maxTime * 1000)));

	if (disconnectOnTimeout){
		m_socket.disconnectFromHost();
	}

	return false;
}


bool CTcpRequestsManagerComp::RegisterEventHandler(Handler* handlerPtr, bool /*allowOtherThread*/)
{
	m_connectionHandlers.insert(handlerPtr);

	return true;
}


bool CTcpRequestsManagerComp::UnregisterEventHandler(Handler* handlerPtr)
{
	return m_connectionHandlers.remove(handlerPtr);
}


// public slots

void CTcpRequestsManagerComp::OnReadyRead()
{
	BaseClass3::ForceRead();
}


void CTcpRequestsManagerComp::OnSocketConnected()
{
	m_isConnected = true;

	// send empty message with header containing version information
	iser::CMemoryWriteArchive sendArchive(m_versionInfoCompPtr.GetPtr(), true);
	CTcpMessageHeader header;
	header.size = quint32(sendArchive.GetBufferSize());
	if (header.size > 0){
		header.command = CTcpMessageHeader::CC_VERSION_COMMAND;

		m_socket.write((char*)&header, sizeof(header));
		m_socket.write((char*)sendArchive.GetBuffer(), header.size);
		m_socket.flush();
	}

	for (		ConnectionHandlers::ConstIterator handlerIter = m_connectionHandlers.constBegin();
				handlerIter != m_connectionHandlers.constEnd();
				++handlerIter){
		IHostConnection::Handler* handlerPtr = *handlerIter;
		Q_ASSERT(handlerPtr != NULL);

		handlerPtr->OnAttached(*this);
	}
}


void CTcpRequestsManagerComp::OnSocketDisconnected()
{
	m_isConnected = false;

	for (		ConnectionHandlers::ConstIterator handlerIter = m_connectionHandlers.constBegin();
				handlerIter != m_connectionHandlers.constEnd();
				++handlerIter){
		IHostConnection::Handler* handlerPtr = *handlerIter;
		Q_ASSERT(handlerPtr != NULL);

		handlerPtr->OnDetached(*this);
	}
}


void CTcpRequestsManagerComp::OnSocketError(QAbstractSocket::SocketError /*socketError*/)
{
	for (		ConnectionHandlers::ConstIterator handlerIter = m_connectionHandlers.constBegin();
				handlerIter != m_connectionHandlers.constEnd();
				++handlerIter){
		Handler* handlerPtr = *handlerIter;
		Q_ASSERT(handlerPtr != NULL);

		handlerPtr->OnError(*this, m_socket.errorString());
	}
}


// protected methods

// reimplemented (icomp::CComponentBase)

void CTcpRequestsManagerComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	if (*m_autoConnectAttrPtr){
		ConnectToHost(NULL);
	}

	m_commonVersionInfo.SetMasterVersionInfo(m_versionInfoCompPtr.GetPtr());
}


} // namespace icomm



