#ifndef FpfPck_included
#define FpfPck_included


#include "icomp/TModelCompWrap.h"
#include "icomp/TMakeComponentWrap.h"

#include "ifpf/CFileProcessingComp.h"
#include "ifpf/CDirectoryMonitorParams.h"
#include "ifpf/CHotfolder.h"
#include "ifpf/CMonitoringSessionsMap.h"
#include "ifpf/CHotfolderStatistics.h"
#include "ifpf/CHotfolderLoaderComp.h"
#include "ifpf/CDirectoryMonitorParamsComp.h"
#include "ifpf/CHotfolderWorkflowComp.h"


/**
	System-undependent package for File Processing Framework.
*/
namespace FpfPck
{


typedef ifpf::CFileProcessingComp FileProcessing;
typedef icomp::TModelCompWrap<
			icomp::TMakeComponentWrap<
						ifpf::CDirectoryMonitorParams, iser::ISerializable, ifpf::IDirectoryMonitorParams> > DirectoryMonitorParams;
typedef icomp::TModelCompWrap<
			icomp::TMakeComponentWrap<
						ifpf::CHotfolder, iser::ISerializable, ifpf::IHotfolderProcessingInfo> > HotfolderStateModel;
typedef icomp::TModelCompWrap<
			icomp::TMakeComponentWrap<
						ifpf::CMonitoringSessionsMap, iser::ISerializable, ifpf::IMonitoringSessionManager> > MonitoringSessions;
typedef icomp::TModelCompWrap<
			icomp::TMakeComponentWrap<
						ifpf::CHotfolderStatistics, iser::ISerializable, imod::IObserver, ifpf::IHotfolderStatistics> > HotfolderStatistics;
typedef ifpf::CHotfolderLoaderComp HotfolderLoader;

typedef icomp::TModelCompWrap<ifpf::CDirectoryMonitorParamsComp> AttributedDirectoryMonitorParams;

typedef icomp::TModelCompWrap<ifpf::CHotfolderWorkflowComp> HotfolderWorkflow;


} // namespace FpfPck


#endif // !FpfPck_included


