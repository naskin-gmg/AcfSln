#ifndef ifpf_IDirectoryMonitor_included
#define ifpf_IDirectoryMonitor_included


// ACF includes
#include "istd/IChangeable.h"
#include "istd/CString.h"
#include "iprm/IParamsSet.h"

#include "ibase/IFileListProvider.h"


namespace ifpf
{


/**
	Interface for a folder monitor.
	Observers of this interface are notified about all changes made in the monitored directory, 
	such as removing or adding files, changes in file contents or file attributes.
*/
class IDirectoryMonitor: virtual public ibase::IFileListProvider
{
public:
	/**
		File system change flags.
	*/
	enum ChangeFlags
	{
		/**
			File(s) were added.
		*/
		CF_FILES_ADDED = 0x1000,

		/**
			File(s) were removed.
		*/
		CF_FILES_REMOVED = 0x2000,

		/**
			File(s) were modified.
		*/
		CF_FILES_MODIFIED = 0x4000,

		/**
			File's attributes were changed.
		*/
		CF_FILES_ATTRIBUTE_CHANGED = 0x8000,

		/**
			All possible changes
		*/
		CF_SOME_CHANGES = CF_FILES_ADDED | CF_FILES_REMOVED | CF_FILES_MODIFIED | CF_FILES_ATTRIBUTE_CHANGED
	};

	/**
		Get the list of changed files according to the given flags
		\sa ChangeFlags
	*/
	virtual istd::CStringList GetChangedFileItems(int changeFlags) const = 0;

	/**
		Start observing process.
		If the \c paramsSetPtr is not equal NULL, the observing parameter are get from this params set,
		otherwise, default parameters are using.
	*/
	virtual bool StartObserving(const iprm::IParamsSet* paramsSetPtr = NULL) = 0;

	/**
		Stop observing process.
	*/
	virtual void StopObserving() = 0;
};


} // namespace ifpf


#endif // !ifpf_IDirectoryMonitor_included


