#pragma once


// Qt includes
#include <QtCore/QByteArray>

// ACF includes
#include <istd/IPolymorphic.h>
#include <iser/IArchive.h>


namespace icomm
{


class IResponsesManager;


/**
	This is common interface for all response objects.
	Response objects you can connect to response container,
	to provide response for associated request.
	\sa icomm::IRequest and icomm::IResponseContainer.
*/
class IResponse: virtual public istd::IPolymorphic
{
public:
	/**
		Identify the command which is provided through this response provider.
		All requests with this command will be serialized using this object.
	*/
	virtual QByteArray GetCommand() const = 0;

	/**
		Get response for specified input stream.
	*/
	virtual bool DoResponse(iser::IArchive& request, iser::IArchive& response) = 0;

	//	internal methods
	/**
		Internal method used to synchronize connection between IResponse and IResponsesManager.
		It called after manager method ConnectProvider is called.
		\sa	icomm::IResponsesManager.
		\return	true, if connection was successfull.
	*/
	virtual bool OnAttached(IResponsesManager* managerPtr) = 0;
	/**
		Internal method used to synchronize connection between IResponse and IResponsesManager.
		It called after manager method DisconnectProvider is called.
		\sa	icomm::IResponsesManager.
		\return	true, if disconnection was successfull.
	*/
	virtual bool OnDetached(IResponsesManager* managerPtr) = 0;
};


} // namespace icomm


