// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#pragma once


// ACF includes
#include <istd/IChangeable.h>


namespace iweb
{


/**
	Interface for the web-server status description.
*/
class IWebServerStatus: virtual public istd::IChangeable
{
public:
	/**
		Server status.
	*/
	enum ServerStatus
	{
		/**
			Status is not available.
		*/
		SS_NONE,

		/**
			Server is currently in the initialization stage. The requests cannot be processed.
		*/
		SS_STARTING,

		/**
			Server is started. All requests can be processed
		*/
		SS_READY,

		/**
			Server is started and processing some maintenance tasks currently.
			It is possible, that some requests cannot be processed.
		*/
		SS_MAINTENANCE
	};

	I_DECLARE_ENUM(ServerStatus, SS_NONE, SS_STARTING, SS_READY, SS_MAINTENANCE);

	/**
		Get the current server status.
	*/
	virtual ServerStatus GetServerStatus() const = 0;

	/**
		Set current server status.
	*/
	virtual void SetServerStatus(ServerStatus status) = 0;
};


} // namespace iweb


