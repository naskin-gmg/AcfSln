#include "icomp/export.h"

#include "FpfPck.h"


namespace FpfPck
{


I_EXPORT_DEFAULT_SERVICES

I_EXPORT_PACKAGE("Fpf", "ImagingTools File2File Processing Framework package", "ImagingTools File Processing");

I_EXPORT_COMPONENT(FileProcessing, "Sequence processing using slave processors", "File Processing Hotfolder");
I_EXPORT_COMPONENT(DirectoryMonitorParams, "Parameter for the directory monitoring", "File Processing Parameter Directory Monitoring");
I_EXPORT_COMPONENT(HotfolderLoader, "Hotfolder loader", "File Processing Parameter Directory Hotfolder");


} // namespace FpfPck


