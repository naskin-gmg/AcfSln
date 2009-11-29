#ifndef ifpf_IHotfolder_included
#define ifpf_IHotfolder_included


// ACF includes
#include "istd/INamed.h"

#include "ibase/IFileListProvider.h"

#include "iprm/IParamsSet.h"


namespace ifpf
{


/**
	Interface for a hotfolder's dynamic model.
*/
class IHotfolder: virtual public ibase::IFileListProvider
{
public:
	enum ChangeFlags
	{
		CF_FILE_ADDED = 0x100000,
		CF_FILE_REMOVED = 0x200000,
		CF_FILE_STATE_CHANGED = 0x400000
	};
	
	/**
		Get state of a file in the hotfolder.
		\sa iproc::IProcessor::TaskState
	*/
	virtual int GetFileState(const istd::CString& fileName) const = 0;

	/**
		Get the working state of the hotfolder.
	*/
	virtual bool GetWorking() const = 0;

	/**
		If \c working is \c true, the hotfolder processing will be started or stopped otherwise.
	*/
	virtual void SetWorking(bool working = true) = 0;

	/**
		Get the hotfolder's parameters
	*/
	virtual iprm::IParamsSet* GetHotfolderParams() const = 0;
};


} // namespace ifpf


#endif // !ifpf_IHotfolder_included


