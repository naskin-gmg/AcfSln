// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#pragma once


// ACF includes
#include <istd/IPolymorphic.h>


namespace icomm
{


/**
	Describes actual state of station.
*/
class IStationState: virtual public istd::IPolymorphic
{
public:
	enum State
	{
		S_UNKNOWN,

		/**
			Station is initialized, but it is not ready.
		*/
		S_INIT,
		
		/**
			Station is ready, no user is logged.
		*/
		S_READY,
		
		/**
			Station is ready and user is logged.
		*/
		S_LOGGED
	};

	/**
		Gets station state.
	*/
	virtual State GetStationState() const = 0;
	
	/**
		Gets name of logged user.
	*/
	virtual const QString& GetLoggedUser() const = 0;
	
	/**
		Gets computer name of logged user.
	*/
	virtual const QString& GetLoggedComputer() const = 0;
	
	/**
		Gets IP address of logged user.
	*/
	virtual const QByteArray& GetLoggedIp() const = 0;
};


} // namespace icomm


