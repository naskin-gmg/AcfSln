#ifndef ifileproc_CFileNamingComp_included
#define ifileproc_CFileNamingComp_included


// ACF includes
#include <ifile/IFilePersistence.h>
#include <ilog/TLoggerCompWrap.h>
#include <ifile/IFileNameParam.h>

// ACF-Solutions includes
#include <ifileproc/IFileNaming.h>
#include <ifileproc/IFileNamingParams.h>


namespace ifileproc
{


/**
	Component for calculating of the new file name.
	\sa ifileproc::IFileNaming
*/
class CFileNamingComp:
			public ilog::CLoggerComponentBase,
			virtual public ifileproc::IFileNaming
{
public:
	typedef ilog::CLoggerComponentBase BaseClass;

	I_BEGIN_COMPONENT(CFileNamingComp);
		I_REGISTER_INTERFACE(ifileproc::IFileNaming);
		I_ASSIGN(m_fileTypeInfoCompPtr, "OutputFileInfo", "Output file info, which will be used for file name calculation", false, "OutputFileInfo");
		I_ASSIGN(m_directoryPathCompPtr, "DirectoryPath", "Default path of the output directory", false, "DirectoryPath"); 
		I_ASSIGN(m_fileNamingParamsCompPtr, "FileNamingParams", "Rules for the file naming", false, "FileNamingParams"); 
	I_END_COMPONENT;

	// reimplemented (ifileproc::IFileNaming)
	virtual QString CalculateFileName(
				const QString& inputFileName,
				const QString& outputDirectoryPath,
				const ifileproc::IFileNamingParams* fileNamingParamsPtr) const;

private:
	I_REF(ifile::IFileTypeInfo, m_fileTypeInfoCompPtr);
	I_REF(ifile::IFileNameParam, m_directoryPathCompPtr);
	I_REF(ifileproc::IFileNamingParams, m_fileNamingParamsCompPtr);
};


} // namespace ifileproc


#endif // !ifileproc_CFileNamingComp_included

