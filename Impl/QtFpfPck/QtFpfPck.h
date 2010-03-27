#ifndef QtFpfPck_included
#define QtFpfPck_included


#include "icomp/TModelCompWrap.h"

#include "iqtfpf/CHotfolderProcessingComp.h"
#include "iqtfpf/CDirectoryMonitorComp.h"
#include "iqtfpf/CDirectoryMonitorParamsGui.h"
#include "iqtfpf/CHotfolderGuiComp.h"
#include "iqtfpf/CFileNamingComp.h"
#include "iqtfpf/CComposedHotfolderGuiComp.h"
#include "iqtfpf/CProcessingItemInfoGuiComp.h"
#include "iqtfpf/CProcessingItemPreviewGuiComp.h"
#include "iqtfpf/CDirectoryItemGuiComp.h"
#include "iqtfpf/CProcessingItemPreviewGeneratorComp.h"
#include "iqtfpf/CFilePreviewGeneratorComp.h"
#include "iqtfpf/CHotfolderWorkflowScenographerComp.h"
#include "iqtfpf/CHotfolderContainerGuiComp.h"


/**
	Qt based package for Flie Processing Framework.
*/
namespace QtFpfPck
{


typedef iqtfpf::CHotfolderProcessingComp HotfolderProcessor;
typedef icomp::TModelCompWrap<iqtfpf::CDirectoryMonitorComp> DirectoryMonitor;
typedef iqtfpf::CDirectoryMonitorParamsGui DirectoryMonitorParamsGui;
typedef iqtfpf::CHotfolderGuiComp HotfolderGui;
typedef iqtfpf::CFileNamingComp FileNaming;
typedef iqtfpf::CComposedHotfolderGuiComp ComposedHotfolderGui;
typedef iqtfpf::CProcessingItemInfoGuiComp ProcessingItemInfoGui;
typedef iqtfpf::CProcessingItemPreviewGuiComp ProcessingItemPreviewGui;
typedef iqtfpf::CDirectoryItemGuiComp DirectoryItemGui;
typedef iqtfpf::CProcessingItemPreviewGeneratorComp ProcessingItemPreviewGenerator;
typedef iqtfpf::CFilePreviewGeneratorComp FilePreviewGenerator;
typedef iqtfpf::CHotfolderWorkflowScenographerComp HotfolderWorkflowScenographer;
typedef iqtfpf::CHotfolderContainerGuiComp HotfolderContainerGui;


} // namespace QtFpfPck


#endif // !QtFpfPck_included


