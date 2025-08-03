#pragma once


// Qt includes
#include <QtCore/QObject>
#include <QtCore/QSet>
#include <QtNetwork/QTcpSocket>

// ACF includes
#include <iser/CArchiveHeaderInfo.h>
#include <ilog/TLoggerCompWrap.h>
#include <ifile/IFileNameParam.h>
#include <icomm/IHostConnection.h>
#include <icomm/TPopCumulatorBase.h>
#include <icomm/CRequestsManagerBase.h>
#include <icomm/CTcpMessageHeader.h>
#include <icomm/CCommonVersionInfo.h>


namespace icomm
{


class CTcpRequestsManagerComp:
			public QObject,
			public ilog::CLoggerComponentBase,
			public icomm::IHostConnection,
			public icomm::TPopCumulatorBase<CTcpMessageHeader>,
			public icomm::CRequestsManagerBase
{
	Q_OBJECT
public:
	typedef ilog::CLoggerComponentBase BaseClass;
	typedef icomm::CRequestsManagerBase BaseClass2;
	typedef icomm::TPopCumulatorBase<CTcpMessageHeader> BaseClass3;

	enum MessageId
	{
		MI_WRONG_SIZE = 0x28335
	};

	I_BEGIN_COMPONENT(CTcpRequestsManagerComp);
		I_REGISTER_INTERFACE(icomm::IHostConnection);
		I_REGISTER_INTERFACE(icomm::IRequestsManager);
		I_ASSIGN(m_autoConnectAttrPtr, "AutoConnect", "Force connection to host during component initialization, default parameters will be used", true, false);
		I_ASSIGN(m_defaultHostPortAttrPtr, "DefaultHostPort", "Port number of host if not specified in URL", true, 8989);
		I_ASSIGN(m_defaultHostUrlParamCompPtr, "DefaultHostUrlParam", "Default host URL parameter", false, "DefaultHostUrlParam");
		I_ASSIGN(m_hostUrlParamIdAttrPtr, "HostUrlParamId", "ID of host URL in parameter set", true, "HostUrlParamId");
		I_ASSIGN(m_versionInfoCompPtr, "VersionInfo", "Version information of the client application", false, "VersionInfo");
	I_END_COMPONENT;

	CTcpRequestsManagerComp();

	// reimplemented (icomm::CRequestsManagerBase)
	virtual iser::IArchive* CreateSendArchive();
	virtual bool RemoveSendArchive(iser::IArchive& archive, bool wasError = false);

	// reimplemented (icomm::IRequestsManager)
	virtual bool IsRequestsManagerActive();
	virtual void DispatchMessagesQueue() const;

	// reimplemented (icomm::TPopCumulatorBase)
	virtual int ReadToBuffer(quint8* bufferPtr, int bytes);
	virtual quint32 CalculateMessageSize(const CTcpMessageHeader& header);
	virtual bool ProcessMessage(const CTcpMessageHeader& header, const quint8* dataPtr, int length);

	// reimplemented (icomm::IHostConnection)
	virtual QString GetHostUrl() const;
	virtual bool ConnectToHost(const iprm::IParamsSet* paramsPtr);
	virtual void DisconnectFromHost();
	virtual bool IsHostConnected() const;
	virtual bool WaitHostConnected(double maxTime, bool disconnectOnTimeout = true);
	virtual bool RegisterEventHandler(Handler* handlerPtr, bool allowOtherThread = false);
	virtual bool UnregisterEventHandler(Handler* handlerPtr);

public Q_SLOTS:
	void OnReadyRead();
	void OnSocketConnected();
	void OnSocketDisconnected();
	void OnSocketError(QAbstractSocket::SocketError socketError);

protected:
	enum CallCode
	{
		CC_ON_CONNECTED,
		CC_ON_DISCONNECTED,
		CC_ON_ERROR
	};

	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated() override;

private:
	I_ATTR(bool, m_autoConnectAttrPtr);
	I_ATTR(int, m_defaultHostPortAttrPtr);
	I_REF(ifile::IFileNameParam, m_defaultHostUrlParamCompPtr);
	I_ATTR(QByteArray, m_hostUrlParamIdAttrPtr);
	I_REF(iser::IVersionInfo, m_versionInfoCompPtr);

	QTcpSocket m_socket;

	typedef QSet<IHostConnection::Handler*> ConnectionHandlers;
	ConnectionHandlers m_connectionHandlers;

	bool m_isConnected;

	CCommonVersionInfo m_commonVersionInfo;
	iser::CArchiveHeaderInfo m_serverVersionInfo;
};


} // namespace icomm


