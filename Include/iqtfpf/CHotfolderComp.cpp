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


// protected methods

bool CHotfolderComp::OnIncommingInputFileEvent(const ifpf::IDirectoryMonitor& directoryMonitor)
{
	if (!m_fileConvertCompPtr.IsValid()){
		SendErrorMessage(0, "File conversion component was not set", "Hotfolder");

		return false;
	}

	if (!m_fileNamingStrategyCompPtr.IsValid()){
		SendErrorMessage(0, "File naming component was not set", "Hotfolder");

		return false;
	}

	istd::CStringList files = directoryMonitor.GetChangedFileItems(ifpf::IDirectoryMonitor::CF_FILES_ADDED);

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

	return true;
}


// public methods of embedded class DirectoryMonitorObserver

CHotfolderComp::DirectoryMonitorObserver::DirectoryMonitorObserver(CHotfolderComp& parent)
	:m_parent(parent)
{
}


// reimplemented (imod::IObserver)
	
void CHotfolderComp::DirectoryMonitorObserver::AfterUpdate(imod::IModel* modelPtr, int updateFlags, istd::IPolymorphic* /*updateParamsPtr*/)
{
	ifpf::IDirectoryMonitor* monitorPtr = dynamic_cast<ifpf::IDirectoryMonitor*>(modelPtr);
	
	if (monitorPtr != NULL && updateFlags & ifpf::IDirectoryMonitor::CF_FILES_ADDED){
		m_parent.OnIncommingInputFileEvent(*monitorPtr);
	}
}


} // namespace iqtfpf


