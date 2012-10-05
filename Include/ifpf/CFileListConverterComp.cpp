#include "ifpf/CFileListConverterComp.h"


// Qt includes
#include <QtCore/QStringList>
#include <QtCore/QDir>

// ACF-Solutions includes
#include <ifpf/CFilePathesContainer.h>


namespace ifpf
{


// public methods

// reimplemented (iproc:IProcessor)

int CFileListConverterComp::DoProcessing(
			const iprm::IParamsSet* paramsPtr,
			const istd::IPolymorphic* inputPtr,
			istd::IChangeable* outputPtr,
			iproc::IProgressManager* progressManagerPtr)
{
	if (!m_outputFileNameCompPtr.IsValid()){
		return TS_INVALID;
	}

	const ibase::IFileListProvider* inputFileListProviderPtr = dynamic_cast<const ibase::IFileListProvider*>(inputPtr);
	if (inputFileListProviderPtr == NULL){
		return TS_INVALID;
	}

	int progressSessionId = 0;
	
	if (progressManagerPtr != NULL){
		progressSessionId = progressManagerPtr->BeginProgressSession("ConvertFiles", "Convert files", true);
	}

	int retVal = TS_OK;

	CFilePathesContainer* convertedFileListPtr = dynamic_cast<CFilePathesContainer*>(outputPtr);

	QStringList fileList = inputFileListProviderPtr->GetFileList();
	int filesCount = fileList.count();

	double progressStep = 1.0 / (double)filesCount;
	
	if (convertedFileListPtr != NULL){
		convertedFileListPtr->ResetFiles();
	}

	QStringList filesToRemove;

	for (int inputFileIndex = 0; inputFileIndex < filesCount; inputFileIndex++){
		const QString& inputFile = fileList[inputFileIndex];

		const QString outputFileName = m_outputFileNameCompPtr->GetFilePath(inputFile);

		QFileInfo outputFileInfo(outputFileName);

		QDir outputDirectory(outputFileInfo.absoluteDir());
		if (!outputDirectory.mkpath(outputFileInfo.absolutePath())){
			SendErrorMessage(0, QObject::tr("Output directry doesn't exist").arg(outputFileInfo.absolutePath()));			
		}

		bool isConverted = false;

		if (m_fileConvertCompPtr.IsValid()){
			isConverted = m_fileConvertCompPtr->ConvertFile(inputFile, outputFileName, paramsPtr);
		}
		else{
			isConverted = QFile::copy(inputFile, outputFileName);
		}

		if (!isConverted){
			SendErrorMessage(0, QObject::tr("Processing of %1 failed").arg(inputFile));
		}
		else{
			if (convertedFileListPtr != NULL){
				convertedFileListPtr->InsertFile(outputFileName);
			}
		}

		if (*m_inputFilesRemovingEnabledAttrPtr){
			filesToRemove.push_back(inputFile);
		}

		if (progressManagerPtr != NULL){
			progressManagerPtr->OnProgress(progressSessionId, inputFileIndex * progressStep);

			if (progressManagerPtr->IsCanceled(progressSessionId)){
				progressManagerPtr->EndProgressSession(progressSessionId);
			
				return iproc::IProcessor::TS_CANCELED;
			}
		}
	}

	int removeFilesCount = filesToRemove.size();
	for (int fileIndex = 0; fileIndex < removeFilesCount; fileIndex++){
		QString fileToRemove = filesToRemove[fileIndex];
		if (!QFile::remove(fileToRemove)){
			SendErrorMessage(0, QObject::tr("File %1 could not be removed").arg(fileToRemove));
		}
	}

	if (progressManagerPtr != NULL){
		progressManagerPtr->EndProgressSession(progressSessionId);
	}

	return retVal;
}


} // namespace ifpf


