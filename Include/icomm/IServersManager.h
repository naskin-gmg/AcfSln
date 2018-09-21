#pragma once


// ACF includes
#include <iser/ISerializable.h>
#include <icomm/IServerInfo.h>


namespace icomm
{


/**
	Server manager on the client side.
*/
class IServersManager: virtual public iser::ISerializable
{
public:
	/**
		Gets number of servers data stored in this manager.
	*/
	virtual int GetServersCount() const = 0;

	/**
		Gets server information for specified server.
	*/
	virtual const IServerInfo& GetServerInfo(int index) const = 0;
};


} // namespace icomm


