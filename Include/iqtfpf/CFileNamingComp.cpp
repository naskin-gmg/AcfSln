#include "iqtfpf/CFileNamingComp.h"


// STL includes
#include <algorithm>


// Qt includes
#include <QFileInfo>
#include <QDir>

#include "iqt/iqt.h"


namespace iqtfpf
{


// public methods

// reimplemented (ifpf::IFileNaming)

istd::CString CFileNamingComp::GetFilePath(const istd::CString& inputFilePath) const
{
	if (!m_directoryPathCompPtr.IsValid()){
		return istd::CString::GetEmpty();
	}

	QFileInfo inputFileInfo(iqt::GetQString(inputFilePath));
	QString baseFileName = inputFileInfo.completeBaseName();
	istd::CString outputExtension = iqt::GetCString(inputFileInfo.suffix());

	// calculate the base file name:
	if (m_fileNamingParamsCompPtr.IsValid()){
		baseFileName = iqt::GetQString(m_fileNamingParamsCompPtr->GetPrefix()) + baseFileName;
		baseFileName += iqt::GetQString(m_fileNamingParamsCompPtr->GetSuffix());
	}

	// calculate the new extension:
	if (m_fileTypeInfoCompPtr.IsValid()){
		istd::CStringList supportedExtensions;
		m_fileTypeInfoCompPtr->GetFileExtensions(supportedExtensions, iser::IFileLoader::QF_NO_LOADING);

		istd::CStringList::const_iterator inputFoundIter = std::find(supportedExtensions.begin(), supportedExtensions.end(), outputExtension);
		if (inputFoundIter == supportedExtensions.end()){
			if (supportedExtensions.empty()){
				outputExtension.clear();
			}
			else{
				outputExtension = supportedExtensions[0];
			}
		}
	}

	QString newFileName = baseFileName + "." + iqt::GetQString(outputExtension);

	QString outputDirectoryPath = iqt::GetQString(m_directoryPathCompPtr->GetPath());

	if (!outputDirectoryPath.isEmpty()){
		QDir outputDirectory(outputDirectoryPath);

		QString outputFilePath = outputDirectory.absoluteFilePath(newFileName);

		if (m_fileNamingParamsCompPtr.IsValid()){
			if (m_fileNamingParamsCompPtr->GetRenamingMode() == ifpf::IFileNamingParams::RM_OVERWRITE){
				return iqt::GetCString(outputFilePath);
			}
			else{
				int counter = 0;
				while (true){
					QFileInfo outputFileInfo(outputFilePath);
					if (outputFileInfo.exists()){
						newFileName = QString("%1_%2").arg(baseFileName).arg(++counter, 3, 10, QChar('0')) + "." + iqt::GetQString(outputExtension);
						
						outputFilePath = outputDirectory.absoluteFilePath(newFileName);
					}
					else{
						break;
					}
				}
			}
		}

		return iqt::GetCString(outputDirectory.absoluteFilePath(newFileName));
	}

	return istd::CString();
}


} // namespace iqtfpf


