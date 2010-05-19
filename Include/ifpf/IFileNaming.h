#ifndef ifpf_IFileNaming_included
#define ifpf_IFileNaming_included


// ACF includes
#include "istd/IPolymorphic.h"


namespace ifpf
{


/**
	Interface for calculation of the new file path for an existing file.
*/
class IFileNaming: virtual public istd::IPolymorphic
{
public:
	/**
		Get the new file path for a given input file name.
		\sa isys::IFileSystem
	*/
	virtual istd::CString GetFilePath(const istd::CString& inputFileName) const = 0;
};


} // namespace ifpf


#endif // !ifpf_IFileNaming_included


