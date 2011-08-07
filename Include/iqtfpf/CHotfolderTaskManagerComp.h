#ifndef iqtfpf_CHotfolderTaskManagerComp_included
#define iqtfpf_CHotfolderTaskManagerComp_included


// Qt includes
#include <QThread>
#include <QTimer>
#include <QList>
#include <QDateTime>


// ACF includes
#include "istd/TPointerVector.h"

#include "imod/TModelWrap.h"
#include "imod/CMultiModelObserverBase.h"
#include "imod/CSingleModelObserverBase.h"

#include "ibase/TLoggerCompWrap.h"

#include "iprm/IParamsSet.h"
#include "iprm/IParamsManager.h"


// AcfSln includes
#include "ifpf/IFileNaming.h"
#include "ifpf/IFileSystemChangeStorage.h"
#include "ifpf/IHotfolderProcessingInfo.h"
#include "ifpf/IDirectoryMonitor.h"
#include "ifpf/IHotfolderTaskManager.h"


namespace iqtfpf
{


/**
	Processing task manager.

	This component creates and managed processing tasks for the incomming hotfolder items.
	It also manages and observes the folder-monitors, to get information about folder-activities.
*/
class CHotfolderTaskManagerComp:
			public ibase::CLoggerComponentBase,
			virtual public ifpf::IHotfolderTaskManager
{
public:
	typedef ibase::CLoggerComponentBase BaseClass;

	I_BEGIN_COMPONENT(CHotfolderTaskManagerComp);
		I_REGISTER_INTERFACE(ifpf::IHotfolderTaskManager);
		I_ASSIGN(m_fileNamingCompPtr, "FileNamingStrategy", "Strategy for naming of the output file", true, "FileNamingStrategy");
		I_ASSIGN(m_monitorFactCompPtr, "DirectoryMontorFactory", "Factory for creation of a directory monitor", true, "DirectoryMontorFactory");
		I_ASSIGN(m_hotfolderProcessingInfoCompPtr, "HotfolderProcessingInfo", "State data model of the hotfolder", true, "HotfolderProcessingInfo");
		I_ASSIGN_TO(m_hotfolderProcessingModelCompPtr, m_hotfolderProcessingInfoCompPtr, true);
		I_ASSIGN(m_inputDirectoriesManagerCompPtr, "InputDirectoriesManager", "Parameter's manageer for the hotfolder's input directories", true, "InputDirectoriesManager");
		I_ASSIGN(m_hotfolderSettingsModelCompPtr, "HotfolderSettingsModel", "Hotfolder settings", true, "HotfolderSettingsModel");
		I_ASSIGN(m_fileSystemChangeStorageCompPtr, "FileSystemChangeStorage", "File storage used by directory monitor", true, "FileSystemChangeStorage");
		I_ASSIGN_TO(m_fileSystemChangeStorageModelCompPtr, m_fileSystemChangeStorageCompPtr, true);
		I_END_COMPONENT();

	CHotfolderTaskManagerComp();

	// reimplemented (ifpf::IHotfolderTaskManager)
	virtual ifpf::IHotfolderProcessingItem* GetNextProcessingTask() const;

protected:
	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated();
	virtual void OnComponentDestroyed();

private:
	istd::CStringList GetFilesFromStorage(const ifpf::IFileSystemChangeStorage& storage, int fileState) const;

	void OnFilesAddedEvent(const ifpf::IFileSystemChangeStorage& storage);
	void OnFilesRemovedEvent(const ifpf::IFileSystemChangeStorage& storage);
	void OnFilesModifiedEvent(const ifpf::IFileSystemChangeStorage& storage);

	void AddFilesToProcessingQueue(const istd::CStringList& files);
	void RemoveFilesFromProcessingQueue(const istd::CStringList& files);
	void RestartProcessingQueueFiles(const istd::CStringList& files);

	/**
		Synchronize the hotfolder with its static parameters.
		If \c applyToPendingTasks is enabled, 
		the changes are applied to the processing item, which are already in processing pipeline.
	*/
	void SynchronizeWithModel(bool applyToPendingTasks = false);

	/**
		Get the list of output directories for the hotfolder.
	*/
	istd::CStringList GetInputDirectories() const;

	/**
		Get parameter set for the given directory path.
	*/
	const iprm::IParamsSet* GetMonitoringParamsSet(const istd::CString& directoryPath) const;

	/**
		Get list of newly added input directories.
	*/
	istd::CStringList GetAddedInputDirectories() const;

	/**
		Get list of newly removed input directories.
	*/
	istd::CStringList GetRemovedInputDirectories() const;

	/**
		Create and add a directory monitor for the given path with given monitoring parameters.
	*/
	ifpf::IDirectoryMonitor* AddDirectoryMonitor(const istd::CString& directoryPath, const iprm::IParamsSet* monitoringParamsPtr);

	/**
		Remove a directory monitor for the given path.
	*/
	void RemoveDirectoryMonitor(const istd::CString& directoryPath);

	/**
		Remove all processing items of the given directory.
	*/
	void RemoveDirectoryItems(const istd::CString& directoryPath);

	/**
		Find a processing item for the given file name.
	*/
	ifpf::IHotfolderProcessingItem* FindProcessingItem(const istd::CString& fileName) const;

	/**
		Get procesing item from its UUID.
	*/
	ifpf::IHotfolderProcessingItem* GetItemFromId(const std::string& itemUuid) const;

private:
	/**
		Internal observer of the changes in the input directories.
	*/
	class FileSystemChangeStorageObserver: public imod::CMultiModelObserverBase
	{
	public:
		FileSystemChangeStorageObserver(CHotfolderTaskManagerComp& parent);

		// reimplemented (imod::IObserver)
		virtual void AfterUpdate(imod::IModel* modelPtr, int updateFlags, istd::IPolymorphic* updateParamsPtr);

	private:
		CHotfolderTaskManagerComp& m_parent;
	};


	/**
		Class for observation of the changes in the hotfolder parameter model
	*/
	class ParametersObserver: public imod::CSingleModelObserverBase
	{
	public:
		ParametersObserver(CHotfolderTaskManagerComp& parent);
		
		// reimplemented (imod::IObserver)
		virtual void AfterUpdate(imod::IModel* modelPtr, int updateFlags, istd::IPolymorphic* updateParamsPtr);
	private:
		CHotfolderTaskManagerComp& m_parent;
	};

private:
	I_REF(ifpf::IFileNaming, m_fileNamingCompPtr);
	I_REF(ifpf::IHotfolderProcessingInfo, m_hotfolderProcessingInfoCompPtr);
	I_REF(imod::IModel, m_hotfolderProcessingModelCompPtr);
	I_REF(iprm::IParamsManager, m_inputDirectoriesManagerCompPtr);
	I_REF(imod::IModel, m_hotfolderSettingsModelCompPtr);
	I_FACT(ifpf::IDirectoryMonitor, m_monitorFactCompPtr);
	I_REF(ifpf::IFileSystemChangeStorage, m_fileSystemChangeStorageCompPtr);
	I_REF(imod::IModel, m_fileSystemChangeStorageModelCompPtr);

	typedef std::map<istd::CString, istd::TDelPtr<ifpf::IDirectoryMonitor> > DirectoryMonitorsMap;
	DirectoryMonitorsMap m_directoryMonitorsMap;

	FileSystemChangeStorageObserver m_fileSystemChangeStorageObserver;
	ParametersObserver m_parametersObserver;

	bool m_isInitialized;
};


} // namespace iqtfpf


#endif // !iqtfpf_CHotfolderTaskManagerComp_included

