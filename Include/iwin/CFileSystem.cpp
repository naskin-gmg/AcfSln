#include "iwin/CFileSystem.h"


// Windows includes
#include <windows.h>


// Undef the microsoft macros, to avoid name conflicts
#ifdef CopyFile
	#undef CopyFile
#endif


namespace iwin
{


// public methods

// reimplemented (isys::IFileSystem)
	
bool CFileSystem::IsPresent(const istd::CString& filePath) const
{
	return (_waccess(filePath.c_str(), 0) != 0);
}


bool CFileSystem::CopyFile(const istd::CString& inputFile, const istd::CString& outputFile, bool overwriteExisting) const
{
	return ::CopyFileW(inputFile.c_str(), outputFile.c_str(), !overwriteExisting);
}


} // namespace iwin


