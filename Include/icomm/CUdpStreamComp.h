#pragma once


// Qt includes
#include <QtNetwork/QUdpSocket>
#include <QtCore/QMutexLocker>
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
	virtual bool SendData(const void* data, int size, void* userContextPtr = NULL);
	virtual bool ReadData(void* data, int& size);
	virtual bool RegisterEventHandler(icomm::IBinaryStream::Handler* handlerPtr, bool /*allowOtherThread = false*/);
	virtual bool UnregisterEventHandler(icomm::IBinaryStream::Handler* handlerPtr);

protected:
	// reimplemented (imod::TSingleModelObserverBase)
	virtual void OnUpdate(const istd::IChangeable::ChangeSet& changeSet);

	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentDestroyed();

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
		virtual void run();

	protected:
		QRecursiveMutex m_lock;

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


