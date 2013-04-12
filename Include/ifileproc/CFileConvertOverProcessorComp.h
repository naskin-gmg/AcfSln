#ifndef ifileproc_CFileConvertOverProcessorComp_included
#define ifileproc_CFileConvertOverProcessorComp_included


// Qt includes
#include <QtCore/QMutex>

// ACF includes
#include "istd/TSmartPtr.h"

#include "ifile/IFilePersistence.h"

#include "ifileproc/IFileConvertCopy.h"
#include "ilog/TLoggerCompWrap.h"

#include "ibase/IProgressManager.h"
#include "iproc/IProcessor.h"


namespace ifileproc
{


/**
	Component for a file conversion over a data processor.
*/
class CFileConvertOverProcessorComp:
			public ilog::CLoggerComponentBase,
			virtual public ifileproc::IFileConvertCopy
{
public:
	typedef ilog::CLoggerComponentBase BaseClass;

	CFileConvertOverProcessorComp();

	I_BEGIN_COMPONENT(CFileConvertOverProcessorComp);
		I_REGISTER_INTERFACE(ifileproc::IFileConvertCopy);
		I_ASSIGN(m_inputFileLoaderCompPtr, "InputFileLoader", "Loader for the input file", true, "InputFileLoader");
		I_ASSIGN(m_outputFileLoaderCompPtr, "OutputFileLoader", "Persistence for the output file. If not set, the input file loader will be used", false, "OutputFileLoader");
		I_ASSIGN(m_processorCompPtr, "DataProcessor", "Processor for the file", true, "FileProcessor");
		I_ASSIGN(m_inputDataCompPtr, "InputData", "Input data definition", true, "InputData");
		I_ASSIGN(m_outputDataCompPtr, "OutputData", "Output data definition", true, "OutputData");
		I_ASSIGN(m_progressManagerCompPtr, "ProgressManager", "Processing progress observer", false, "ProgressManager");
		I_ASSIGN(m_processingParamsSetCompPtr, "ProcessingParams", "Processing parameters", false, "ProcessingParams");
	I_END_COMPONENT;

	// reimplemented (ifileproc::IFileConvertCopy)
	virtual bool ConvertFiles(
				const QString& inputPath,
				const QString& outputPath,
				const iprm::IParamsSet* paramsPtr = NULL) const;

private:
	I_REF(ifile::IFilePersistence, m_inputFileLoaderCompPtr);
	I_REF(ifile::IFilePersistence, m_outputFileLoaderCompPtr);
	I_REF(iproc::IProcessor, m_processorCompPtr);
	I_REF(istd::IChangeable, m_inputDataCompPtr);
	I_REF(istd::IChangeable, m_outputDataCompPtr);
	I_REF(ibase::IProgressManager, m_progressManagerCompPtr);
	I_REF(iprm::IParamsSet, m_processingParamsSetCompPtr);

	int m_progressSessionId;

	mutable QMutex m_mutex;
};


} // namespace ifileproc


#endif // !ifileproc_CFileConvertOverProcessorComp_included


