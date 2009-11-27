#ifndef QtFpfPck_included
#define QtFpfPck_included


#include "icomp/TModelCompWrap.h"

#include "iqtfpf/CHotfolderComp.h"
#include "iqtfpf/CDirectoryMonitorComp.h"
#include "iqtfpf/CDirectoryMonitorParamsGui.h"
#include "iqtfpf/CHotfolderGuiComp.h"


/**
	Qt based package for Flie Processing Framework.
*/
namespace QtFpfPck
{


typedef icomp::TModelCompWrap<iqtfpf::CHotfolderComp> Hotfolder;
typedef icomp::TModelCompWrap<iqtfpf::CDirectoryMonitorComp> DirectoryMonitor;
typedef iqtfpf::CDirectoryMonitorParamsGui DirectoryMonitorParamsGui;
typedef iqtfpf::CHotfolderGuiComp HotfolderGui;


} // namespace QtFpfPck


#endif // !QtFpfPck_included


