#include "CommunicationPck.h"


#include <icomp/export.h>


namespace CommunicationPck
{


I_EXPORT_PACKAGE(
			"AcfSln/CommunicationPck",
			"Package for basic components used for communcation",
			IM_PROJECT("ACF-Solutions") IM_COMPANY("ImagingTools") IM_TAG("Communication") IM_AUTHOR("\"Witold Gantzke\" \"Kirill Lepskiy\""));

I_EXPORT_COMPONENT(
			TcpRequestsManager,
			"Request manager for TCP-Protocol",
			"TCP Request Manager" IM_TAG("Network"));

I_EXPORT_COMPONENT(
			TcpClientsManager,
			"Clients manager for TCP-Protocol",
			"TCP Clients Manager" IM_TAG("Network"));

I_EXPORT_COMPONENT(
			UdpServersManager,
			"Server manager for UDP-Protocol",
			"UDP Server Manager" IM_TAG("Network"));

I_EXPORT_COMPONENT(
			UdpStream,
			"Simple UDP datagram receiver",
			"Socket Stream UDP")

I_EXPORT_COMPONENT(
			TcpStream,
			"Simple TCP/IP streamer",
			"Socket Stream TCP IP")


} // namespace CommunicationPck


