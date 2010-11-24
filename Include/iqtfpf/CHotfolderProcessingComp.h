#ifndef iqtfpf_CHotfolderProcessingComp_included
#define iqtfpf_CHotfolderProcessingComp_included


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

#include "ibase/IFileConvertCopy.h"
#include "ibase/TLoggerCompWrap.h"

#include "iprm/IParamsSet.h"
#include "iprm/IParamsManager.h"

#include "iqt/CCriticalSection.h"


// AcfSln includes
#include "ifpf/IFileNaming.h"
#include "ifpf/IHotfolderProcessingInfo.h"
#include "ifpf/IHotfolderStatistics.h"
#include "ifpf/IDirectoryMonitor.h"


namespace iqtfpf
{


/**
	Hotfolder representation.
*/
class CHotfolderProcessingComp: public QObject, public ibase::CLoggerComponentBase
{
	Q_OBJECT
public:
	typedef ibase::CLoggerComponentBase BaseClass;

	I_BEGIN_COMPONENT(CHotfolderProcessingComp);
		I_ASSIGN(m_fileConvertCompPtr, "FileConverter", "File converter", true, "FileConverter");
		I_ASSIGN(m_fileNamingCompPtr, "FileNamingStrategy", "Strategy for naming of the output file", true, "FileNamingStrategy");
		I_ASSIGN(m_monitorFactCompPtr, "DirectoryMontorFactory", "Factory for creation of a directory monitor", true, "DirectoryMontorFactory");
		I_ASSIGN(m_hotfolderProcessingInfoCompPtr, "HotfolderProcessingInfo", "State data model of the hotfolder", true, "HotfolderProcessingInfo");
		I_ASSIGN(m_hotfolderProcessingModelCompPtr, "HotfolderProcessingInfo", "State data model of the hotfolder", true, "HotfolderProcessingInfo");
		I_ASSIGN(m_inputDirectoriesManagerCompPtr, "InputDirectoriesManager", "Parameter's manageer for the hotfolder's input directories", true, "InputDirectoriesManager");
		I_ASSIGN(m_hotfolderSettingsModelCompPtr, "HotfolderSettingsModel", "Hotfolder settings", true, "HotfolderSettingsModel");
	I_END_COMPONENT();

	CHotfolderProcessingComp();

protected:
	/**
		Implementation of processing thread.
	*/
	class ItemProcessor: public QThread
	{
	public:
		typedef QThread BaseClass;

		ItemProcessor(
					CHotfolderProcessingComp& parent,
					const istd::CString& inputFilePath,
					const istd::CString& outputFilePath,
					const std::string& itemUuid);

		int GetProcessingState() const;
		std::string GetItemUuid() const;
		QDateTime GetStartTime() const;
		double GetProcessingTime() const;
		void Cancel();

	protected:
		// reimplemented (QThread)
		virtual void run();

	private:
		CHotfolderProcessingComp& m_parent;
		istd::CString m_inputFilePath;
		istd::CString m_outputFilePath;
		std::string m_itemUuid;
		int m_processingState;
		double m_processingTime;
		QDateTime m_startTime; 
	};

	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated();
	virtual void OnComponentDestroyed();

protected Q_SLOTS:
	void OnProcessingTimer();
	void OnProcessingItemFinished(const ItemProcessor& processor);

private:
	void OnFilesAddedEvent(const istd::CStringList& addedFiles);
	void OnFilesRemovedEvent(const istd::CStringList& removedFiles);
	void OnFilesModifiedEvent(const istd::CStringList& modifiedFiles);

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
		Get next item to be processed.
	*/
	ifpf::IHotfolderProcessingItem* GetNextProcessingFile() const;

	/**
		Find a processing item for the given file name.
	*/
	ifpf::IHotfolderProcessingItem* FindProcessingItem(const istd::CString& fileName) const;

	/**
		Cancel processing item
	*/
	void OnCancelProcessingItem(const ifpf::IHotfolderProcessingItem* processingItemPtr);

	/**
		Cancel all items.
	*/
	void CancelAllProcessingItems();

	/**
		Get procesing item from its UUID.
	*/
	ifpf::IHotfolderProcessingItem* GetItemFromId(const std::string& itemUuid) const;

private:
	/**
		Internal observer of the changes in the input directories.
	*/
	class DirectoryMonitorObserver: public imod::CMultiModelObserverBase
	{
	public:
		DirectoryMonitorObserver(CHotfolderProcessingComp& parent);

		// reimplemented (imod::IObserver)
		virtual void AfterUpdate(imod::IModel* modelPtr, int updateFlags, istd::IPolymorphic* updateParamsPtr);

	private:
		CHotfolderProcessingComp& m_parent;
	};


	/**
		Class for observation of the changes in the hotfolder parameter model
	*/
	class ParametersObserver: public imod::CSingleModelObserverBase
	{
	public:
		ParametersObserver(CHotfolderProcessingComp& parent);
		
		// reimplemented (imod::IObserver)
		virtual void AfterUpdate(imod::IModel* modelPtr, int updateFlags, istd::IPolymorphic* updateParamsPtr);
	private:
		CHotfolderProcessingComp& m_parent;
	};


	/**
		Class for observation of the changes in the hotfolder state model
	*/
	class StateObserver: public imod::CSingleModelObserverBase
	{
	public:
		typedef imod::CSingleModelObserverBase BaseClass;

		StateObserver(CHotfolderProcessingComp& parent);
		
		// reimplemented (imod::IObserver)
		virtual void AfterUpdate(imod::IModel* modelPtr, int updateFlags, istd::IPolymorphic* updateParamsPtr);
	private:
		CHotfolderProcessingComp& m_parent;
	};

private:
	I_REF(ibase::IFileConvertCopy, m_fileConvertCompPtr);
	I_REF(ifpf::IFileNaming, m_fileNamingCompPtr);
	I_REF(ifpf::IHotfolderProcessingInfo, m_hotfolderProcessingInfoCompPtr);
	I_REF(imod::IModel, m_hotfolderProcessingModelCompPtr);
	I_REF(iprm::IParamsManager, m_inputDirectoriesManagerCompPtr);
	I_REF(imod::IModel, m_hotfolderSettingsModelCompPtr);
	I_FACT(ifpf::IDirectoryMonitor, m_monitorFactCompPtr);

	typedef std::map<istd::CString, istd::TDelPtr<ifpf::IDirectoryMonitor> > DirectoryMonitorsMap;
	DirectoryMonitorsMap m_directoryMonitorsMap;

	DirectoryMonitorObserver m_directoryMonitorObserver;
	ParametersObserver m_parametersObserver;
	StateObserver m_stateObserver;

	QTimer m_processingTimer;

	typedef istd::TPointerVector<ItemProcessor> PendingProcessors;
	PendingProcessors m_pendingProcessors;

	bool m_isInitialized;
};


} // namespace iqtfpf


#endif // !iqtfpf_CHotfolderProcessingComp_included

