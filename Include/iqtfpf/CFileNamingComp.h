#ifndef iqtfpf_CFileNamingComp_included
#define iqtfpf_CFileNamingComp_included


// ACF includes
#include "iser/IFileLoader.h"

#include "ibase/TLoggerCompWrap.h"


// AcfSln includes
#include "ifpf/IFileNaming.h"


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
		I_ASSIGN(m_fileLoaderCompPtr, "FileLoader", "File loader, which will used the result file name", false, "FileLoader");
		I_ASSIGN(m_outputDirectoryIdAttrPtr, "OutputDirectoryId", "Parameter set ID for the output directory parameter", true, ""); 
		I_ASSIGN(m_fileNameStrategyIdAttrPtr, "FileNameStrategyId", "Parameter set ID for the file renaming strategy parameter", true, ""); 
	I_END_COMPONENT();

	// reimplemented (ifpf::IFileNaming)
	virtual istd::CString GetFilePath(const istd::CString& inputFileName, const iprm::IParamsSet* paramsSetPtr) const;

private:
	I_REF(iser::IFileLoader, m_fileLoaderCompPtr);
	I_ATTR(istd::CString, m_outputDirectoryIdAttrPtr);
	I_ATTR(istd::CString, m_fileNameStrategyIdAttrPtr);
};


} // namespace iqtfpf


#endif // !iqtfpf_CFileNamingComp_included

