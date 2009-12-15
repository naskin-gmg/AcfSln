#ifndef ifpf_IDirectoryMonitorParams_included
#define ifpf_IDirectoryMonitorParams_included


// ACF includes
#include "iser/ISerializable.h"


namespace ifpf
{


/**
	Interface for a folder monitor parameters.
*/
class IDirectoryMonitorParams: virtual public iser::ISerializable
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
		Get accept file filters, given as the list of regular expressions.
	*/
	virtual istd::CStringList GetAcceptPatterns() const = 0;

	/**
		Set the accept file filters. Only file system items, that match the specified filters will be observed.
	*/
	virtual void SetAcceptPatterns(const istd::CStringList& acceptPatterns) = 0;

	/**
		Get ignore file filters, given as the list of regular expressions.
	*/
	virtual istd::CStringList GetIgnorePatterns() const = 0;

	/**
		Set the ignore file filters. The file system items, that match the specified filters will be ignored.
	*/
	virtual void SetIgnorePatterns(const istd::CStringList& acceptPatterns) = 0;
};


} // namespace ifpf


#endif // !ifpf_IDirectoryMonitorParams_included


