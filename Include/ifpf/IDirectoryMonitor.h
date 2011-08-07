#ifndef ifpf_IDirectoryMonitor_included
#define ifpf_IDirectoryMonitor_included


// ACF includes
#include "istd/IPolymorphic.h"
#include "istd/CString.h"

#include "iprm/IParamsSet.h"


namespace ifpf
{


/**
	Interface for a folder monitor.
	Observers of this interface are notified about all changes made in the monitored directory, 
	such as removing or adding files, changes in file contents or file attributes.
*/
class IDirectoryMonitor: virtual public istd::IPolymorphic
{
public:
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


