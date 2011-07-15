#ifndef iwin_CFileSystem_included
#define iwin_CFileSystem_included


// ACF includes
#include "isys/CFileSystemBase.h"

#include "iwin/iwin.h"


namespace iwin
{


/**
	Implementation of file system information service for the windows plattform.
*/
class CFileSystem: virtual public isys::CFileSystemBase
{
public:
	// reimplemented (isys::IFileSystem)
	virtual bool IsPresent(const istd::CString& filePath) const;
	virtual bool CreateFileCopy(const istd::CString& inputFile, const istd::CString& outputFile, bool overwriteExisting = false) const;
	virtual bool RemoveFile(const istd::CString& filePath) const;
	virtual bool RemoveFolder(const istd::CString& directoryPath, bool ignoreNonEmpty = false) const;
	virtual bool CreateFolder(const istd::CString& directoryPath) const;
};


} // namespace iwin


#endif // !iwin_CFileSystem_included


