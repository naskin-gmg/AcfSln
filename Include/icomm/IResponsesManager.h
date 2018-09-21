#pragma once


// ACF includes
#include <istd/IPolymorphic.h>


namespace icomm
{


class IResponse;


/**
	Server side component, which provide managment of answer to client requests.
	\sa icomm::IRequestManager, icomm::IResponse
*/
class IResponsesManager: virtual public istd::IPolymorphic
{
public:
	/**
		Connect response provider to manager.
		\param	providerPtr	a pointer to connected response provider. It cannot be NULL.
	*/
	virtual void ConnectProvider(IResponse* providerPtr) = 0;

	/**
		Disconnect response provider from manager.
		\param	providerPtr	a pointer to disconnected response provider.
		Disconnected response provider must be previous connected to this manager and this pointer cannot be NULL.
	*/
	virtual void DisconnectProvider(IResponse* providerPtr) = 0;
};


} // namespace icomm


