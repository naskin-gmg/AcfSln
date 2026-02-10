// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#pragma once


// Qt includes
#include <QtCore/QVector>


#include <icomm/IServersManager.h>
#include <icomm/CHeartbeatAnswer.h>


namespace icomm
{


class IHeartbeatServersManager: public icomm::IServersManager
{
public:
	typedef QVector<icomm::CHeartbeatAnswer> FoundServers;

	/**
		Starts search function to find all accessible servers.
		\param	result	list of found servers.
	*/
	virtual bool FindServers(FoundServers& result) = 0;

	/**
		Gets CHeatbeatAnswer object.
		\param	index	the same index like in icomm::IServersManager::GetServerInfo method.
						\sa icomm::IServersManager
	*/
	virtual CHeartbeatAnswer& GetServerInfoImpl(int index) const = 0;

	/**
		Creates new server info object.
		\param	index		position in servers list.
							Server will be added on the end if it's negative.
		\param	copyFromIndex	If it is positive value, specified info object
								will be used as template.
		\return	index of server, or negative value if error.
	*/
	virtual int CreateServerInfo(int index = -1, int copyFromIndex = -1) = 0;

	/**
		Removes specified server info object.
	*/
	virtual bool RemoveServerInfo(int index) = 0;
};


} // namespace icomm


