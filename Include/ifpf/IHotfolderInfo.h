#ifndef ifpf_IHotfolderInfo_included
#define ifpf_IHotfolderInfo_included


// ACF includes
#include "istd/INamed.h"

#include "ibase/IFileListProvider.h"


namespace ifpf
{


/**
	Interface for a hotfolder.
*/
class IHotfolderInfo: virtual public istd::INamed, virtual ibase::IFileListProvider
{
public:
	enum FileState
	{
		FS_UNKNOWN,
		FS_PROCESSING,
		FS_CANCELED,
		FS_WAITING,
		FS_PROCESSED
	};

	/**
		Get state of a file in the hotfolder.
	*/
	virtual int GetFileState(const istd::CString& fileName) const = 0;
};


} // namespace ifpf


#endif // !ifpf_IHotfolderInfo_included


