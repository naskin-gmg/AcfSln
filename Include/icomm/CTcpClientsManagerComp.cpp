#include <icomm/CTcpClientsManagerComp.h>


// ACF includes
#include <iqt/iqt.h>
#include <iser/CMemoryReadArchive.h>
#include <iser/CMemoryWriteArchive.h>
#include <icomm/CHeartbeatQuestion.h>
#include <icomm/CHeartbeatAnswer.h>
#include <icomm/CTcpClientStatus.h>


namespace icomm
{


CTcpClientsManagerComp::CTcpClientsManagerComp()
{
	connect(&m_tcpServer, SIGNAL(newConnection()), this, SLOT(OnNewConnection()));
	connect(&m_heartbeatSocket, SIGNAL(readyRead()), this, SLOT(OnHeartbeatRead()));
}


// public slots

void CTcpClientsManagerComp::OnNewConnection()
{
	QTcpSocket* socketPtr = m_tcpServer.nextPendingConnection();
	if (			(socketPtr != NULL) &&
					m_responsesCompPtr.IsValid() &&
					m_applicationInfoCompPtr.IsValid()){
		CTcpClientStatus* statusPtr = new CTcpClientStatus(
						socketPtr,
						m_applicationInfoCompPtr.GetPtr());
		if (statusPtr != NULL){
			int responesCount = m_responsesCompPtr.GetCount();
			for (int i = 0; i < responesCount; ++i){
				icomm::IResponse* responsePtr = m_responsesCompPtr[i];
				if (responsePtr != NULL){
					statusPtr->ConnectProvider(responsePtr);
				}
			}
		}
	}
}


void CTcpClientsManagerComp::OnHeartbeatRead()
{
	while (m_heartbeatSocket.hasPendingDatagrams()){
		int datagramSize = m_heartbeatSocket.pendingDatagramSize();
		Q_ASSERT(datagramSize >= 0);

		QByteArray datagram;
		datagram.resize(datagramSize);
		QHostAddress senderAddress;
		quint16 senderPort;
		m_heartbeatSocket.readDatagram(
						datagram.data(), datagram.size(),
						&senderAddress, &senderPort);

		if (m_applicationInfoCompPtr.IsValid()){
			const QString& appName = m_applicationInfoCompPtr->GetApplicationAttribute(ibase::IApplicationInfo::AA_APPLICATION_NAME);

			iser::CMemoryReadArchive questionArchive(datagram.data(), datagram.size());

			icomm::CHeartbeatQuestion question;
			const QString& questionAppName = question.GetApplicationName();
			if (question.Serialize(questionArchive) && (questionAppName == appName)){
				quint32 appVersion;
				m_applicationInfoCompPtr->GetVersionInfo().GetVersionNumber(0, appVersion);

				icomm::CHeartbeatAnswer answer(question);
				answer.SetVersion(appVersion);
				answer.SetApplicationName(appName);
				answer.SetComputerName(QHostInfo::localHostName());
				//answer.SetName(m_stationInfoCompPtr->GetName());
				//answer.SetDescription(m_stationInfoCompPtr->GetDescription());

				if (m_stationStateCompPtr.IsValid()){
					answer.SetStationState(m_stationStateCompPtr->GetStationState());
				}
				else{
					answer.SetStationState(icomm::IStationState::S_UNKNOWN);
				}

				iser::CMemoryWriteArchive answerArchive(&m_applicationInfoCompPtr->GetVersionInfo());
				if (answer.Serialize(answerArchive)){
					m_heartbeatSocket.writeDatagram(
									(const char*)answerArchive.GetBuffer(),
									answerArchive.GetBufferSize(),
									senderAddress,
									senderPort);
				}
			}
		}
	}
}


// protected methods

// reimplemented (icomp::CComponentBase)

void CTcpClientsManagerComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	if (m_portNumberAttrPtr.IsValid()){
		m_tcpServer.listen(QHostAddress::Any, *m_portNumberAttrPtr);
	}

	if (m_heartbeatPortAttrPtr.IsValid()){
		m_heartbeatSocket.bind(*m_heartbeatPortAttrPtr);
	}
}


} // namespace icomm


