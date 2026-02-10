// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#pragma once


// Qt includes
#include <QtNetwork/QAbstractSocket>

// ACF includes
#include <iprm/IParamsSet.h>


namespace icomm
{


/**
	Host connection interface.
	Base function for connection oriented communication,
	where one of side is waiting passive for connection and the second one initialise it.
	Objects implementing this interface supports active communication side.
*/
class IHostConnection: virtual public istd::IChangeable
{
public:
	/** Interface for event handler.
	*/
	class Handler: virtual public istd::IPolymorphic
	{
	public:
		/**
			Called if connected.
		*/
		virtual void OnAttached(icomm::IHostConnection& connection) = 0;
		/**
			Called if disconnected.
		*/
		virtual void OnDetached(icomm::IHostConnection& connection) = 0;
		/**
			Called on error.
		*/
		virtual void OnError(icomm::IHostConnection& connection, QString errorMessage) = 0;
		/**
			Called on socket state change (see QAbstractSocket::SocketState).
		*/
		virtual void OnStateChanged(icomm::IHostConnection& connection, QAbstractSocket::SocketState newState) = 0;
	};

	virtual QString GetHostUrl() const = 0;

	/**
		Try to connect to remote part.
		\param	paramsPtr	describe connections parameters
		\return	true, if success.
	*/
	virtual bool ConnectToHost(const iprm::IParamsSet* paramsPtr = NULL) = 0;

	/**
		Disconnect connection to remote part.
	*/
	virtual void DisconnectFromHost() = 0;

	/**
		Checks if connected to host.
	*/
	virtual bool IsHostConnected() const = 0;

	/**
		Waits until host is connected.
		\param	maxTime				maximal time, will be waited
		\param	disconnectOnTimeout	force to disconnection (canceling connection process),
									if time out occured.
		\return	true, if connection was established.
	*/
	virtual bool WaitHostConnected(double maxTime, bool disconnectOnTimeout = true) = 0;

	/**
		Connect event handler.
		\param	handlerPtr			Pointer to user events handler.
									This events will be called in user specificated thread.
		\param	allowOtherThread	if true, it allows calls of handler in other than current caller thread.
	*/
	virtual bool RegisterEventHandler(Handler* handlerPtr, bool allowOtherThread = false) = 0;

	/**
		Disconnect handler connected with \c RegisterEventHandler.
		\param	handlerPtr	Pointer to user events handler.
	*/
	virtual bool UnregisterEventHandler(Handler* handlerPtr) = 0;
};


} // namespace icomm


