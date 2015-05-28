#ifndef QtExPck_included
#define QtExPck_included


#include "icomp/TModelCompWrap.h"
#include "icomp/TMakeComponentWrap.h"

#include "iqtex/CTcpTerminalGuiComp.h"
#include "iqtex/CXslSerializerComp.h"

#include "iservice/CServiceApplicationComp.h"


/**
	Qt package with extended untipical components.
*/
namespace QtExPck
{


typedef iqtex::CTcpTerminalGuiComp TcpTerminalGui;
typedef iqtex::CXslSerializerComp XslFileSerializer;
typedef iservice::CServiceApplicationComp ServiceApplication;


} // namespace QtExPck


#endif // !QtExPck_included


