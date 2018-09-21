#pragma once


// Qt includes
#include <QtCore/QtGlobal>


namespace icomm
{


/**
	Data definition for simple communication header.
*/
class CTcpMessageHeader
{
public:
	enum ClientCommand
	{
		/**
			Client set its version to the server.
		*/
		CC_VERSION_COMMAND,
		/**
			Client call server-side request.
		*/
		CC_REQUEST_COMMAND,
		/**
			Server send the response to the client request.
		*/
		CC_RESPONSE_COMMAND
	};

	quint32 command;
	quint32 size;
};


}// namespace icomm


