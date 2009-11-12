#ifndef ifpf_IDirectoryMonitorParams_included
#define ifpf_IDirectoryMonitorParams_included


// ACF includes
#include "iprm/IFileNameParam.h"


namespace ifpf
{


/**
	Interface for a folder monitor parameters.
*/
class IDirectoryMonitorParams: virtual public iprm::IFileNameParam
{
public:
	enum ObserveItems
	{
		OI_DIR = 0x1,
		OI_FILES = 0x2,
		OI_DRIVES = 0x4,
		OI_ALL = OI_DIR | OI_FILES | OI_DRIVES
	};

	enum ObserveChanges
	{
		OC_ADD = 0x1,
		OC_REMOVE = 0x2,
		OC_MODIFIED = 0x4,
		OC_ATTR_CHANGED = 0x8,
		OC_ALL = OC_ADD | OC_REMOVE | OC_MODIFIED | OC_ATTR_CHANGED
	};

	/**
		Get pooling intervall for directory changes in seconds.	
	*/
	virtual double GetPoolingIntervall() const = 0;

	/**
		Set pooling intervall for directory changes in seconds.
	*/
	virtual void SetPoolingIntervall(double poolingIntervall) = 0;

	/**
		Get the group of items to be observed.
	*/
	virtual int GetObservedItemTypes() const = 0;

	/**
		Set the group of items to be observed
	*/
	virtual void SetObservedItemTypes(int observedItemTypes) = 0;

	/**
		Get the group of changes to be observed.
	*/
	virtual int GetObservedChanges() const = 0;

	/**
		Set the changes to be observed.
	*/
	virtual void SetObservedChanges(int ovservedChanges) = 0;

	/**
		Get file filters, given as the list of regular expressions.
	*/
	virtual istd::CStringList GetFileFilters() const = 0;

	/**
		Set the file filters. Only file system items, that match the specified filters will be observed.
	*/
	virtual void SetFileFilters(const istd::CStringList& fileFilters) = 0;
};


} // namespace ifpf


#endif // !ifpf_IDirectoryMonitorParams_included


