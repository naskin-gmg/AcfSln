#pragma once


// ACF includes
#include <icomm/IRequest.h>


namespace icomm
{


/**
	This is simple implementation of icomm::IRequest interface.
	This implementation serialize slave object and is associated with icomm::CUserResponse class.
	Note: On both sides the same object type als slave must be connected!
	You can use it as base class for your own requsts implementation.
*/
class CUserRequest: public IRequest
{
public:
	/**
		Contructs request object with specified command and slave object.
		\param	command		command string. It is used to identify the request type on server side.
		\param	slavePtr	slave object will be serializated using request data.
							It cannot be NULL.
		\param	timeout		timeout of this request.
	*/
	CUserRequest(const QByteArray& command, ISerializable* slavePtr, double timeout = 3);

	// reimplemented (icomm::IRequest)
	virtual double GetTimeOut() const;
	virtual QByteArray GetCommand() const;
	virtual QString ExtractMessage(const CResponseHeader& header) const;

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);

private:
	QByteArray m_command;
	double m_timeOut;
	ISerializable* m_slavePtr;
};


} // namespace icomm


