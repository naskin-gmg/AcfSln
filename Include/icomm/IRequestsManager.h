#pragma once


// Qt includes
#include <QtCore/QString>
#include <QtCore/QVector>

// ACF includes
#include <istd/IPolymorphic.h>


namespace icomm
{


class IRequest;
class IRequestsManager;


/**
	Requests container stores requests object and manage it.
	You can add request to container and wait to request is realised.
*/
class IRequestsManager: virtual public istd::IPolymorphic
{
public:
	enum RequestState
	{
		/**
			Request is not processed, its state is unknown.
		*/
		RS_NONE,
		/**
			Request is not processed yet.
		*/
		RS_WAIT,
		/**
			Request is processed without errors.
		*/
		RS_OK,
		/**
			Request is processed but there are errors and state of destination object is not changed.
		*/
		RS_FAILED,
		/**
			Request is processed but there are errors during serialization and state of destination can be damaged.
		*/
		RS_DAMAGED
	};

	/**
		Information about request state.
	*/
	struct RequestInfo
	{
		RequestState state;
		QString errorMessage;
	};
	typedef QVector<RequestInfo> RequestsInfos;

	/**
		Check, if this manager is active, it means can process the commands.
	*/
	virtual bool IsRequestsManagerActive() = 0;

	/**
		Add request to container.
		Container is the owner of added request and delete it after request is executed.
		\param			Request	a pointer to request object. It cannot be NULL.
		\return	id		Unique ID of this request or empty value if error.
	*/
	virtual QByteArray AddRequest(IRequest* requestPtr) = 0;

	/**
		Wait for single request.
		You have to specify id of this request in RequestState structure.
		\param		requestId		ID of request returned by AddRequest or -1.
		\param		killOnTimeout	if it is true, task will be killed when time out occured.
		\param		requestInfoPtr	structure where is stored additional
									information about processing of request.
		\return		true, if there are no errors.
	*/
	virtual RequestState WaitRequestFinished(
				const QByteArray& requestId = QByteArray(),
				bool killOnTimeout = true,
				RequestInfo* requestInfoPtr = NULL) const = 0;

	/**
		Force container to send all waiting requests.
		It can be usefull for implementations they cache the requests.
	*/
	virtual void Flush() = 0;

	/**
		Start dispatching of messages queue.
	*/
	virtual void DispatchMessagesQueue() const = 0;
};


} // namespace icomm


