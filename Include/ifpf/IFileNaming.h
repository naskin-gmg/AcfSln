#ifndef ifpf_IFileNaming_included
#define ifpf_IFileNaming_included


// ACF includes
#include "istd/IChangeable.h"

#include "iprm/IParamsSet.h"


namespace ifpf
{


/**
	Interface for calculation of the new file path for an existing file.
*/
class IFileNaming: virtual public istd::IChangeable
{
public:
	/**
		Get the new file path for a given input according to the parameters \c paramsSetPtr.
		\c paramsSetPtr can be \c NULL. It means, that some default parameters will be used to calculate the output.
		\sa isys::IFileSystem
	*/
	virtual istd::CString GetFilePath(const istd::CString& inputFileName, const iprm::IParamsSet* paramsSetPtr) const = 0;
};


} // namespace ifpf


#endif // !ifpf_IFileNaming_included


