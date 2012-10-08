#ifndef HotfolderPck_included
#define HotfolderPck_included


#include "icomp/TModelCompWrap.h"
#include "icomp/TMakeComponentWrap.h"

#include "ihotf/CDirectoryMonitorParams.h"
#include "ihotf/CHotfolderProcessingInfo.h"
#include "ihotf/CMonitoringSessionsManager.h"
#include "ihotf/CHotfolderStatistics.h"
#include "ihotf/CHotfolderLoaderComp.h"
#include "ihotf/CDirectoryMonitorParamsComp.h"
#include "ihotf/CHotfolderWorkflowComp.h"
#include "ihotf/CVisualHotfolderWorkflowComp.h"
#include "ihotf/CFileSystemChangeStorage.h"


/**
	System-undependent package for File Processing Framework.
*/
namespace HotfolderPck
{


typedef icomp::TModelCompWrap<
			icomp::TMakeComponentWrap<
						ihotf::CDirectoryMonitorParams, iser::ISerializable, ihotf::IDirectoryMonitorParams> > DirectoryMonitorParams;

typedef icomp::TModelCompWrap<
			icomp::TMakeComponentWrap<
						ihotf::CHotfolderProcessingInfo, iser::ISerializable, ihotf::IHotfolderProcessingInfo> > HotfolderProcessingInfo;

typedef icomp::TModelCompWrap<
			icomp::TMakeComponentWrap<
						ihotf::CMonitoringSessionsManager, iser::ISerializable, ihotf::IMonitoringSessionManager> > MonitoringSessionsManager;

typedef icomp::TModelCompWrap<
			icomp::TMakeComponentWrap<
						ihotf::CHotfolderStatistics, iser::ISerializable, imod::IObserver, ihotf::IHotfolderStatistics> > HotfolderStatistics;
typedef ihotf::CHotfolderLoaderComp HotfolderLoader;

typedef icomp::TModelCompWrap<ihotf::CDirectoryMonitorParamsComp> AttributedDirectoryMonitorParams;

typedef icomp::TModelCompWrap<ihotf::CHotfolderWorkflowComp> HotfolderWorkflow;

typedef icomp::TModelCompWrap<ihotf::CVisualHotfolderWorkflowComp> VisualHotfolderWorkflow;

typedef icomp::TModelCompWrap<
			icomp::TMakeComponentWrap<
						ihotf::CFileSystemChangeStorage, ihotf::IFileSystemChangeStorage> > FileSystemChangeStorage;


} // namespace HotfolderPck


#endif // !HotfolderPck_included


