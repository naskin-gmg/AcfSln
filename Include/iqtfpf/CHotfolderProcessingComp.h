#ifndef iqtfpf_CHotfolderProcessingComp_included
#define iqtfpf_CHotfolderProcessingComp_included


// Qt includes
#include<QThread>


// ACF includes
#include "ifpf/IDirectoryMonitor.h"

#include "imod/TModelWrap.h"
#include "imod/CMultiModelObserverBase.h"
#include "imod/CSingleModelObserverBase.h"

#include "ibase/IFileConvertCopy.h"
#include "ibase/TLoggerCompWrap.h"

#include "iprm/IParamsSet.h"

#include "iqt/CCriticalSection.h"


// AcfSln includes
#include "ifpf/IFileNaming.h"
#include "ifpf/IHotfolder.h"
#include "ifpf/IMonitoringSessionManager.h"
#include "ifpf/CMonitoringSession.h"


namespace iqtfpf
{


/**
	Hotfolder representation.
*/
class CHotfolderProcessingComp:
			protected QThread,
			public ibase::CLoggerComponentBase,
			virtual public ifpf::IMonitoringSessionManager
{
	Q_OBJECT
public:
	typedef ibase::CLoggerComponentBase BaseClass;
	typedef QThread BaseClass2;

	I_BEGIN_COMPONENT(CHotfolderProcessingComp);
		I_REGISTER_INTERFACE(ifpf::IMonitoringSessionManager);
		I_ASSIGN(m_fileConvertCompPtr, "FileConverter", "File converter", true, "FileConverter");
		I_ASSIGN(m_fileNamingCompPtr, "FileNamingStrategy", "Strategy for naming of the output file", true, "FileNamingStrategy");
		I_ASSIGN(m_paramsSetCompPtr, "ParamsSet", "Parameter set for the hotfolder", true, "ParamsSet");
		I_ASSIGN(m_paramsSetModelCompPtr, "ParamsSet", "Parameter set for the hotfolder", true, "ParamsSet");
		I_ASSIGN(m_monitorFactCompPtr, "DirectoryMontorFactory", "Factory for creation of a directory monitor", true, "DirectoryMontorFactory");
		I_ASSIGN(m_inputPathParamsManagerIdAttrPtr, "InputPathParamsManagerId", "Parameter set ID for the hotfolder's input directories", true, "");
		I_ASSIGN(m_outputDirectoryParamsIdAttrPtr, "OutputDirectoryParamsId", "Parameter set ID for the output directory", true, "");
		I_ASSIGN(m_monitoringParamsIdAttrPtr, "MonitoringParamsId", "Parameter set ID for the directory monitoring parameters", true, "");
		I_ASSIGN(m_hotfolderCompPtr, "HotfolderStateModel", "State data model of the hotfolder", true, "HotfolderStateModel");
	I_END_COMPONENT();

	CHotfolderProcessingComp();

	// reimplemented (ifpf::IMonitoringSessionManager)
	virtual ifpf::IMonitoringSession* GetSession(const ifpf::IDirectoryMonitor& directoryMonitor, const istd::CString& directoryPath) const;

	// reimplemented (icomp::IComponent)
	virtual void OnComponentCreated();
	virtual void OnComponentDestroyed();

protected:
	virtual bool OnInputFileEvent(const ifpf::IDirectoryMonitor& directoryMonitor);

	// reimplemented (QThread)
	virtual void run();

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
		If \c applyToPendingTasks is enabled, the 
	*/
	void SynchronizeWithModel(bool applyToPendingTasks = false);

	/**
		Serialize monitoring sessions
	*/
	bool SerializeMonitoringSession(iser::IArchive& archive);

	/**
		Get the output directory of this hotfolder
	*/
	istd::CString GetOutputDirectory() const;

	istd::CStringList GetInputDirectories() const;
	const iprm::IParamsSet* GetMonitoringParamsSet(int index) const;
	istd::CStringList GetAddedInputDirectories() const;
	istd::CStringList GetRemovedInputDirectories() const;
	ifpf::IDirectoryMonitor* AddDirectoryMonitor(const istd::CString& directoryPath, const iprm::IParamsSet* monitoringParamsPtr);
	void RemoveDirectoryMonitor(const istd::CString& directoryPath);

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

	I_REF(ibase::IFileConvertCopy, m_fileConvertCompPtr);
	I_REF(ifpf::IFileNaming, m_fileNamingCompPtr);
	I_REF(iprm::IParamsSet, m_paramsSetCompPtr);
	I_REF(imod::IModel, m_paramsSetModelCompPtr);
	I_REF(ifpf::IHotfolder, m_hotfolderCompPtr);
	I_FACT(ifpf::IDirectoryMonitor, m_monitorFactCompPtr);

	I_ATTR(istd::CString, m_inputPathParamsManagerIdAttrPtr);
	I_ATTR(istd::CString, m_monitoringParamsIdAttrPtr);
	I_ATTR(istd::CString, m_outputDirectoryParamsIdAttrPtr);
	I_ATTR(istd::CString, m_hotfolderStateModelIdAttrPtr);

	typedef std::map<istd::CString, istd::TDelPtr<ifpf::IDirectoryMonitor> > DirectoryMonitorsMap;

	DirectoryMonitorsMap m_directoryMonitorsMap;
	DirectoryMonitorObserver m_directoryMonitorObserver;
	ParametersObserver m_parametersObserver;

	typedef std::map<istd::CString, istd::TDelPtr<ifpf::CMonitoringSession> > MonitoringSessionsMap;
	MonitoringSessionsMap m_monitoringSessionsMap;

	iqt::CCriticalSection m_parameterLock;
	iqt::CCriticalSection m_processingQueueLock;

	bool m_finishThread;

	istd::TDelPtr<iprm::IParamsSet> m_runParameterPtr;
};


} // namespace iqtfpf


#endif // !iqtfpf_CHotfolderProcessingComp_included

