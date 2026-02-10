// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#ifndef HotfolderPck_included
#define HotfolderPck_included


#include <icomp/TModelCompWrap.h>
#include <icomp/TMakeComponentWrap.h>

#include <ihotf/CMonitoringSessionsManager.h>
#include <ihotf/CDirectoryMonitorParamsComp.h>
#include <ihotf/CFileSystemChangeStorage.h>


/**
	System-undependent package for File Processing Framework.
*/
namespace HotfolderPck
{


typedef icomp::TModelCompWrap<
			icomp::TMakeComponentWrap<
						ihotf::CMonitoringSessionsManager, iser::ISerializable, ihotf::IMonitoringSessionManager> > MonitoringSessionsManager;

typedef icomp::TModelCompWrap<
			ihotf::CDirectoryMonitorParamsComp> DirectoryMonitorParams;

typedef icomp::TModelCompWrap<
			icomp::TMakeComponentWrap<
						ihotf::CFileSystemChangeStorage, ihotf::IFileSystemChangeStorage> > FileSystemChangeStorage;


} // namespace HotfolderPck


#endif // !HotfolderPck_included


