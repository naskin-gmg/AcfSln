#ifndef QtFpfPck_included
#define QtFpfPck_included


#include "icomp/TModelCompWrap.h"

#include "iqtfpf/CHotfolderComp.h"
#include "iqtfpf/CFolderMonitorComp.h"
#include "iqtfpf/CDirectoryMonitorParamsGui.h"


/**
	Qt based package for Flie Processing Framework.
*/
namespace QtFpfPck
{


typedef iqtfpf::CHotfolderComp Hotfolder;
typedef icomp::TModelCompWrap<iqtfpf::CFolderMonitorComp> FolderMonitor;
typedef iqtfpf::CDirectoryMonitorParamsGui DirectoryMonitorParamsGui;


} // namespace QtFpfPck


#endif // !QtFpfPck_included


