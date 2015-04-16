#include "ifileproc/CFileListConverterComp.h"


// Qt includes
#include <QtCore/QStringList>
#include <QtCore/QDir>

// ACF-Solutions includes
#include <ifileproc/CFilePathesContainer.h>


namespace ifileproc
{


// public methods

// reimplemented (iproc:IProcessor)

int CFileListConverterComp::DoProcessing(
			const iprm::IParamsSet* paramsPtr,
			const istd::IPolymorphic* inputPtr,
			istd::IChangeable* outputPtr,
			ibase::IProgressManager* progressManagerPtr)
{
	if (!m_outputFileNameCompPtr.IsValid()){
		return TS_INVALID;
	}

	const ifile::IFileListProvider* inputFileListProviderPtr = dynamic_cast<const ifile::IFileListProvider*>(inputPtr);
	if (inputFileListProviderPtr == NULL){
		return TS_INVALID;
	}

	int progressSessionId = 0;
	
	if (progressManagerPtr != NULL){
		progressSessionId = progressManagerPtr->BeginProgressSession("ConvertFiles", "Convert files", true);
	}

	int retVal = TS_OK;

	CFilePathesContainer* convertedFileListPtr = dynamic_cast<CFilePathesContainer*>(outputPtr);

	const QFileInfoList& fileList = inputFileListProviderPtr->GetFileList();
	int filesCount = fileList.count();

	double progressStep = 1.0 / (double)filesCount;
	
	if (convertedFileListPtr != NULL){
		convertedFileListPtr->ResetFiles();
	}

	QStringList filesToRemove;

	for (int inputFileIndex = 0; inputFileIndex < filesCount; inputFileIndex++){
		const QString& inputFile = fileList[inputFileIndex].absoluteFilePath();

		const QString outputFileName = m_outputFileNameCompPtr->CalculateFileName(inputFile, QString(), NULL);

		QFileInfo outputFileInfo(outputFileName);

		QDir outputDirectory(outputFileInfo.absoluteDir());
		if (!outputDirectory.mkpath(outputFileInfo.absolutePath())){
			SendErrorMessage(0, QObject::tr("Output directory doesn't exist").arg(outputFileInfo.absolutePath()));			
		}

		bool isConverted = false;

		if (m_fileConvertCompPtr.IsValid()){
			isConverted = (m_fileConvertCompPtr->ConvertFiles(inputFile, outputFileName, paramsPtr) == iproc::IProcessor::TS_OK);
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

	if (progressManagerPtr != NULL){
		progressManagerPtr->OnProgress(progressSessionId, 1.0);
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


} // namespace ifileproc


