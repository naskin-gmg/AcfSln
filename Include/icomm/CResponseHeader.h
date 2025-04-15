#pragma once


// Qt includes
#include <QtCore/QString>

// ACF includes
#include <iser/ISerializable.h>


namespace icomm
{


class CRequestHeader;


/**
	Container used to store header of request message.
	This object is send from response manager to request manager.
	It identify response and store information needed to route the request to corrected object.
	\sa icomm::IRequestsManager, icomm::IResponseManager and icomm::CRequestHeader.
*/
class CResponseHeader: virtual public iser::ISerializable
{
public:
	enum ResponseFlags
	{
		/**
			Invalid value, it is not a flag and can't be compared using & operator.
			Use to initialize only.
		*/
		RF_INVALID = 0,

		/**
			Signalize that error is occured.
		*/
		RF_ERROR = 1,

		/**
			Signalize, that in archive answer sequence is serialized.
		*/
		RF_SERIALIZED = 2,

		/**
			Signalize, that request should wait for a next answer part.
			Wait time out count will be reset after this. 
		*/
		RF_KEEP_ALIVE = 4,
	};

	enum ErrorType
	{
		/**
			There is no error.
		*/
		ET_NONE,

		/**
			Server operation can't be done.
		*/
		ET_OPERATION,

		/**
			There are problems to get the request.
		*/
		ET_REQUEST_COMM,

		/**
			There are problems to get the response.
		*/
		ET_RESPONSE_COMM,

		/**
			First user code.
		*/
		ET_USER = 1024
	};

	CResponseHeader();

	/**
		Set state of this object to ok answer for specified request.
		\param	request			request object used for correct routing.
	*/
	void SetOk(const CRequestHeader& request);

	/**
		Set state of this object to error message with text message.
		\param	request			request object used for correct routing.
		\param	errorMessage	text of error message, will be sent to requester.
		\param	serialized		should be set, if response object was stored.
	*/
	void SetError(ErrorType errorType, const CRequestHeader& request, const QString& errorMessage, bool serialized);

	/**
		Set state of this object to 'keep alive' message for specified request.
		This message avoid to remove request from request manager due of time out.
		\param	request			request object used for correct routing.
	*/
	void SetToKeepAliveMessage(const CRequestHeader& request);

	/**
		Get request ID in request container.
	*/
	QByteArray GetRequestId() const;

	/**
		Set request ID in request container.
	*/
	void SetRequestId(const QByteArray& id);

	/**
		Get flags of this response.
		Please note, that you have to read serialization object when RF_SERIALIZED is signaled.
	*/
	ResponseFlags GetResponseFlags() const;
	/**
		Get type of error.
		You can check if there is error occured by comparing this value with ET_NONE.
	*/
	ErrorType GetErrorType() const;
	/**
		Get error message.
		It is valid only if RF_ERROR is signaled.
	*/
	const QString& GetErrorMessage() const;

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive) override;

private:
	ResponseFlags m_flags;
	QString m_errorMessage;
	QByteArray m_requestId;
	ErrorType m_errorType;
};


inline QByteArray CResponseHeader::GetRequestId() const
{
	return m_requestId;
}


inline void CResponseHeader::SetRequestId(const QByteArray& id)
{
	m_requestId = id;
}


inline CResponseHeader::ResponseFlags CResponseHeader::GetResponseFlags() const
{
	return m_flags;
}


inline CResponseHeader::ErrorType CResponseHeader::GetErrorType() const
{
	return m_errorType;
}


inline const QString& CResponseHeader::GetErrorMessage() const
{
	return m_errorMessage;
}


} // namespace icomm


// public operators

inline ::icomm::CResponseHeader::ResponseFlags operator|(::icomm::CResponseHeader::ResponseFlags flag1, ::icomm::CResponseHeader::ResponseFlags flag2)
{
	return icomm::CResponseHeader::ResponseFlags(int(flag1) | int(flag2));
}


