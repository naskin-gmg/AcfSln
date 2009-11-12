#include "iqtfpf/CHotfolderComp.h"


// ACF includes
#include "istd/CStaticServicesProvider.h"

#include "isys/IFileSystem.h"

#include "iprm/IFileNameParam.h"


namespace iqtfpf
{


// public methods

// reimplemented (ifpf::IFileNamingStrategy)

istd::CString CHotfolderComp::GetFileName(const istd::CString& inputFileName) const
{
	std::string paramId;
	if (m_outputDirectoryIdAttrPtr.IsValid()){
		paramId = (*m_outputDirectoryIdAttrPtr).ToString();
	}

	const iprm::IFileNameParam* outputDirectoryPtr = dynamic_cast<const iprm::IFileNameParam*>(m_paramsSetCompPtr->GetParameter(paramId));
	if (outputDirectoryPtr != NULL && outputDirectoryPtr->GetPathType() == iprm::IFileNameParam::PT_DIRECTORY){
		isys::IFileSystem* fileSystemPtr = istd::GetService<isys::IFileSystem>();
		if (fileSystemPtr != NULL){
			istd::CString fileName = fileSystemPtr->GetFileName(inputFileName);

			return outputDirectoryPtr->GetPath() + "/" + fileName;
		}
	}

	return istd::CString();
}


// reimplemented (imod::IObserver)
	
void CHotfolderComp::AfterUpdate(imod::IModel* /* modelPtr*/, int updateFlags, istd::IPolymorphic* /*updateParamsPtr*/)
{
	if (!m_fileConvertCompPtr.IsValid()){
		SendErrorMessage(0, "File conversion component was not set", "Hotfolder");

		return;
	}

	if (!m_fileNamingStrategyCompPtr.IsValid()){
		SendErrorMessage(0, "File naming component was not set", "Hotfolder");

		return;
	}

	ifpf::IDirectoryMonitor* objectPtr = GetObjectPtr();
	if (objectPtr != NULL && updateFlags & ifpf::IDirectoryMonitor::CF_FILES_ADDED){
		istd::CStringList files = objectPtr->GetChangedFileItems(ifpf::IDirectoryMonitor::CF_FILES_ADDED);

		for (int fileIndex = 0; fileIndex < int(files.size()); fileIndex++){
			const istd::CString& inputFilePath = files[fileIndex];

			isys::IFileSystem* fileSystemPtr = istd::GetService<isys::IFileSystem>();
			if (fileSystemPtr != NULL){
				if (!fileSystemPtr->IsPresent(inputFilePath)){
					continue;
				}
			}

			istd::CString outputFilePath = m_fileNamingStrategyCompPtr->GetFileName(inputFilePath);

			istd::CString message = istd::CString("Process file: ") + outputFilePath;
			SendInfoMessage(0, message, "Hotfolder");

			if (!m_fileConvertCompPtr->CopyFile(inputFilePath, outputFilePath, m_paramsSetCompPtr.GetPtr())){
				istd::CString message = istd::CString("Processing of ") + outputFilePath + "failed";
		
				SendErrorMessage(0, message, "Hotfolder");
			}
		}
	}
}


} // namespace iqtfpf


