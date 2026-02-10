// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#pragma once


// Qt includes
#include <QtCore/QByteArray>
#include <QtCore/QString>

// ACF includes
#include <iser/ISerializable.h>


namespace icomm
{


class CResponseHeader;


/**
	Request object used in client-server communication.
	Request object can be added to request container (\sa icomm::IRequestContainer).
	Please note, that serialization has other meaning for reading and storing archive.
	For storing it is request serialization, for reading it is response serialization.
	Every request must specify its time out value and unique request command.
*/
class IRequest: virtual public iser::ISerializable
{
public:
	/**
		It describes how long this request would be stored in container.
		Additionally there is 'keep alive' mechanism.
	*/
	virtual double GetTimeOut() const = 0;
	/**
		Get string ID of request command.
		This string must correspond to command ID on server side.
		Standard implementation is, that pair of classes for client and server
		(implementing icomm::IRequest and icomm::IResponse, respectively)
		have hard coded the same ID.
	*/
	virtual QByteArray GetCommand() const = 0;
	/**
		Get error message of response.
	*/
	virtual QString ExtractMessage(const CResponseHeader& header) const = 0;
};


} // namespace icomm


