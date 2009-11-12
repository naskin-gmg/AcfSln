#include "icomp/export.h"

#include "iqt/CDefaultServicesProvider.h"

#include "QtFpfPck.h"


namespace QtFpfPck
{


I_EXPORT_SERVICES_PROVIDER(iqt::CDefaultServicesProvider);

I_EXPORT_PACKAGE("QtFpf", "Qt based package for File Processsing Framework", "Qt File Processing");

I_EXPORT_COMPONENT(Hotfolder, "Implementation of the hot folder", "File Processing Hotfolder");
I_EXPORT_COMPONENT(FolderMonitor, "This component observes changes in a given file folder", "Qt File Dir Filter Observer");
I_EXPORT_COMPONENT(DirectoryMonitorParamsGui, "Editor for directory monitor parameters", "Qt File Dir Filter Observer Observer Editor GUI Monitor");


} // namespace QtFpfPck


