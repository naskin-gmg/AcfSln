#ifndef ifpf_IFileNaming_included
#define ifpf_IFileNaming_included


// ACF includes
#include "istd/IChangeable.h"


namespace ifpf
{


/**
	Interface for calculation of the new file name for an existing file.
*/
class IFileNaming: virtual public istd::IChangeable
{
public:
	/**
		Get the file path for the given file name and directory.
	*/
	virtual istd::CString GetFilePath(const istd::CString& fileName, const istd::CString& directoryPath) const = 0;
};


} // namespace ifpf


#endif // !ifpf_IFileNaming_included


