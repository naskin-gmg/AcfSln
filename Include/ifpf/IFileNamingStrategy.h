#ifndef ifpf_IFileNamingStrategy_included
#define ifpf_IFileNamingStrategy_included


// ACF includes
#include "istd/IChangeable.h"


namespace ifpf
{


/**
	Interface for calculation of the new file name for an existing file.
*/
class IFileNamingStrategy: virtual public istd::IChangeable
{
public:
	/**
		Get the file name (not file path) for the given input file name.
	*/
	virtual istd::CString GetFileName(const istd::CString& inputFileName) const = 0;
};


} // namespace ifpf


#endif // !ifpf_IFileNamingStrategy_included


