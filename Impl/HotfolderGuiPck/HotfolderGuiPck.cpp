// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#include "HotfolderGuiPck.h"


#include <icomp/export.h>


I_REGISTER_QT_RESOURCE(iqtgui);
I_REGISTER_QT_RESOURCE(ihotfgui);


namespace HotfolderGuiPck
{


I_EXPORT_PACKAGE(
			"AcfSln/Hotfolder/Gui",
			"Qt based package for File Processsing Framework",
			"File Processing" IM_TAG("Qt Hotfolder") IM_PROJECT("ACF-Solutions") IM_COMPANY("ImagingTools") IM_AUTHOR("\"Witold Gantzke\" \"Kirill Lepskiy\""));

I_EXPORT_COMPONENT(
			DirectoryMonitor,
			"This component observes changes in a given file folder",
			"File Dir Filter Observer");

I_EXPORT_COMPONENT(
			DirectoryMonitorParamsGui,
			"Editor for directory monitor parameters",
			"File Dir Filter Observer Editor Monitor" IM_TAG("GUI"));

I_EXPORT_COMPONENT(
			FileNamingParamsGui,
			"Editor for file naming parameters",
			"File Dir Name" IM_TAG("GUI"));


} // namespace HotfolderGuiPck


