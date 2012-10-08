#include "HotfolderGuiPck.h"


#include "icomp/export.h"


I_REGISTER_QT_RESOURCE(iqtgui);
I_REGISTER_QT_RESOURCE(ihotfgui);


namespace HotfolderGuiPck
{


I_EXPORT_PACKAGE(
			"AcfSln/Fpf/Gui",
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
			ProcessingItemPreviewGui,
			"GUI for the graphical preview of a hotfolder processing item",
			"File Processing Editor View Item" IM_TAG("GUI"));

I_EXPORT_COMPONENT(
			DirectoryItemGui,
			"View sub-component for a hotfolder directory item",
			"File Processing Editor View GUI Item");

I_EXPORT_COMPONENT(
			HotfolderContainerGui,
			"Hotfolder container UI",
			"Hotfolder File Procesing Observer" IM_TAG("GUI"));

I_EXPORT_COMPONENT(
			HotfolderTaskManager,
			"Hotfolder task manager",
			"File Procesing" IM_TAG("Hotfolder"));


} // namespace HotfolderGuiPck


