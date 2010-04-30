#ifndef iqtfpf_CHotfolderProcessingComp_included
#define iqtfpf_CHotfolderProcessingComp_included


// Qt includes
#include<QThread>
#include <QTimer>


// ACF includes
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
#include "ifpf/IMonitoringSessionManager.h"
#include "ifpf/CMonitoringSession.h"


namespace iqtfpf
{


/**
	Hotfolder representation.
*/
class CHotfolderProcessingComp:
			protected QThread,
			public ibase::CLoggerComponentBase
{
	Q_OBJECT
public:
	typedef ibase::CLoggerComponentBase BaseClass;
	typedef QThread BaseClass2;

	I_BEGIN_COMPONENT(CHotfolderProcessingComp);
		I_ASSIGN(m_fileConvertCompPtr, "FileConverter", "File converter", true, "FileConverter");
		I_ASSIGN(m_fileNamingCompPtr, "FileNamingStrategy", "Strategy for naming of the output file", true, "FileNamingStrategy");
		I_ASSIGN(m_monitorFactCompPtr, "DirectoryMontorFactory", "Factory for creation of a directory monitor", true, "DirectoryMontorFactory");
		I_ASSIGN(m_hotfolderProcessingInfoCompPtr, "HotfolderStateModel", "State data model of the hotfolder", true, "HotfolderStateModel");
		I_ASSIGN(m_hotfolderProcessingModelCompPtr, "HotfolderStateModel", "State data model of the hotfolder", true, "HotfolderStateModel");
		I_ASSIGN(m_inputDirectoriesManagerCompPtr, "InputDirectoriesManager", "Parameter's manageer for the hotfolder's input directories", true, "InputDirectoriesManager");
		I_ASSIGN(m_hotfolderParamsModelCompPtr, "HotfolderParamsModel", "Hotfolder parameters", true, "HotfolderParamsModel");
	I_END_COMPONENT();

	CHotfolderProcessingComp();

	// reimplemented (icomp::IComponent)
	virtual void OnComponentCreated();
	virtual void OnComponentDestroyed();

protected:
	virtual bool OnInputFileEvent(const ifpf::IDirectoryMonitor& directoryMonitor);

	// reimplemented (QThread)
	virtual void run();

Q_SIGNALS:
	void EmitItemState(ifpf::IHotfolderProcessingItem*, int itemState);

protected Q_SLOTS:
	void OnItemState(ifpf::IHotfolderProcessingItem* itemPtr, int itemState);
	void OnUpdateQueueTimer();

private:
	/**
		Start hotfolder processing.
	*/
	void StartHotfolder();

	/**
		Stop hotfolder processing.
	*/
	void StopHotfolder();
	
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
		Get parameter set for each input directory.
	*/
	const iprm::IParamsSet* GetMonitoringParamsSet(int index) const;

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
		Get next item to be processed.
	*/
	ifpf::IHotfolderProcessingItem* GetNextProcessingFile() const;

	/**
		Process a single file.
	*/
	int ProcessFile(const istd::CString& inputFile, const istd::CString& outputFile);

	/**
		Change state of the processing item and notify all observers.
	*/
	void UpdateProcessingState(ifpf::IHotfolderProcessingItem* processingItemPtr, int processingState) const;

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
		StateObserver(CHotfolderProcessingComp& parent);
		
		// reimplemented (imod::IObserver)
		virtual void BeforeUpdate(imod::IModel* modelPtr, int updateFlags, istd::IPolymorphic* updateParamsPtr);
		virtual void AfterUpdate(imod::IModel* modelPtr, int updateFlags, istd::IPolymorphic* updateParamsPtr);
	private:
		CHotfolderProcessingComp& m_parent;
	};

	I_REF(ibase::IFileConvertCopy, m_fileConvertCompPtr);
	I_REF(ifpf::IFileNaming, m_fileNamingCompPtr);
	I_REF(ifpf::IHotfolderProcessingInfo, m_hotfolderProcessingInfoCompPtr);
	I_REF(imod::IModel, m_hotfolderProcessingModelCompPtr);
	I_REF(iprm::IParamsManager, m_inputDirectoriesManagerCompPtr);
	I_REF(imod::IModel, m_hotfolderParamsModelCompPtr);
	I_FACT(ifpf::IDirectoryMonitor, m_monitorFactCompPtr);

	typedef std::map<istd::CString, istd::TDelPtr<ifpf::IDirectoryMonitor> > DirectoryMonitorsMap;
	DirectoryMonitorsMap m_directoryMonitorsMap;

	DirectoryMonitorObserver m_directoryMonitorObserver;
	ParametersObserver m_parametersObserver;
	StateObserver m_stateObserver;

	mutable iqt::CCriticalSection m_parameterLock;
	mutable iqt::CCriticalSection m_processingQueueLock;

	bool m_finishThread;

	istd::TDelPtr<iprm::IParamsSet> m_runParameterPtr;

	typedef std::list<istd::CString> FilesQueue;

	FilesQueue m_filesQueue;

	QTimer m_filesQueueTimer;
};


} // namespace iqtfpf


#endif // !iqtfpf_CHotfolderProcessingComp_included

