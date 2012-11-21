#ifndef ifileproc_CProcessingResultsReviewComp_included
#define ifileproc_CProcessingResultsReviewComp_included


// ACF includes
#include "ifile/IFileNameParam.h"
#include "iser/IFileLoader.h"
#include "ibase/IFileConvertCopy.h"
#include "ibase/TLoggerCompWrap.h"

// ACF-Solutions includes
#include "iproc/ISupplier.h"


class iser::IArchive;


namespace ifileproc
{

class CProcessingResultsReviewComp:
			public ibase::CLoggerComponentBase,
			virtual public ibase::IFileConvertCopy			
{
public:
	typedef ibase::CLoggerComponentBase BaseClass;

	I_BEGIN_COMPONENT(CProcessingResultsReviewComp);
		I_REGISTER_INTERFACE(ibase::IFileConvertCopy);
		I_ASSIGN(m_inputPathCompPtr, "InputPathObject", "Optional storage of current processed input (file or directory)", false, "InputPathObject");
		I_ASSIGN(m_processingInputFilePathCompPtr, "ProcessingInputFilePath", "Full path to currently processing file, should be used by supplier as input file path", true, "ProcessingInputFilePath");
		I_ASSIGN(m_outputSupplierCompPtr, "OutputSupplier", "Supplier to process files", true, "OutputSupplier");
		I_ASSIGN_TO(m_outputSupplierSerializerCompPtr, m_outputSupplierCompPtr, true);
		I_ASSIGN(m_outputFileSerializerCompPtr, "OutputFileSerializer", "Output formatter", true, "OutputFileSerializer");		
	I_END_COMPONENT;

	// reimplemented (ibase::IFileConvertCopy)
	virtual bool ConvertFiles(				
				const QString& inputPath,
				const QString& outputPath,
				const iprm::IParamsSet* paramsPtr = NULL) const;

protected:
	/**
		Process single file and send the results to archive.
	*/
	virtual bool ProcessSingleFile(const QString& filePath, iser::IArchive& archive);

private:
	class CProcessSerializer: public iser::ISerializable
	{
	public:
		/**
			Initialize the serializer with parent object and path to be processed.
		*/
		CProcessSerializer(const CProcessingResultsReviewComp* parentPtr, const QString& path);

		// reimplemented (iser::ISerializable)
		virtual bool Serialize(iser::IArchive& archive);

	private:
		const CProcessingResultsReviewComp& m_parent;
		QString m_path;
	};

	I_REF(ifile::IFileNameParam, m_inputPathCompPtr);
	I_REF(ifile::IFileNameParam, m_processingInputFilePathCompPtr);
	I_REF(iproc::ISupplier, m_outputSupplierCompPtr);
	I_REF(iser::ISerializable, m_outputSupplierSerializerCompPtr);
	I_REF(iser::IFileLoader, m_outputFileSerializerCompPtr);	
};


} // namespace ifileproc


#endif // !ifileproc_CProcessingResultsReviewComp_included


