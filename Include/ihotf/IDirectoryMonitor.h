#ifndef ihotf_IDirectoryMonitor_included
#define ihotf_IDirectoryMonitor_included


// Qt includes
#include <QtCore/QString>

// ACF includes
#include "iprm/IParamsSet.h"


namespace ihotf
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
		Checks if the folder monitoring is active.
	*/
	virtual bool IsRunning() const = 0;

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


} // namespace ihotf


#endif // !ihotf_IDirectoryMonitor_included


