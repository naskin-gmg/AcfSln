#include "QtExPck.h"


#include "icomp/export.h"


static struct StaticInit
{
	StaticInit()
	{
		Q_INIT_RESOURCE(iqtgui);
	}

} instance;


namespace QtExPck
{


I_EXPORT_PACKAGE(
			"QtExPck",
			"Acf-Solutions extended package based on Qt",
			"Extensions" IM_TAG("Qt") IM_PROJECT("ACF-Solutions") IM_COMPANY("ImagingTools") IM_AUTHOR("\"Witold Gantzke\" \"Kirill Lepskiy\""));

I_EXPORT_COMPONENT(
			TcpTerminalGui,
			"Tcp client terminal",
			"TCP Client Terminal Input Output Test" IM_TAG("GUI"));
I_EXPORT_COMPONENT(
		   XslFileSerializer,
		   "File loader serializing objects using simplified XML format using Qt, provides XSLT",
		   "Save Serializer Archive Format XSLT" IM_CATEGORY(I_DATA_PERSISTENCE) IM_TAG("XML Loader File"));


} // namespace QtExPck


