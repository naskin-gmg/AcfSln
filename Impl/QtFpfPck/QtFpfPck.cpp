#include "icomp/export.h"

#include "iqt/CDefaultServicesProvider.h"

#include "QtFpfPck.h"


namespace QtFpfPck
{


I_EXPORT_SERVICES_PROVIDER(iqt::CDefaultServicesProvider);

I_EXPORT_PACKAGE("QtFpf", "Qt based package for File Processsing Framework", "Qt File Processing");

I_EXPORT_COMPONENT(Hotfolder, "Implementation of the hot folder", "File Processing Hotfolder");
I_EXPORT_COMPONENT(DirectoryMonitor, "This component observes changes in a given file folder", "Qt File Dir Filter Observer");
I_EXPORT_COMPONENT(DirectoryMonitorParamsGui, "Editor for directory monitor parameters", "Qt File Dir Filter Observer Editor GUI Monitor");
I_EXPORT_COMPONENT(HotfolderGui, "Hotfolder view", "Qt File Observer Editor View GUI Hotfolder");
I_EXPORT_COMPONENT(FileNaming, "Component for file (re)nameing", "Qt File Hotfolder Renaming Directory");


} // namespace QtFpfPck


