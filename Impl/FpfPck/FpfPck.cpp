#include "icomp/export.h"

#include "FpfPck.h"


namespace FpfPck
{


I_EXPORT_DEFAULT_SERVICES

I_EXPORT_PACKAGE("Fpf", "Acf-Solutions File-To-File Processing Framework package", "ACF-Solutions File Processing Hotfolder Workflow");

I_EXPORT_COMPONENT(
			FileProcessing,
			"Sequence processing using slave processors",
			"File Processing Hotfolder",
			icomp::IComponentStaticInfo::CCT_DATA_PROCESSING);

I_EXPORT_COMPONENT(
			DirectoryMonitorParams,
			"Parameter for the directory monitoring",
			"File Processing Parameter Directory Monitoring",
			icomp::IComponentStaticInfo::CCT_DATA);

I_EXPORT_COMPONENT(
			HotfolderLoader,
			"Hotfolder loader",
			"File Processing Parameter Directory Hotfolder",
			icomp::IComponentStaticInfo::CCT_PERSISTENCY);

I_EXPORT_COMPONENT(
			HotfolderStateModel,
			"Hotfolder's state",
			"File Processing Hotfolder",
			icomp::IComponentStaticInfo::CCT_DATA);

I_EXPORT_COMPONENT(
			MonitoringSessions,
			"Monitoring sessions",
			"File Processing Hotfolder Session",
			icomp::IComponentStaticInfo::CCT_DATA);

I_EXPORT_COMPONENT(
			HotfolderStatistics,
			"Simple hotfolder statistics",
			"File Processing Hotfolder Statistics",
			icomp::IComponentStaticInfo::CCT_DATA);

I_EXPORT_COMPONENT(
			AttributedDirectoryMonitorParams,
			"Default parameter for directory monitoring",
			"File Processing Hotfolder Directory Monitoring",
			icomp::IComponentStaticInfo::CCT_DATA);

I_EXPORT_COMPONENT(
			HotfolderWorkflow,
			"Hotfolder workflow",
			"File Processing Hotfolder Workflow",
			icomp::IComponentStaticInfo::CCT_DATA);


} // namespace FpfPck


