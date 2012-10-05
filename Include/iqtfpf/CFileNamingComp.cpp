#include "iqtfpf/CFileNamingComp.h"


// Qt includes
#include <QtCore/QFileInfo>
#include <QtCore/QDir>

#include "iqt/iqt.h"


namespace iqtfpf
{


// public methods

// reimplemented (ifpf::IFileNaming)

QString CFileNamingComp::GetFilePath(const QString& inputFilePath) const
{
	if (!m_directoryPathCompPtr.IsValid()){
		return QString();
	}

	QFileInfo inputFileInfo(inputFilePath);
	QString baseFileName = inputFileInfo.completeBaseName();
	QString outputExtension = inputFileInfo.suffix();

	// Remove patterns to be ingnored:

	// Calculate the base file name:
	if (m_fileNamingParamsCompPtr.IsValid()){
		baseFileName = m_fileNamingParamsCompPtr->GetPrefix() + baseFileName;
		baseFileName += m_fileNamingParamsCompPtr->GetSuffix();
	}

	// Calculate the new extension:
	if (m_fileTypeInfoCompPtr.IsValid()){
		QStringList supportedExtensions;
		m_fileTypeInfoCompPtr->GetFileExtensions(supportedExtensions, iser::IFileLoader::QF_SAVE);

		QStringList::const_iterator inputFoundIter = qFind(supportedExtensions.begin(), supportedExtensions.end(), outputExtension);
		if (inputFoundIter == supportedExtensions.end()){
			if (supportedExtensions.isEmpty()){
				outputExtension.clear();
			}
			else{
				outputExtension = supportedExtensions[0];
			}
		}
	}

	QString newFileName = baseFileName;
	
	if (!outputExtension.isEmpty()){
		newFileName += QString(".") + outputExtension;
	}

	QString outputDirectoryPath = m_directoryPathCompPtr->GetPath();

	if (!outputDirectoryPath.isEmpty()){
		QDir outputDirectory(outputDirectoryPath);

		QString outputFilePath = outputDirectory.absoluteFilePath(newFileName);

		if (m_fileNamingParamsCompPtr.IsValid()){
			if (m_fileNamingParamsCompPtr->GetOverwriteStrategy() == ifpf::IFileNamingParams::RM_OVERWRITE){
				return outputFilePath;
			}
			else{
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
		}

		return outputDirectory.absoluteFilePath(newFileName);
	}

	return QString();
}


} // namespace iqtfpf


