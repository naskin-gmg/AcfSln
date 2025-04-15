#pragma once


// ACF includes
#include <iser/ISerializable.h>


namespace icomm
{


/**
	Request header for request/response mechanism.
	This object is send from request manager to response manager.
	It identify request and store information needed to send back correct answer.
	\sa icomm::IRequestsManager, icomm::IResponseManager and icomm::CResponseHeader.
*/
class CRequestHeader: virtual public iser::ISerializable
{
public:
	/**
		Construct header without specifying attributes.
	*/
	CRequestHeader();

	/**
		Construct header and specify command, container and request-ID.
	*/
	CRequestHeader(const QByteArray& command, const QByteArray& requestId = QByteArray());

	/**
		Get command for request.
	*/
	const QByteArray& GetCommand() const;

	/**
		Set command for request.
	*/
	void SetCommand(const QByteArray& command);

	/**
		Get ID of request inside request container.
	*/
	QByteArray GetRequestId() const;

	/**
		Set ID of request inside request container.
	*/
	void SetRequestId(const QByteArray& requestId);

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive) override;

private:
	QByteArray m_command;
	QByteArray m_requestId;
};


// inline methods

inline const QByteArray& CRequestHeader::GetCommand() const
{
	return m_command;
}


inline void CRequestHeader::SetCommand(const QByteArray& command)
{
	m_command = command;
}


inline QByteArray CRequestHeader::GetRequestId() const
{
	return m_requestId;
}


inline void CRequestHeader::SetRequestId(const QByteArray& id)
{
	m_requestId = id;
}


} // namespace icomm


