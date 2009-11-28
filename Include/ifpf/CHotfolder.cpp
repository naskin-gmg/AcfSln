#include "ifpf/CHotfolder.h"


// ACF includes
#include "istd/TChangeNotifier.h"

#include "iser/IArchive.h"
#include "iser/CArchivetag.h"

#include "iproc/IProcessor.h"


namespace ifpf
{


// public methods

CHotfolder::CHotfolder()
	:m_isWorking(false),
	m_paramsSetPtr(NULL)
{
}

void CHotfolder::SetFileState(const istd::CString& fileName, int state)
{
	for (FileItems::iterator index = m_fileItems.begin(); index != m_fileItems.end(); index++){
		if ((*index).filePath == fileName){
			if ((*index).fileState != state){
				istd::CChangeNotifier changePtr(this, CF_FILE_STATE_CHANGED);
				
				(*index).fileState = state;
			}
		}
	}
}


void CHotfolder::RemoveFile(const istd::CString& fileName)
{
	for (FileItems::iterator index = m_fileItems.begin(); index != m_fileItems.end(); index++){
		if ((*index).filePath == fileName){
			istd::CChangeNotifier changePtr(this, CF_FILE_REMOVED);

			m_fileItems.erase(index);
			
			break;
		}
	}
}


void CHotfolder::AddFile(const istd::CString& fileName, int state)
{
	FileItem newFileItem;
	newFileItem.filePath = fileName;
	newFileItem.fileState = state;

	istd::CChangeNotifier changePtr(this, CF_FILE_ADDED);

	m_fileItems.push_back(newFileItem);
}


void CHotfolder::SetParams(iprm::IParamsSet* paramsSet)
{
	// Set params set only by initialization:
	I_ASSERT(m_paramsSetPtr == NULL);
	// Parameter validation:
	I_ASSERT(paramsSet != NULL);

	m_paramsSetPtr = paramsSet;
}


// reimplemented (ifpf::IHotfolderInfo)

int CHotfolder::GetFileState(const istd::CString& fileName) const
{
	for (FileItems::const_iterator index = m_fileItems.begin(); index != m_fileItems.end(); index++){
		if ((*index).filePath == fileName){
			return (*index).fileState; 
		}
	}

	return iproc::IProcessor::TS_NONE;
}


bool CHotfolder::GetWorking() const
{
	return m_isWorking;
}

void CHotfolder::SetWorking(bool working)
{
	if (working != m_isWorking){
		istd::CChangeNotifier changePtr(this);
	
		m_isWorking = m_isWorking;
	}
}


iprm::IParamsSet* CHotfolder::GetHotfolderParams() const
{
	return m_paramsSetPtr;
}


// reimplemented (ibase::IFileListProvider)

istd::CStringList CHotfolder::GetFileList() const
{
	istd::CStringList files;

	for (FileItems::const_iterator index = m_fileItems.begin(); index != m_fileItems.end(); index++){
		files.push_back((*index).filePath);
	}

	return files;
}


// reimplemented (iser::ISerializable)

bool CHotfolder::Serialize(iser::IArchive& archive)
{
	return true;
}


istd::CStringList CHotfolder::GetFilesForState(int state) const
{
	istd::CStringList files;

	for (FileItems::const_iterator index = m_fileItems.begin(); index != m_fileItems.end(); index++){
		if ((*index).fileState == state){
			files.push_back((*index).filePath); 
		}
	}

	return files;
}


} // namespace ifpf

