// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
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


