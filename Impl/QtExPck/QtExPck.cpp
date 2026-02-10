// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#include "QtExPck.h"


#include <icomp/export.h>


I_REGISTER_QT_RESOURCE(iqtgui);


namespace QtExPck
{


I_EXPORT_PACKAGE(
			"AcfSln/QtExtension",
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

I_EXPORT_COMPONENT(
			ServiceApplication,
			"An application as service/daemon",
			"Service Daemon" IM_TAG("Application"));


} // namespace QtExPck


