#include "ifileproc/CFileCopyOverLoaderComp.h"


// Qt includes
#include <QtCore/QStringList>
#include <QtCore/QFileInfo>


namespace ifileproc
{


// reimplemented (ifileproc::IFileConversion)

int CFileCopyOverLoaderComp::ConvertFiles(
			const QString& inputPath,
			QString& outputPath,
			const iprm::IParamsSet* /*paramsSetPtr*/,
			ibase::IProgressManager* /*progressManagerPtr*/) const
{
	if (!m_inputLoaderCompPtr.IsValid()){
		SendErrorMessage(0, QObject::tr("Input data loader is not defined"), "FileCopyOverLoader");

		return iproc::IProcessor::TS_INVALID;
	}

	if (!m_outputLoaderCompPtr.IsValid()){
		SendErrorMessage(0, QObject::tr("Output data loader is not defined"), "FileCopyOverLoader");

		return iproc::IProcessor::TS_INVALID;
	}

	if (!m_objectCompPtr.IsValid()){
		SendErrorMessage(0, QObject::tr("Data object for copy operaration is not set"));

		return iproc::IProcessor::TS_INVALID;
	}

	QString usedOutputPath = outputPath;

	if (usedOutputPath.isEmpty()){
		QStringList extensions;
		m_outputLoaderCompPtr->GetFileExtensions(extensions);

		if (extensions.isEmpty()){
			SendErrorMessage(0, QObject::tr("File extension list is empty"));
	
			return iproc::IProcessor::TS_INVALID;
		}

		int pointPos = inputPath.lastIndexOf(".");
		if (pointPos != -1){
			usedOutputPath = inputPath.left(pointPos + 1) + extensions.front();
		}
		else{
			usedOutputPath = inputPath + extensions.front();
		}
	}

	QFileInfo inputFileInfo(inputPath);
	if (!inputFileInfo.exists()){
		SendWarningMessage(0, QObject::tr("Input file %1 doesn't exist").arg(inputPath));
	}

	int loadState = m_inputLoaderCompPtr->LoadFromFile(*m_objectCompPtr, inputPath);
	if (loadState != ifile::IFilePersistence::OS_OK){
		SendErrorMessage(0, QObject::tr("Data could not be loaded from %1").arg(inputPath));

		return iproc::IProcessor::TS_INVALID;
	}

	int saveState = m_outputLoaderCompPtr->SaveToFile(*m_objectCompPtr, usedOutputPath);
	if (saveState != ifile::IFilePersistence::OS_OK){
		SendErrorMessage(0, QObject::tr("Data could not be saved to %1").arg(usedOutputPath));

		return iproc::IProcessor::TS_INVALID;
	}

	if (IsVerboseEnabled()){
		SendVerboseMessage(QObject::tr("Converted %1 to %2").arg(inputPath).arg(usedOutputPath));
	}

	return iproc::IProcessor::TS_OK;
}


} // namespace ifileproc


