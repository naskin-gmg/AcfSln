#ifndef ihotf_IDirectoryMonitorParams_included
#define ihotf_IDirectoryMonitorParams_included


// ACF includes
#include "istd/istd.h"
#include "iser/ISerializable.h"


namespace ihotf
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

	enum FileTimeStampMode
	{
		/**
			File modification time will be get from 'Last Modified'-attribute.
		*/
		FTM_MODIFIED = 0x1,

		/**
			File modification time will be get from 'Created'-attribute.
		*/
		FTM_CREATED = 0x2
	};

	I_DECLARE_FLAGS(FileTimeStampMode, FTM_MODIFIED, FTM_CREATED);

	/**
		Get pooling intervall for directory changes in seconds.	
	*/
	virtual double GetPoolingIntervall() const = 0;

	/**
		Set pooling intervall for directory changes in seconds.
	*/
	virtual void SetPoolingIntervall(double poolingIntervall) = 0;

	/**
		Get the minimal time to wait since last modification of the file before operating with this (e.g. to add the file to the list of observed files).
	*/
	virtual int GetMinLastModificationTimeDifference() const = 0;

	/**
		Set the minimal time to wait since last modification of the file before operating with this.
	*/
	virtual void SetMinLastModificationTimeDifference(int minLastModificationTimeDifference) = 0;
	
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
	virtual void SetObservedChanges(int observedChanges) = 0;

	/**
		Get accept file filters, given as the list of regular expressions.
	*/
	virtual QStringList GetAcceptPatterns() const = 0;

	/**
		Set the accept file filters. Only file system items, that match the specified filters will be observed.
	*/
	virtual void SetAcceptPatterns(const QStringList& acceptPatterns) = 0;

	/**
		Get ignore file filters, given as the list of regular expressions.
	*/
	virtual QStringList GetIgnorePatterns() const = 0;

	/**
		Set the ignore file filters. The file system items, that match the specified filters will be ignored.
	*/
	virtual void SetIgnorePatterns(const QStringList& acceptPatterns) = 0;

	/**
		Get depth of the folder monitoring. This value defines the depth of the folder tree to be monitored.
		A negative value means no limitation, 0 - defined root folder only and any other positive number - Number of sub-folders to be traversed.
	*/
	virtual int GetFolderDepth() const = 0;

	/**
		Set depth of the folder monitoring.
		\sa GetFolderDepth
	*/
	virtual void SetFolderDepth(int folderDepth) = 0;

	/**
		Get timestamp mode used for file detection and access check.
		\sa FileTimestampMode
	*/
	virtual int GetFileTimeStampMode() const = 0;

	/**
		Set timestamp mode used for file detection and access check.
		\sa FileTimestampMode
	*/
	virtual void SetFileTimeStampMode(int fileTimeStampMode) = 0;
};


} // namespace ihotf


#endif // !ihotf_IDirectoryMonitorParams_included


