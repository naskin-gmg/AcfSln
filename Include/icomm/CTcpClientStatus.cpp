#include <icomm/CTcpClientStatus.h>


// ACF includes
#include <iser/CMemoryReadArchive.h>
#include <iser/CMemoryWriteArchive.h>


namespace icomm
{


CTcpClientStatus::CTcpClientStatus(
				QTcpSocket* socketPtr,
				const ibase::IApplicationInfo* applicationInfoPtr)
:	m_socket(*socketPtr),
	m_applicationInfo(*applicationInfoPtr)
{
	Q_ASSERT(socketPtr != NULL);
	Q_ASSERT(applicationInfoPtr != NULL);

	connect(&m_socket, SIGNAL(disconnected()), this, SLOT(deleteLater()));
	connect(&m_socket, SIGNAL(readyRead()), this, SLOT(OnReadyRead()));

	m_commonVersionInfo.SetMasterVersionInfo(&m_applicationInfo.GetVersionInfo());
}


// reimplemented (icomm::IClientStatus)

const QString& CTcpClientStatus::GetClientComputerName() const
{
	return m_computerName;
}


const QByteArray& CTcpClientStatus::GetClientComputerAddress() const
{
	return m_ipAddress;
}


const iser::IVersionInfo& CTcpClientStatus::GetClientVersionInfo() const
{
	return m_clientVersionInfo;
}


// reimplemented (icomm::TPopCumulatorBase)

int CTcpClientStatus::ReadToBuffer(quint8* bufferPtr, int bytes)
{
	return int(m_socket.read((char*)bufferPtr, bytes));
}


quint32 CTcpClientStatus::CalculateMessageSize(const CTcpMessageHeader& header)
{
	if (header.size < 1000000){
		return header.size;
	}
	else{
		return 0;
	}
}


bool CTcpClientStatus::ProcessMessage(const CTcpMessageHeader& header, const quint8* dataPtr, int length)
{
	iser::CMemoryReadArchive request(dataPtr, length);

	if (header.command == CTcpMessageHeader::CC_VERSION_COMMAND){
		const iser::IVersionInfo& clientVersionInfo = request.GetVersionInfo();
		m_clientVersionInfo.CopyFrom(clientVersionInfo);
		m_commonVersionInfo.MergeVersions(clientVersionInfo);

		// send empty message with header containing version information
		const iser::IVersionInfo& applicationVersionInfo = m_applicationInfo.GetVersionInfo();
		iser::CMemoryWriteArchive sendArchive(&applicationVersionInfo, true);
		CTcpMessageHeader headerVersion;
		headerVersion.size = quint32(sendArchive.GetBufferSize());
		if (header.size > 0){
			headerVersion.command = CTcpMessageHeader::CC_VERSION_COMMAND;

			m_socket.write((char*)&headerVersion, sizeof(headerVersion));
			m_socket.write((char*)sendArchive.GetBuffer(), headerVersion.size);
			m_socket.flush();
		}

		return true;
	}
	else if (header.command == CTcpMessageHeader::CC_REQUEST_COMMAND){
		iser::CMemoryWriteArchive response(&m_commonVersionInfo);

		return BaseClass::OnReceived(request, response);
	}

	return false;
}


// public slots

void CTcpClientStatus::OnReadyRead()
{
	BaseClass2::ForceRead();
}


// protected methods

// reimplemented (icomm::CResponsesManagerBase)

bool CTcpClientStatus::SendAndClear(iser::IArchive& answer)
{
	bool retVal = false;
	iser::CMemoryWriteArchive* answerImplPtr = dynamic_cast<iser::CMemoryWriteArchive*>(&answer);
	if (answerImplPtr != NULL){
		CTcpMessageHeader header;
		header.size = answerImplPtr->GetBufferSize();
		if (header.size > 0){
			header.command = CTcpMessageHeader::CC_RESPONSE_COMMAND;

			m_socket.write((char*)&header, sizeof(header));
			m_socket.write((char*)answerImplPtr->GetBuffer(), header.size);
		}

		answerImplPtr->Reset();
		retVal = true;
	}

	return retVal;
}


}// namespace icomm

