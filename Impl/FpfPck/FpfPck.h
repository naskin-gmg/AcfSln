#ifndef FpfPck_included
#define FpfPck_included


#include "icomp/TModelCompWrap.h"
#include "icomp/TMakeComponentWrap.h"

#include "ifpf/CFileProcessingComp.h"
#include "ifpf/CDirectoryMonitorParams.h"
#include "ifpf/CHotfolderLoaderComp.h"


/**
	System-undependent package for File Processing Framework.
*/
namespace FpfPck
{


typedef ifpf::CFileProcessingComp FileProcessing;
typedef icomp::TModelCompWrap<
			icomp::TMakeComponentWrap<
						ifpf::CDirectoryMonitorParams, iser::ISerializable, ifpf::IDirectoryMonitorParams> > DirectoryMonitorParams;
typedef ifpf::CHotfolderLoaderComp HotfolderLoader;

} // namespace FpfPck


#endif // !FpfPck_included


