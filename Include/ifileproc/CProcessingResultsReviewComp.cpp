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


namespace
{
	static iser::CArchiveTag processedFilesTag("ProcessedFiles", "ProcessedFiles");
	static iser::CArchiveTag fileTag("File", "File");
	static iser::CArchiveTag filePathTag("FilePath", "FilePath");
	static iser::CArchiveTag workStatusTag("WorkStatus", "WorkStatus");
	static iser::CArchiveTag outputDataTag("OutputData", "OutputData");
}


// reimplemented (ifileproc::IFileConvertCopy)

bool CProcessingResultsReviewComp::ConvertFiles(
			const QString& inputPath,
			const QString& outputPath,
			const iprm::IParamsSet* /*paramsPtr*/) const
{
	if (		!m_currentProcessedFilePathCompPtr.IsValid() ||
				!m_outputSupplierCompPtr.IsValid() ||
				!m_outputSupplierSerializerCompPtr.IsValid() ||
				!m_outputFileSerializerCompPtr.IsValid()){
		SendErrorMessage(0, "Bad component definition");

		return false;
	}

	if (m_inputPathCompPtr.IsValid()){
		m_inputPathCompPtr->SetPath(inputPath);
	}

	if (IsVerboseEnabled()){
		SendVerboseMessage(QObject::tr("Processing input '%1' to output '%2'").arg(inputPath).arg(outputPath));
	}

	ProcessSerializer processSerializer(this, inputPath);

	if (m_outputFileSerializerCompPtr->SaveToFile(processSerializer, outputPath) == ifile::IFilePersistence::OS_OK){
		return true;
	}

	return false;
}


// protected methods

bool CProcessingResultsReviewComp::ProcessSingleFile(const QString& filePath, iser::IArchive& archive)
{
	Q_ASSERT(archive.IsStoring());

	if (IsVerboseEnabled()){
		SendVerboseMessage(QObject::tr("File to process %1").arg(filePath));
	}

	bool retVal = true;

	m_currentProcessedFilePathCompPtr->SetPath(filePath);

	m_outputSupplierCompPtr->ClearWorkResults();
	m_outputSupplierCompPtr->EnsureWorkInitialized();
	m_outputSupplierCompPtr->EnsureWorkFinished();

	int workStatus = m_outputSupplierCompPtr->GetWorkStatus();

	retVal = retVal && archive.BeginTag(fileTag);

	retVal = retVal && archive.BeginTag(filePathTag);
	retVal = retVal && archive.Process(const_cast<QString&>(filePath));
	retVal = retVal && archive.EndTag(filePathTag);

	retVal = retVal && archive.BeginTag(workStatusTag);
	retVal = retVal && archive.Process(workStatus);
	retVal = retVal && archive.EndTag(workStatusTag);

	if (workStatus == iinsp::ISupplier::WS_OK){
		retVal = retVal && archive.BeginTag(outputDataTag);
		retVal = retVal && m_outputSupplierSerializerCompPtr->Serialize(archive);
		retVal = retVal && archive.EndTag(outputDataTag);
	}

	retVal = retVal && archive.EndTag(fileTag);

	return retVal;
}


// public methods of embedded class ProcessSerializer

CProcessingResultsReviewComp::ProcessSerializer::ProcessSerializer(
			const CProcessingResultsReviewComp* parentPtr,
			const QString& path)
:	m_parent(*parentPtr),
	m_path(path)
{
	Q_ASSERT(parentPtr != NULL);
}


// reimplemented (iser::ISerializable)

bool CProcessingResultsReviewComp::ProcessSerializer::Serialize(iser::IArchive& archive)
{
	if (!archive.IsStoring()){
		return false;
	}

	bool retVal = true;

	QFileInfo fileInfo(m_path);

	if (fileInfo.isFile()){
		int count = 1;
		retVal = archive.BeginMultiTag(processedFilesTag, fileTag, count);

		retVal = retVal && const_cast<CProcessingResultsReviewComp&>(m_parent).ProcessSingleFile(m_path, archive) && retVal;

		retVal = retVal && archive.EndTag(processedFilesTag);

		return retVal;
	}
	else{
		QStringList filters;
		QDir dir(".");
		if (fileInfo.isDir()){
			dir = QDir(m_path);

			if (m_parent.m_inputFileTypeInfoCompPtr.IsValid()){
				QStringList extensions;
				m_parent.m_inputFileTypeInfoCompPtr->GetFileExtensions(extensions);

				for (		QStringList::ConstIterator extensionIter = extensions.begin();
							extensionIter != extensions.begin();
							++extensionIter){
					const QString& extension = *extensionIter;
					filters += "*." + extension;
				}
			}
			else{
				filters += "*";
			}
		}
		else if (m_path.contains("*") || m_path.contains("?")){
			int lastSlash = qMax(m_path.lastIndexOf('/'), m_path.lastIndexOf('\\'));
			if (lastSlash > 0){
				filters += m_path.mid(lastSlash + 1);

				dir = QDir(m_path.left(lastSlash));
			}
			else{
				filters += m_path;
			}
		}

		if (!dir.exists()){
			m_parent.SendErrorMessage(MI_INPUT_PATH, QObject::tr("Directory %1 not exists").arg(dir.canonicalPath()));

			return false;
		}

		QStringList fileList = dir.entryList(filters, QDir::Files, QDir::Name);

		int count = fileList.count();

		retVal = archive.BeginMultiTag(processedFilesTag, fileTag, count);

		for(		QStringList::const_iterator iter = fileList.constBegin();
					iter != fileList.constEnd();
					++iter){
			QString filePath = dir.filePath(*iter);

			if (!retVal){
				return false;
			}

			retVal = retVal && const_cast<CProcessingResultsReviewComp&>(m_parent).ProcessSingleFile(filePath, archive);
		}

		retVal = retVal && archive.EndTag(processedFilesTag);

		return retVal;		
	}

	return false;
}


} // namespace ifileproc


