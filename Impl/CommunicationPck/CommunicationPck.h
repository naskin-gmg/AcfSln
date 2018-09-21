#ifndef CommunicationPckPck_included
#define CommunicationPckPck_included


// ACF includes
#include <icomp/TModelCompWrap.h>
#include <icomp/TMakeComponentWrap.h>
#include <ibase/TModelObserverCompWrap.h>

// ACF-Solutions includes
#include <icomm/CTcpClientsManagerComp.h>
#include <icomm/CTcpRequestsManagerComp.h>
#include <icomm/CTcpStreamComp.h>
#include <icomm/CUdpServersManagerComp.h>
#include <icomm/CUdpStreamComp.h>


/**
	Package for basic communication components.
*/
namespace CommunicationPck
{


typedef icomm::CTcpRequestsManagerComp TcpRequestsManager;
typedef icomm::CTcpClientsManagerComp TcpClientsManager;
typedef icomp::TModelCompWrap<icomm::CUdpServersManagerComp> UdpServersManager;
typedef ibase::TModelObserverCompWrap<icomm::CUdpStreamComp> UdpStream;
typedef ibase::TModelObserverCompWrap<icomm::CTcpStreamComp> TcpStream;


} // namespace CommunicationPck


#endif // !CommunicationPckPck_included


