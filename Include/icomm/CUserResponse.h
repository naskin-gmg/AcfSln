#pragma once


// Qt includes
#include <QtCore/QByteArray>

// ACF includes
#include <iser/IArchive.h>
#include <iser/ISerializable.h>
#include <icomm/IResponse.h>


namespace icomm
{


class IResponsesManager;


/**
	This is simple implementation of icomm::IResponse interface.
	This implementation serialize slave object and is associated with icomm::CUserRequest class.
	Note: On both sides the same object type als slave must be connected!
	You can use it as base class for your own response implementation.
*/
class CUserResponse: public IResponse
{
public:
	/**
		Contructs response object with specified command and slave object.
		\param	command		command string. It is used to identify the request type on server side.
		\param	slavePtr	slave object will be serializated using request data.
							It cannot be NULL.
	*/
	CUserResponse(const QByteArray& command, iser::ISerializable* slavePtr);

	// reimplemented (icomm::IResponse)
	virtual QByteArray GetCommand() const override;
	virtual bool DoResponse(iser::IArchive& request, iser::IArchive& response) override;
	virtual bool OnAttached(IResponsesManager* managerPtr) override;
	virtual bool OnDetached(IResponsesManager* managerPtr) override;

private:
	QByteArray m_command;
	iser::ISerializable* m_slavePtr;
	IResponsesManager* m_managerPtr;
};


} // namespace icomm


