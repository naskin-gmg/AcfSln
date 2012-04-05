#include "FpfPck.h"


#include "icomp/export.h"


namespace FpfPck
{


I_EXPORT_PACKAGE(
			"AcfSln/Fpf",
			"Acf-Solutions File-To-File Processing Framework package",
			"File Processing Hotfolder Workflow" IM_TAG("Hotfolder") IM_PROJECT("ACF-Solutions") IM_COMPANY("ImagingTools") IM_AUTHOR("\"Witold Gantzke\" \"Kirill Lepskiy\""));

I_EXPORT_COMPONENT(
			DirectoryMonitorParams,
			"Parameter for the directory monitoring",
			"Processing Parameter Directory Monitoring" IM_TAG("File Model"));

I_EXPORT_COMPONENT(
			HotfolderLoader,
			"Hotfolder loader",
			"File Processing Parameter Directory Hotfolder" IM_TAG("Loader"));

I_EXPORT_COMPONENT(
			HotfolderProcessingInfo,
			"Hotfolder's state",
			"File Processing Hotfolder" IM_TAG("Model"));

I_EXPORT_COMPONENT(
			MonitoringSessionsManager,
			"Monitoring sessions",
			"File Processing Hotfolder Session");

I_EXPORT_COMPONENT(
			HotfolderStatistics,
			"Simple hotfolder statistics",
			"File Processing Hotfolder Statistics");

I_EXPORT_COMPONENT(
			AttributedDirectoryMonitorParams,
			"Default parameter for directory monitoring",
			"File Processing Hotfolder Directory Monitoring");

I_EXPORT_COMPONENT(
			HotfolderWorkflow,
			"Hotfolder workflow",
			"File Processing Hotfolder Workflow");

I_EXPORT_COMPONENT(
			VisualHotfolderWorkflow,
			"Visualization model of a hotfolder workflow",
			"File Processing Hotfolder Workflow");

I_EXPORT_COMPONENT(
			FileNamingParams,
			"Parmeter for file (re)naming",
			"File Processing Hotfolder Parameter");

I_EXPORT_COMPONENT(
			FileSystemChangeStorage,
			"File storage component",
			"File Processing Hotfolder Parameter");

} // namespace FpfPck


