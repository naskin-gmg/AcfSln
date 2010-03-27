#include "icomp/export.h"

#include "QtExPck.h"


static struct StaticInit
{
	StaticInit()
	{
		Q_INIT_RESOURCE(iqtgui);
	}

} instance;


namespace QtExPck
{


I_EXPORT_DEFAULT_SERVICES

I_EXPORT_PACKAGE("QtExPck", "Acf-Solutions extended package based on Qt", "Acf-Solutions Qt Extensions");

I_EXPORT_COMPONENT(
			TcpTerminalGui,
			"Tcp client terminal",
			"TCP Client Terminal Input Output Test",
			icomp::IComponentStaticInfo::CCT_SERVICE);


} // namespace QtExPck


