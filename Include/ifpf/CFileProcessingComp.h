#ifndef ifpf_CFileProcessingComp_included
#define ifpf_CFileProcessingComp_included


// ACF includes
#include "ibase/IFileConvertCopy.h"
#include "ibase/TLoggerCompWrap.h"

#include "iser/IFileLoader.h"

#include "iproc/IProgressManager.h"
#include "iproc/IProcessor.h"


namespace ifpf
{


/**
	Component for processing a file.
*/
class CFileProcessingComp:
			public ibase::CLoggerComponentBase,
			virtual public ibase::IFileConvertCopy
{
public:
	typedef ibase::CLoggerComponentBase BaseClass;

	CFileProcessingComp();

	I_BEGIN_COMPONENT(CFileProcessingComp);
		I_REGISTER_INTERFACE(ibase::IFileConvertCopy);
		I_ASSIGN(m_inputFileLoaderCompPtr, "InputFileLoader", "Loader for the input file", true, "InputFileLoader");
		I_ASSIGN(m_outputFileLoaderCompPtr, "OutputFileLoader", "Persistence for the output file. If not set, the input file loader will be used", false, "OutputFileLoader");
		I_ASSIGN(m_processorCompPtr, "DataProcessor", "Processor for the file", true, "FileProcessor");
		I_ASSIGN(m_inputDataCompPtr, "InputData", "Input data definition", true, "InputData");
		I_ASSIGN(m_outputDataCompPtr, "OutputData", "Output data definition", true, "OutputData");
		I_ASSIGN(m_progressManagerCompPtr, "ProgressManager", "Processing progress observer", false, "ProgressManager");
	I_END_COMPONENT();

	// reimplemented (ibase::IFileConvertCopy)
	virtual bool CopyFile(
				const istd::CString& inputFilePath,
				const istd::CString& outputFilePath,
				const iprm::IParamsSet* paramsPtr = NULL) const;

private:
	I_REF(iser::IFileLoader, m_inputFileLoaderCompPtr);
	I_REF(iser::IFileLoader, m_outputFileLoaderCompPtr);
	I_REF(iproc::IProcessor, m_processorCompPtr);
	I_REF(istd::IChangeable, m_inputDataCompPtr);
	I_REF(istd::IChangeable, m_outputDataCompPtr);
	I_REF(iproc::IProgressManager, m_progressManagerCompPtr);

	int m_progressSessionId;
};


} // namespace ifpf


#endif // !ifpf_CFileProcessingComp_included


