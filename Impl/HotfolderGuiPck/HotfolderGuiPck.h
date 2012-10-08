#ifndef HotfolderGuiPck_included
#define HotfolderGuiPck_included


#include "icomp/TModelCompWrap.h"

#include "ihotfgui/CHotfolderProcessingComp.h"
#include "ihotfgui/CDirectoryMonitorComp.h"
#include "ihotfgui/CDirectoryMonitorParamsGui.h"
#include "ihotfgui/CHotfolderGuiComp.h"
#include "ihotfgui/CProcessingItemPreviewGuiComp.h"
#include "ihotfgui/CDirectoryItemGuiComp.h"
#include "ihotfgui/CHotfolderContainerGuiComp.h"
#include "ihotfgui/CHotfolderTaskManagerComp.h"


/**
	Qt based package for Flie Processing Framework.
*/
namespace HotfolderGuiPck
{


typedef ihotfgui::CHotfolderProcessingComp HotfolderProcessor;
typedef ihotfgui::CDirectoryMonitorComp DirectoryMonitor;
typedef ihotfgui::CDirectoryMonitorParamsGui DirectoryMonitorParamsGui;
typedef ihotfgui::CHotfolderGuiComp HotfolderGui;
typedef ihotfgui::CProcessingItemPreviewGuiComp ProcessingItemPreviewGui;
typedef ihotfgui::CDirectoryItemGuiComp DirectoryItemGui;
typedef ihotfgui::CHotfolderContainerGuiComp HotfolderContainerGui;
typedef ihotfgui::CHotfolderTaskManagerComp HotfolderTaskManager;


} // namespace HotfolderGuiPck


#endif // !HotfolderGuiPck_included


