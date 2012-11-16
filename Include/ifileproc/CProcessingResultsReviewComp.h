#ifndef ifileproc_CProcessingResultsReviewComp_included
#define ifileproc_CProcessingResultsReviewComp_included


// ACF includes
//#include "icomp/CComponentBase.h"
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
		I_ASSIGN(m_inputPathObjectPtr, "InputPathObject", "Path to files", true, "InputPathObject");
		I_ASSIGN(m_processingInputFilePathPtr, "ProcessingInputFilePath", "Full path to currently processing file", true, "ProcessingInputFilePath");
		I_ASSIGN(m_outputSupplierPtr, "OutputSupplier", "Supplier to process files", true, "OutputSupplier");
		I_ASSIGN_TO(m_outputSupplierSerializerPtr, m_outputSupplierPtr, true);		
		I_ASSIGN(m_outputFileSerializerPtr, "OutputFileSerializer", "Output formatter", true, "OutputFileSerializer");		
	I_END_COMPONENT;


	// reimplemented (ibase::IFileConvertCopy)
	virtual bool ConvertFiles(				
				const QString& inputPath,
				const QString& outputPath,
				const iprm::IParamsSet* paramsPtr = NULL) const;

private:
	class CProcessSerializer : public iser::ISerializable
	{

	public:
		CProcessSerializer(
				const CProcessingResultsReviewComp* parent);

		// reimplemented (iser::ISerializable)
		virtual bool Serialize(iser::IArchive& archive);

	protected:
		const CProcessingResultsReviewComp* m_parentPtr;
	};	
	
	I_REF(ifile::IFileNameParam, m_inputPathObjectPtr);
	I_REF(ifile::IFileNameParam, m_processingInputFilePathPtr);
	I_REF(iproc::ISupplier, m_outputSupplierPtr);
	I_REF(iser::ISerializable, m_outputSupplierSerializerPtr);
	I_REF(iser::IFileLoader, m_outputFileSerializerPtr);	
};


} // namespace ifileproc


#endif // !ifileproc_CProcessingResultsReviewComp_included
