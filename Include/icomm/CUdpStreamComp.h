// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#pragma once


// Qt includes
#include <QtNetwork/QUdpSocket>
#include <QtCore/QMutex>
#include <QtCore/QThread>

// ACF includes
#include <iprm/IParamsSet.h>
#include <imod/TSingleModelObserverBase.h>
#include <ilog/TLoggerCompWrap.h>
#include <icomm/IBinaryStream.h>


namespace icomm
{


class CUdpStreamComp: 
			public QObject,
			public ilog::CLoggerComponentBase,
			public imod::TSingleModelObserverBase<iprm::IParamsSet>,
			public icomm::IBinaryStream
{
	Q_OBJECT

public:
	typedef ilog::CLoggerComponentBase BaseClass;
	typedef imod::TSingleModelObserverBase<iprm::IParamsSet> BaseClass2;

	I_BEGIN_COMPONENT(CUdpStreamComp);
		I_REGISTER_INTERFACE(icomm::IBinaryStream);
	I_END_COMPONENT;

	CUdpStreamComp();

	// returns last packet time
	QDateTime GetPacketTime() const { return m_readThread.m_lastDateTime; }

	// reimplemented (icomm::IBinaryStream)
	virtual bool SendData(const void* data, int size, void* userContextPtr = NULL) override;
	virtual bool ReadData(void* data, int& size) override;
	virtual bool RegisterEventHandler(icomm::IBinaryStream::Handler* handlerPtr, bool /*allowOtherThread = false*/) override;
	virtual bool UnregisterEventHandler(icomm::IBinaryStream::Handler* handlerPtr) override;

protected:
	// reimplemented (imod::TSingleModelObserverBase)
	virtual void OnUpdate(const istd::IChangeable::ChangeSet& changeSet);

	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentDestroyed() override;

private:
	class ReadThread: public QThread
	{
	public:
		friend class CUdpStreamComp;

		ReadThread(CUdpStreamComp* parent);

		bool Connect(const QHostAddress &address, quint16 port = 0); 
		void Disconnect();

		// reimplemented (icomm::IBinaryStream)
		virtual bool ReadData(void* data, int& size);
		virtual bool RegisterEventHandler(icomm::IBinaryStream::Handler* handlerPtr, bool /*allowOtherThread = false*/);
		virtual bool UnregisterEventHandler(icomm::IBinaryStream::Handler* handlerPtr);

		// reimplemented (QThread)
		virtual void run() override;

	protected:
#if QT_VERSION >= 0x060000
		mutable QRecursiveMutex m_mutex;
#else
		mutable QMutex m_mutex;
#endif

		bool m_stopThread;

		CUdpStreamComp* m_parent;

		QUdpSocket m_socket;

		icomm::IBinaryStream::Handler* m_handlerPtr;

		QByteArray m_lastData;
		QDateTime m_lastDateTime;
	};

	ReadThread m_readThread;
};


} // namespace icomm


