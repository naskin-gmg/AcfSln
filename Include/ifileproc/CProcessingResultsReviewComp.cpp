#include "ifileproc/CProcessingResultsReviewComp.h"


// Qt includes
#include <QStringList>
#include <QDir>
#include <QFile>
#include <QFileInfo>

// ACF includes
#include "istd/TChangeNotifier.h"

#include "iser/IArchive.h"
#include "iser/CArchiveTag.h"


namespace ifileproc
{

// reimplemented (ibase::IFileConvertCopy)

bool CProcessingResultsReviewComp::ConvertFiles(
			const QString& inputPath,
			const QString& outputPath,
			const iprm::IParamsSet* /*paramsPtr*/) const
{
	if(		!m_inputPathObjectPtr.IsValid() ||
			!m_processingInputFilePath.IsValid() ||
			!m_outputSupplierPtr.IsValid() ||
			!m_outputFileSerializerPtr.IsValid()){

		return false;
	}

	m_inputPathObjectPtr->SetPath(inputPath);

	CProcessSerializer processSerializer(this);

	if(		m_outputFileSerializerPtr->SaveToFile(
			processSerializer, outputPath) == iser::IFileLoader::StateOk){

		return true;
	}

	return false;
}

// public methods of embedded class CProcessSerializer

CProcessingResultsReviewComp::CProcessSerializer::CProcessSerializer(
			const CProcessingResultsReviewComp* parent)
:	m_parentPtr(parent)
{
}

// reimplemented (iser::ISerializable)
bool CProcessingResultsReviewComp::CProcessSerializer::Serialize(iser::IArchive& archive)
{
	static iser::CArchiveTag processedFilesTag("ProcessedFiles", "ProcessedFiles");
	static iser::CArchiveTag fileTag("File", "File");
	static iser::CArchiveTag filePathTag("FilePath", "FilePath");
	static iser::CArchiveTag workStatusTag("WorkStatus", "WorkStatus");
	static iser::CArchiveTag outputDataTag("OutputData", "OutputData");

	bool retVal = true;

	int pathType = m_parentPtr->m_inputPathObjectPtr->GetPathType();

	if(pathType == ifile::IFileNameParam::PT_FILE){
		int count = 1;
		retVal = archive.BeginMultiTag(processedFilesTag, fileTag, count);

		QString filePath = m_parentPtr->m_inputPathObjectPtr->GetPath();

		m_parentPtr->m_processingInputFilePath->SetPath(filePath);

		m_parentPtr->m_outputSupplierPtr->ClearWorkResults();
		m_parentPtr->m_outputSupplierPtr->EnsureWorkInitialized();
		m_parentPtr->m_outputSupplierPtr->EnsureWorkFinished();

		int workStatus = m_parentPtr->m_outputSupplierPtr->GetWorkStatus();

		retVal = retVal && archive.BeginTag(fileTag);

		retVal = retVal && archive.BeginTag(filePathTag);
		retVal = retVal && archive.Process(filePath);
		retVal = retVal && archive.EndTag(filePathTag);

		retVal = retVal && archive.BeginTag(workStatusTag);
		retVal = retVal && archive.Process(workStatus);
		retVal = retVal && archive.EndTag(workStatusTag);

		if(workStatus == iproc::ISupplier::WS_OK){

			retVal = retVal && archive.BeginTag(outputDataTag);
			retVal = retVal && m_parentPtr->m_outputSupplierSerializerPtr->Serialize(archive);
			retVal = retVal && archive.EndTag(outputDataTag);
		}

		retVal = retVal && archive.EndTag(fileTag);

		retVal = retVal && archive.EndTag(processedFilesTag);

		return retVal;
	}
	else if(pathType == ifile::IFileNameParam::PT_DIRECTORY){
		QDir dir(m_parentPtr->m_inputPathObjectPtr->GetPath());		
		
		QStringList fileList = dir.entryList(QDir::Files, QDir::Name);

		int count = fileList.count();

		retVal = archive.BeginMultiTag(processedFilesTag, fileTag, count);

		for(		QStringList::const_iterator iter = fileList.constBegin();
					iter != fileList.constEnd();
					++iter){

			QString filePath = dir.filePath(*iter);
			
			m_parentPtr->m_processingInputFilePath->SetPath(filePath);

			m_parentPtr->m_outputSupplierPtr->ClearWorkResults();
			m_parentPtr->m_outputSupplierPtr->EnsureWorkInitialized();
			m_parentPtr->m_outputSupplierPtr->EnsureWorkFinished();

			int workStatus = m_parentPtr->m_outputSupplierPtr->GetWorkStatus();

			retVal = retVal && archive.BeginTag(fileTag);

			retVal = retVal && archive.BeginTag(filePathTag);
			retVal = retVal && archive.Process(filePath);
			retVal = retVal && archive.EndTag(filePathTag);

			retVal = retVal && archive.BeginTag(workStatusTag);
			retVal = retVal && archive.Process(workStatus);
			retVal = retVal && archive.EndTag(workStatusTag);

			if(workStatus == iproc::ISupplier::WS_OK){

				retVal = retVal && archive.BeginTag(outputDataTag);
				retVal = retVal && m_parentPtr->m_outputSupplierSerializerPtr->Serialize(archive);
				retVal = retVal && archive.EndTag(outputDataTag);
			}

			retVal = retVal && archive.EndTag(fileTag);
		}

		retVal = retVal && archive.EndTag(processedFilesTag);

		return retVal;		
	}

	return false;
}

} // namespace ifileproc


