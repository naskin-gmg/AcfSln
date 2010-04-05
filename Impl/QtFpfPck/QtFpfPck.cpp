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

I_EXPORT_PACKAGE(
			"QtFpf",
			"Qt based package for File Processsing Framework",
			"File Processing" IM_TAG("Qt Hotfolder") IM_PROJECT("ACF-Solutions") IM_COMPANY("ImagingTools") IM_AUTHOR("\"Witold Gantzke\" \"Kirill Lepskiy\""));

I_EXPORT_COMPONENT(
			HotfolderProcessor,
			"Implementation of the hotfolder processor",
			"File" IM_TAG("Processing"));

I_EXPORT_COMPONENT(
			DirectoryMonitor,
			"This component observes changes in a given file folder",
			"File Dir Filter Observer");

I_EXPORT_COMPONENT(
			DirectoryMonitorParamsGui,
			"Editor for directory monitor parameters",
			"File Dir Filter Observer Editor Monitor" IM_TAG("GUI"));

I_EXPORT_COMPONENT(
			HotfolderGui,
			"Hotfolder view",
			"File Observer Editor View" IM_TAG("GUI"));

I_EXPORT_COMPONENT(
			FileNaming,
			"Component for file (re)nameing",
			"File Renaming Directory");

I_EXPORT_COMPONENT(
			ComposedHotfolderGui,
			"UI for hotfolder visualization and setting editor",
			"File Processing Editor View" IM_TAG("GUI"));

I_EXPORT_COMPONENT(
			ProcessingItemInfoGui,
			"GUI for the info a hotfolder processing item",
			"File Processing Editor View GUI Item");

I_EXPORT_COMPONENT(
			ProcessingItemPreviewGui,
			"GUI for the graphical preview of a hotfolder processing item",
			"File Processing Editor View Item" IM_TAG("GUI"));

I_EXPORT_COMPONENT(
			DirectoryItemGui,
			"View sub-component for a hotfolder directory item",
			"File Processing Editor View GUI Item");

I_EXPORT_COMPONENT(
			ProcessingItemPreviewGenerator,
			"Preview generator for a processing item",
			"File Processing Preview Generator");

I_EXPORT_COMPONENT(
			FilePreviewGenerator,
			"Preview generator for a file",
			"File Preview Generator");

I_EXPORT_COMPONENT(
			HotfolderWorkflowScenographer,
			"Hotfolder workflow visualization",
			"Hotfolder File Procesing Observer");

I_EXPORT_COMPONENT(
			HotfolderContainerGui,
			"Hotfolder container UI",
			"Hotfolder File Procesing Observer" IM_TAG("GUI"));


} // namespace QtFpfPck


