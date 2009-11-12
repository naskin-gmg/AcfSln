#ifndef ifpf_IDirectoryMonitor_included
#define ifpf_IDirectoryMonitor_included


// ACF includes
#include "istd/IChangeable.h"
#include "istd/CString.h"

#include "ibase/IFileListProvider.h"


namespace ifpf
{


/**
	Interface for a folder monitor.
	Observers of this interface are notified about all changes made in all monitored directories, 
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
		CF_FILES_ATTRIBUTE_CHANGED = 0x8000
	};

	/**
		Get the list of changed files according to the given flags
		\sa ChangeFlags
	*/
	virtual istd::CStringList GetChangedFileItems(int changeFlags) const = 0;
};


} // namespace ifpf


#endif // !ifpf_IDirectoryMonitor_included


