#include "HotfolderPck.h"


#include <icomp/export.h>


namespace HotfolderPck
{


I_EXPORT_PACKAGE(
			"AcfSln/Hotfolder",
			"Acf-Solutions File-To-File Processing Framework package",
			"File Processing Hotfolder Workflow" IM_TAG("Hotfolder") IM_PROJECT("ACF-Solutions") IM_COMPANY("ImagingTools") IM_AUTHOR("\"Witold Gantzke\" \"Kirill Lepskiy\""));

I_EXPORT_COMPONENT(
			DirectoryMonitorParams,
			"Parameter for the directory monitoring",
			"Processing Parameter Directory Monitoring" IM_TAG("File Model"));

I_EXPORT_COMPONENT(
			MonitoringSessionsManager,
			"Monitoring sessions",
			"File Processing Hotfolder Session");

I_EXPORT_COMPONENT(
			FileSystemChangeStorage,
			"File storage component",
			"File Processing Hotfolder Parameter");


} // namespace HotfolderPck


