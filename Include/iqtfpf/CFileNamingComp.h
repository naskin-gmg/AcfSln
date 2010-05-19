#ifndef iqtfpf_CFileNamingComp_included
#define iqtfpf_CFileNamingComp_included


// ACF includes
#include "iser/IFileLoader.h"

#include "ibase/TLoggerCompWrap.h"

#include "iprm/IFileNameParam.h"


// AcfSln includes
#include "ifpf/IFileNaming.h"
#include "ifpf/IFileNamingParams.h"


namespace iqtfpf
{


/**
	Component for calculating of the new file name.
	\sa ifpf::IFileNaming
*/
class CFileNamingComp:
			public ibase::CLoggerComponentBase,
			virtual public ifpf::IFileNaming
{
public:
	typedef ibase::CLoggerComponentBase BaseClass;

	I_BEGIN_COMPONENT(CFileNamingComp);
		I_REGISTER_INTERFACE(ifpf::IFileNaming);
		I_ASSIGN(m_fileTypeInfoCompPtr, "OutputFileInfo", "Output file info, which will be used for file name calculation", false, "OutputFileInfo");
		I_ASSIGN(m_directoryPathCompPtr, "DirectoryPath", "Output directory", true, "DirectoryPath"); 
		I_ASSIGN(m_fileNamingParamsCompPtr, "FileNamingParams", "Rules for the file naming", false, "FileNamingParams"); 
	I_END_COMPONENT();

	// reimplemented (ifpf::IFileNaming)
	virtual istd::CString GetFilePath(const istd::CString& inputFileName) const;

private:
	I_REF(iser::IFileTypeInfo, m_fileTypeInfoCompPtr);
	I_REF(iprm::IFileNameParam, m_directoryPathCompPtr);
	I_REF(ifpf::IFileNamingParams, m_fileNamingParamsCompPtr);
};


} // namespace iqtfpf


#endif // !iqtfpf_CFileNamingComp_included

