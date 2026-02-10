// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#pragma once


// Qt includes
#include <QtCore/QByteArray>

// ACF includes
#include <istd/IPolymorphic.h>


namespace icomm
{


/**
	Informations about connected client application.
*/
class IClientStatus: virtual public istd::IPolymorphic
{
public:
	/**
		Name of client station.
	*/
	virtual const QString& GetClientComputerName() const = 0;
	/**
		Network identification of client station.
	*/
	virtual const QByteArray& GetClientComputerAddress() const = 0;
	/**
		Provide access to client version informations.
	*/
	virtual const iser::IVersionInfo& GetClientVersionInfo() const = 0;
};


} // namespace icomm


