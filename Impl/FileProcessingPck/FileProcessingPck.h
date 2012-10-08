#ifndef FileProcessingPck_included
#define FileProcessingPck_included


#include "icomp/TModelCompWrap.h"
#include "icomp/TMakeComponentWrap.h"

#include "ifileproc/CFileNamingComp.h"
#include "ifileproc/CFileNamingParamsComp.h"
#include "ifileproc/CFileListConverterComp.h"
#include "ifileproc/CFilePathesContainer.h"


/**
	System-undependent package for File Processing Framework.
*/
namespace FileProcessingPck
{


typedef ifileproc::CFileNamingComp FileNaming;

typedef icomp::TModelCompWrap<ifileproc::CFileNamingParamsComp> FileNamingParams;

typedef ifileproc::CFileListConverterComp FileListConverter;

typedef icomp::TMakeComponentWrap<
			ifileproc::CFilePathesContainer, ibase::IFileListProvider, istd::IChangeable> FilesContainer;


} // namespace FileProcessingPck


#endif // !FileProcessingPck_included


