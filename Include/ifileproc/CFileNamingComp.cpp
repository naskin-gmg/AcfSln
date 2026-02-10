// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#include <ifileproc/CFileNamingComp.h>


// Qt includes
#include <QtCore/QFileInfo>
#include <QtCore/QDir>


namespace ifileproc
{


// public static methods

QString CFileNamingComp::CalculateNewFileName(
			const QString& inputFilePath,
			const QString& outputDirectoryPath,
			const QString& prefix,
			const QString& suffix,
			const QString& fileExtension,
			IFileNamingParams::OverwriteStrategy overwriteStrategy)
{
	QFileInfo inputFileInfo(inputFilePath);
	QString baseFileName = inputFileInfo.completeBaseName();
	QString outputExtension = inputFileInfo.suffix();

	if (!fileExtension.isEmpty()){
		outputExtension = fileExtension;
	}

	// Calculate the base file name:
	baseFileName = prefix + baseFileName;
	baseFileName += suffix;

	QString newFileName = baseFileName;
	
	if (!outputExtension.isEmpty()){
		newFileName += QString(".") + outputExtension;
	}

	if (!outputDirectoryPath.isEmpty()){
		QDir outputDirectory(outputDirectoryPath);

		if (overwriteStrategy == ifileproc::IFileNamingParams::RM_NUMBERING){
			QString outputFilePath = outputDirectory.absoluteFilePath(newFileName);

			int counter = 0;
			while (true){
				QFileInfo outputFileInfo(outputFilePath);
				if (outputFileInfo.exists()){
					newFileName = QString("%1_%2").arg(baseFileName).arg(++counter, 3, 10, QChar('0')) + "." + outputExtension;

					outputFilePath = outputDirectory.absoluteFilePath(newFileName);
				}
				else{
					break;
				}
			}
		}

		return outputDirectory.absoluteFilePath(newFileName);
	}

	return QString();
}


// public methods

// reimplemented (ifileproc::IFileNaming)

QString CFileNamingComp::CalculateFileName(
			const QString& inputFilePath,
			const QString& outputDirectoryPath,
			const ifileproc::IFileNamingParams* fileNamingParamsPtr) const
{
	QString targetDirectoryPath = outputDirectoryPath;

	if (targetDirectoryPath.isEmpty()){
		if (!m_directoryPathCompPtr.IsValid()){
			return QString();
		}

		targetDirectoryPath = m_directoryPathCompPtr->GetPath();
	}

	QFileInfo inputFileInfo(inputFilePath);
	QString baseFileName = inputFileInfo.completeBaseName();
	QString outputExtension = inputFileInfo.suffix();

	if (fileNamingParamsPtr == NULL){
		fileNamingParamsPtr = m_fileNamingParamsCompPtr.GetPtr();
	}

	QString prefix;
	QString suffix;
	ifileproc::IFileNamingParams::OverwriteStrategy overwriteStrategy = ifileproc::IFileNamingParams::RM_OVERWRITE;

	if (fileNamingParamsPtr != NULL){
		prefix = fileNamingParamsPtr->GetPrefix();
		suffix = fileNamingParamsPtr->GetSuffix();
		overwriteStrategy = fileNamingParamsPtr->GetOverwriteStrategy();
	}


	// Calculate the new extension:
	if (m_fileTypeInfoCompPtr.IsValid()){
		QStringList supportedExtensions;
		m_fileTypeInfoCompPtr->GetFileExtensions(supportedExtensions, NULL, ifile::IFileTypeInfo::QF_SAVE);

		QStringList::const_iterator inputFoundIter = std::find(supportedExtensions.begin(), supportedExtensions.end(), outputExtension);
		if (inputFoundIter == supportedExtensions.end()){
			if (supportedExtensions.isEmpty()){
				outputExtension.clear();
			}
			else{
				outputExtension = supportedExtensions[0];
			}
		}
	}

	return CalculateNewFileName(inputFilePath, targetDirectoryPath, prefix, suffix, outputExtension, overwriteStrategy);
}


} // namespace ifileproc


