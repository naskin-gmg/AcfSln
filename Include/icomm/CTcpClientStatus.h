#pragma once


// Qt includes
#include <QtCore/QObject>
#include <QtNetwork/QTcpSocket>

// ACF includes
#include <iser/CArchiveHeaderInfo.h>
#include <icomp/IComponent.h>
#include <ibase/IApplicationInfo.h>
#include <icomm/IClientStatus.h>
#include <icomm/TPopCumulatorBase.h>
#include <icomm/CResponsesManagerBase.h>
#include <icomm/CTcpMessageHeader.h>
#include <icomm/CCommonVersionInfo.h>


namespace icomm
{


/**
	Data definition for header.
*/
/**
	Representation of TCP client on server side.
*/
class CTcpClientStatus:
			public QObject,
			public icomm::CResponsesManagerBase,
			public icomm::TPopCumulatorBase<CTcpMessageHeader>,
			virtual public icomm::IClientStatus
{
    Q_OBJECT

public:
	typedef icomm::CResponsesManagerBase BaseClass;
	typedef icomm::TPopCumulatorBase<CTcpMessageHeader> BaseClass2;

	/**	Construct client representation using associated socket.
	 *		\param	socketPtr	pointer to socket object.
	 *							It cannot be NULL.
	*/
	CTcpClientStatus(
				QTcpSocket* socketPtr,
				const ibase::IApplicationInfo* applicationInfoPtr);

	// reimplemented (icomm::TPopCumulatorBase)
	virtual int ReadToBuffer(quint8* bufferPtr, int bytes);
	virtual quint32 CalculateMessageSize(const CTcpMessageHeader& header);
	virtual bool ProcessMessage(const CTcpMessageHeader& header, const quint8* dataPtr, int length);

	// reimplemented (icomm::IClientStatus)
	virtual const QString& GetClientComputerName() const;
	virtual const QByteArray& GetClientComputerAddress() const;
	virtual const iser::IVersionInfo& GetClientVersionInfo() const;

public Q_SLOTS:
	void OnReadyRead();

protected:
	// reimplemented (icomm::CResponsesManagerBase)
	virtual bool SendAndClear(iser::IArchive& answer);

private:
	QTcpSocket& m_socket;

	const ibase::IApplicationInfo& m_applicationInfo;

	QString m_computerName;
	QByteArray m_ipAddress;

	CCommonVersionInfo m_commonVersionInfo;
	iser::CArchiveHeaderInfo m_clientVersionInfo;
};


}// namespace icomm


