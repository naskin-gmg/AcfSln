#include "icomp/export.h"

#include "iqt/CDefaultServicesProvider.h"

#include "QtFpfPck.h"


namespace QtFpfPck
{


static struct StaticInit
{
	StaticInit()
	{
		Q_INIT_RESOURCE(iqtgui);
		Q_INIT_RESOURCE(iqtfpf);
	}

} instance;


I_EXPORT_SERVICES_PROVIDER(iqt::CDefaultServicesProvider);

I_EXPORT_PACKAGE("QtFpf", "Qt based package for File Processsing Framework", "Qt File Processing");

I_EXPORT_COMPONENT(
			HotfolderProcessor,
			"Implementation of the hotfolder processor",
			"File Processing Hotfolder",
			icomp::IComponentStaticInfo::CCT_DATA_PROCESSING);
I_EXPORT_COMPONENT(
			DirectoryMonitor,
			"This component observes changes in a given file folder",
			"Qt File Dir Filter Observer",
			icomp::IComponentStaticInfo::CCT_SERVICE);

I_EXPORT_COMPONENT(
			DirectoryMonitorParamsGui,
			"Editor for directory monitor parameters",
			"Qt File Dir Filter Observer Editor GUI Monitor",
			icomp::IComponentStaticInfo::CCT_GUI | icomp::IComponentStaticInfo::CCT_DATA_PRESENTATION);

I_EXPORT_COMPONENT(
			HotfolderGui,
			"Hotfolder view",
			"Qt File Observer Editor View GUI Hotfolder",
			icomp::IComponentStaticInfo::CCT_GUI | icomp::IComponentStaticInfo::CCT_DATA_PRESENTATION);

I_EXPORT_COMPONENT(
			FileNaming,
			"Component for file (re)nameing",
			"Qt File Hotfolder Renaming Directory",
			icomp::IComponentStaticInfo::CCT_SERVICE);

I_EXPORT_COMPONENT(
			ComposedHotfolderGui,
			"UI for hotfolder visualization and setting editor",
			"File Processing Hotfolder Editor View GUI",
			icomp::IComponentStaticInfo::CCT_GUI | icomp::IComponentStaticInfo::CCT_DATA_PRESENTATION);

I_EXPORT_COMPONENT(
			ProcessingItemInfoGui,
			"GUI for the info a hotfolder processing item",
			"File Processing Hotfolder Editor View GUI Item",
			icomp::IComponentStaticInfo::CCT_GUI | icomp::IComponentStaticInfo::CCT_DATA_PRESENTATION);

I_EXPORT_COMPONENT(
			ProcessingItemPreviewGui,
			"GUI for the graphical preview of a hotfolder processing item",
			"File Processing Hotfolder Editor View GUI Item",
			icomp::IComponentStaticInfo::CCT_GUI | icomp::IComponentStaticInfo::CCT_DATA_PRESENTATION);

I_EXPORT_COMPONENT(
			DirectoryItemGui,
			"View sub-component for a hotfolder directory item",
			"File Processing Hotfolder Editor View GUI Item",
			icomp::IComponentStaticInfo::CCT_GUI | icomp::IComponentStaticInfo::CCT_DATA_PRESENTATION);

I_EXPORT_COMPONENT(
			ProcessingItemPreviewGenerator,
			"Preview generator for a processing item",
			"File Processing Hotfolder Preview Generator",
			icomp::IComponentStaticInfo::CCT_SERVICE);

I_EXPORT_COMPONENT(
			FilePreviewGenerator,
			"Preview generator for a file",
			"File Preview Generator",
			icomp::IComponentStaticInfo::CCT_SERVICE);

I_EXPORT_COMPONENT(
			HotfolderWorkflowScenographer,
			"Hotfolder workflow visualization",
			"Hotfolder File Procesing Observer",
			icomp::IComponentStaticInfo::CCT_GUI | icomp::IComponentStaticInfo::CCT_DATA_PRESENTATION);

I_EXPORT_COMPONENT(
			HotfolderContainerGui,
			"Hotfolder container UI",
			"Hotfolder File Procesing Observer",
			icomp::IComponentStaticInfo::CCT_GUI | icomp::IComponentStaticInfo::CCT_DATA_PRESENTATION);


} // namespace QtFpfPck


