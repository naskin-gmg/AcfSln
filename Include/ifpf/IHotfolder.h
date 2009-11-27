#ifndef ifpf_IHotfolder_included
#define ifpf_IHotfolder_included


// ACF includes
#include "istd/INamed.h"

#include "ibase/IFileListProvider.h"


namespace ifpf
{


/**
	Interface for a hotfolder's dynamic model.
*/
class IHotfolder: virtual ibase::IFileListProvider
{
public:
	/**
		Get state of a file in the hotfolder.
		\sa iproc::IProcessor::TaskState
	*/
	virtual int GetFileState(const istd::CString& fileName) const = 0;

	/**
		Get state of the hotfolder
		\sa iproc::IProcessor::TaskState
	*/
	virtual int GetHotfolderState() const = 0;
};


} // namespace ifpf


#endif // !ifpf_IHotfolder_included


