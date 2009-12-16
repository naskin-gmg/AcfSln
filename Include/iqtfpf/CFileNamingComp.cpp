#include "iqtfpf/CFileNamingComp.h"


// STL includes
#include <algorithm>


// Qt includes
#include <QFileInfo>
#include <QDir>


// ACF includes
#include "iprm/IFileNameParam.h"

#include "iqt/iqt.h"


// AcfSln includes
#include "ifpf/IFileNamingStrategy.h"


namespace iqtfpf
{


// public methods

// reimplemented (ifpf::IFileNaming)

istd::CString CFileNamingComp::GetFilePath(const istd::CString& inputFilePath, const iprm::IParamsSet* paramsSetPtr) const
{
	if (paramsSetPtr == NULL){
		return inputFilePath;
	}

	I_ASSERT(m_outputDirectoryIdAttrPtr.IsValid());
	I_ASSERT(m_fileNameStrategyIdAttrPtr.IsValid());
	if (!m_outputDirectoryIdAttrPtr.IsValid() || !m_fileNameStrategyIdAttrPtr.IsValid()){
		return istd::CString();
	}

	const iprm::IFileNameParam* outputDirectoryPtr = dynamic_cast<const iprm::IFileNameParam*>(paramsSetPtr->GetParameter((*m_outputDirectoryIdAttrPtr).ToString()));
	if (outputDirectoryPtr == NULL){
		return istd::CString();
	}

	QFileInfo inputFileInfo(iqt::GetQString(inputFilePath));
	QString baseFileName = inputFileInfo.baseName();
	istd::CString outputExtension = iqt::GetCString(inputFileInfo.suffix());

	// calculate the base file name:
	const ifpf::IFileNamingStrategy* renamingStrategyPtr = dynamic_cast<const ifpf::IFileNamingStrategy*>(paramsSetPtr->GetParameter((*m_fileNameStrategyIdAttrPtr).ToString()));
	if (renamingStrategyPtr != NULL){
		baseFileName = iqt::GetQString(renamingStrategyPtr->GetPrefix()) + baseFileName;
		baseFileName += iqt::GetQString(renamingStrategyPtr->GetSuffix());
	}

	// calculate the new extension:
	if (m_fileLoaderCompPtr.IsValid()){
		istd::CStringList supportedExtensions;
		m_fileLoaderCompPtr->GetFileExtensions(supportedExtensions, iser::IFileLoader::QF_NO_LOADING);

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

	QString outputDirectoryPath = iqt::GetQString(outputDirectoryPtr->GetPath());

	if (!outputDirectoryPath.isEmpty()){
		QDir outputDirectory(outputDirectoryPath);

		QString outputFilePath = outputDirectory.absoluteFilePath(newFileName);

		if (renamingStrategyPtr != NULL){
			if (renamingStrategyPtr->GetRenamingMode() == ifpf::IFileNamingStrategy::RM_OVERWRITE){
				return iqt::GetCString(outputFilePath);
			}
			else{
				int counter = 0;
				while (true){
					QFileInfo outputFileInfo(outputFilePath);
					if (outputFileInfo.exists()){
						newFileName = QString("%1_%2").arg(++counter, 3, 10, QChar('0')).arg(newFileName);
						
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


