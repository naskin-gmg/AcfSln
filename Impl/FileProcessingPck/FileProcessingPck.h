#ifndef FileProcessingPck_included
#define FileProcessingPck_included


#include "icomp/TModelCompWrap.h"
#include "icomp/TMakeComponentWrap.h"

#include "ifileproc/CFileNamingComp.h"
#include "ifileproc/CFileNamingParamsComp.h"
#include "ifileproc/CFileListConverterComp.h"
#include "ifileproc/CFilePathesContainer.h"
#include "ifileproc/CSelectableFileConverterComp.h"
#include "ifileproc/CFileConvertOverProcessorComp.h"
#include "ifileproc/CExternalFileConverterComp.h"
#include "ifileproc/CCopyFilesTreeComp.h"
#include "ifileproc/CComposedCopyFilesComp.h"


/**
	System-undependent package for File Processing Framework.
*/
namespace FileProcessingPck
{


typedef ifileproc::CFileNamingComp FileNaming;

typedef icomp::TModelCompWrap<ifileproc::CFileNamingParamsComp> FileNamingParams;

typedef ifileproc::CFileListConverterComp FileListConverter;

typedef icomp::TMakeComponentWrap<
			ifileproc::CFilePathesContainer, ifile::IFileListProvider, istd::IChangeable> FilesContainer;

typedef ifileproc::CExternalFileConverterComp ExternalFileConverter;
typedef icomp::TModelCompWrap<ifileproc::CSelectableFileConverterComp> SelectableFileConverter;
typedef ifileproc::CFileConvertOverProcessorComp FileConvertOverProcessor;
typedef ifileproc::CCopyFilesTreeComp CopyFilesTree;
typedef ifileproc::CComposedCopyFilesComp ComposedCopyFiles;


} // namespace FileProcessingPck


#endif // !FileProcessingPck_included


