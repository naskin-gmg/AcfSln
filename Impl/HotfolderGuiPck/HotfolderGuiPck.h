// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#ifndef HotfolderGuiPck_included
#define HotfolderGuiPck_included


#include <icomp/TModelCompWrap.h>

#include <ihotfgui/CDirectoryMonitorComp.h>
#include <ihotfgui/CDirectoryMonitorParamsGui.h>
#include <ihotfgui/CFileNamingParamsGuiComp.h>


/**
	Qt based package for Flie Processing Framework.
*/
namespace HotfolderGuiPck
{


typedef ihotfgui::CDirectoryMonitorComp DirectoryMonitor;
typedef ihotfgui::CDirectoryMonitorParamsGui DirectoryMonitorParamsGui;
typedef ihotfgui::CFileNamingParamsGuiComp FileNamingParamsGui;


} // namespace HotfolderGuiPck


#endif // !HotfolderGuiPck_included


