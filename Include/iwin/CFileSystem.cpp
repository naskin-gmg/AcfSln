#include "iwin/CFileSystem.h"


// Windows includes
#include <windows.h>


namespace iwin
{


// public methods

// reimplemented (isys::IFileSystem)
	
bool CFileSystem::IsPresent(const istd::CString& filePath) const
{
	return (_waccess(filePath.c_str(), 0) == 0);
}


bool CFileSystem::CreateFileCopy(const istd::CString& inputFile, const istd::CString& outputFile, bool overwriteExisting) const
{
	return (::CopyFileW(inputFile.c_str(), outputFile.c_str(), overwriteExisting? FALSE: TRUE) == TRUE);
}


bool CFileSystem::RemoveFile(const istd::CString& filePath) const
{
	return (::DeleteFileW(filePath.c_str()) == TRUE);
}


bool CFileSystem::RemoveFolder(const istd::CString& directoryPath, bool ignoreNonEmpty) const
{
	if (ignoreNonEmpty){
		WIN32_FIND_DATAW fileFindData;

		istd::CString searchString = directoryPath + "\\*";

		HANDLE fileHandle = FindFirstFileW(searchString.c_str(), &fileFindData);

		do{
			istd::CString foundFileName = fileFindData.cFileName;

			if (foundFileName != "." && foundFileName != ".."){
				if ((fileFindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0){
					RemoveFolder(foundFileName.c_str(), true);
				}
				else{
					DeleteFileW(foundFileName.c_str());
				}
			}
		}
		while (FindNextFileW(fileHandle, &fileFindData));
		
		FindClose(fileHandle);
	}

	return (::RemoveDirectoryW(directoryPath.c_str()) == TRUE);
}


bool CFileSystem::CreateFolder(const istd::CString& directoryPath) const
{
	if (IsPresent(directoryPath)){
		return true;
	}

	return (::CreateDirectoryW(directoryPath.c_str(), NULL) == TRUE);
}


} // namespace iwin


