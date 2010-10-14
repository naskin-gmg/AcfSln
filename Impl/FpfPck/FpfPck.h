#ifndef FpfPck_included
#define FpfPck_included


#include "icomp/TModelCompWrap.h"
#include "icomp/TMakeComponentWrap.h"

#include "ifpf/CDirectoryMonitorParams.h"
#include "ifpf/CHotfolderProcessingInfo.h"
#include "ifpf/CMonitoringSessionsManager.h"
#include "ifpf/CHotfolderStatistics.h"
#include "ifpf/CHotfolderLoaderComp.h"
#include "ifpf/CDirectoryMonitorParamsComp.h"
#include "ifpf/CHotfolderWorkflowComp.h"
#include "ifpf/CVisualHotfolderWorkflowComp.h"
#include "ifpf/CFileNamingParamsComp.h"


/**
	System-undependent package for File Processing Framework.
*/
namespace FpfPck
{


typedef icomp::TModelCompWrap<
			icomp::TMakeComponentWrap<
						ifpf::CDirectoryMonitorParams, iser::ISerializable, ifpf::IDirectoryMonitorParams> > DirectoryMonitorParams;

typedef icomp::TModelCompWrap<
			icomp::TMakeComponentWrap<
						ifpf::CHotfolderProcessingInfo, iser::ISerializable, ifpf::IHotfolderProcessingInfo> > HotfolderProcessingInfo;

typedef icomp::TModelCompWrap<
			icomp::TMakeComponentWrap<
						ifpf::CMonitoringSessionsManager, iser::ISerializable, ifpf::IMonitoringSessionManager> > MonitoringSessionsManager;

typedef icomp::TModelCompWrap<
			icomp::TMakeComponentWrap<
						ifpf::CHotfolderStatistics, iser::ISerializable, imod::IObserver, ifpf::IHotfolderStatistics> > HotfolderStatistics;
typedef ifpf::CHotfolderLoaderComp HotfolderLoader;

typedef icomp::TModelCompWrap<ifpf::CDirectoryMonitorParamsComp> AttributedDirectoryMonitorParams;

typedef icomp::TModelCompWrap<ifpf::CHotfolderWorkflowComp> HotfolderWorkflow;

typedef icomp::TModelCompWrap<ifpf::CVisualHotfolderWorkflowComp> VisualHotfolderWorkflow;

typedef icomp::TModelCompWrap<ifpf::CFileNamingParamsComp> FileNamingParams;


} // namespace FpfPck


#endif // !FpfPck_included


