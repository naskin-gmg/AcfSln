#ifndef iqtfpf_CHotfolderComp_included
#define iqtfpf_CHotfolderComp_included


// Qt includes
#include<QThread>


// ACF includes
#include "ifpf/IDirectoryMonitor.h"

#include "imod/CMultiModelObserverBase.h"

#include "ibase/IFileConvertCopy.h"
#include "ibase/TLoggerCompWrap.h"

#include "iprm/IParamsSet.h"

#include "iqt/CCriticalSection.h"


// AcfSln includes
#include "ifpf/IFileNamingStrategy.h"
#include "ifpf/IMonitoringSessionManager.h"
#include "ifpf/CHotfolder.h"
#include "ifpf/CMonitoringSession.h"


namespace iqtfpf
{


/**
	Hotfolder representation.
*/
class CHotfolderComp:
			public QThread,
			public ibase::CLoggerComponentBase,
			virtual public ifpf::IFileNamingStrategy,
			virtual public ifpf::IMonitoringSessionManager,
			virtual public ifpf::CHotfolder
{
	Q_OBJECT
public:
	typedef ibase::CLoggerComponentBase BaseClass;
	typedef ifpf::CHotfolder BaseClass2;
	typedef QThread BaseClass3;

	I_BEGIN_COMPONENT(CHotfolderComp);
		I_REGISTER_INTERFACE(ifpf::IFileNamingStrategy);
		I_ASSIGN(m_fileConvertCompPtr, "FileConverter", "File converter", true, "FileConverter");
		I_ASSIGN(m_fileNamingStrategyCompPtr, "FileNamingStrategy", "Strategy for naming of the output file", true, "FileNamingStrategy");
		I_ASSIGN(m_paramsSetCompPtr, "ParamsSet", "Parameter set for the hotfolder", true, "ParamsSet");
		I_ASSIGN(m_monitorFactCompPtr, "DirectoryMontorFactory", "Factory for creation of a directory monitor", true, "DirectoryMontorFactory");
		I_ASSIGN(m_inputPathParamsManagerIdAttrPtr, "InputPathParamsManagerId", "Parameter set ID for the hotfolder's input directories", true, "");
		I_ASSIGN(m_outputDirectoryParamsIdAttrPtr, "OutputDirectoryParamsId", "Parameter set ID for the output directory", true, "");
	I_END_COMPONENT();

	// reimplemented (ifpf::IFileNamingStrategy)
	virtual istd::CString GetFileName(const istd::CString& inputFileName) const;

	// reimplemented (ifpf::IMonitoringSessionManager)
	virtual ifpf::IMonitoringSession* GetSession(const ifpf::IDirectoryMonitor& directoryMonitor, const istd::CString& directoryPath) const;

	// reimplemeneted (icomp::IComponent)
	virtual void OnComponentCreated();
	virtual void OnComponentDestroyed();

protected:
	virtual bool OnIncommingInputFileEvent(const ifpf::IDirectoryMonitor& directoryMonitor);

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
		Internal observer of changes in the input directories.
	*/
	class DirectoryMonitorObserver: public imod::CMultiModelObserverBase
	{
	public:
		DirectoryMonitorObserver(CHotfolderComp& parent);

		// reimplemented (imod::IObserver)
		virtual void AfterUpdate(imod::IModel* modelPtr, int updateFlags, istd::IPolymorphic* updateParamsPtr);

	private:
		CHotfolderComp& m_parent;
	};
	
	I_REF(ibase::IFileConvertCopy, m_fileConvertCompPtr);
	I_REF(ifpf::IFileNamingStrategy, m_fileNamingStrategyCompPtr);
	I_REF(iprm::IParamsSet, m_paramsSetCompPtr);
	I_FACT(	ifpf::IDirectoryMonitor, m_monitorFactCompPtr);

	I_ATTR(istd::CString, m_inputPathParamsManagerIdAttrPtr);
	I_ATTR(istd::CString, m_outputDirectoryParamsIdAttrPtr);

	typedef std::map<istd::CString, istd::TDelPtr<ifpf::IDirectoryMonitor> > DirectoryMonitorsMap;
	typedef std::map<istd::CString, istd::TDelPtr<ifpf::CMonitoringSession> > MonitoringSessionsMap;

	DirectoryMonitorsMap m_directoryMonitorsMap;
	MonitoringSessionsMap m_monitoringSessionsMap;

	iqt::CCriticalSection m_parameterLock;
	iqt::CCriticalSection m_processingQueueLock;

	struct QueueItem
	{
		istd::CString inputFile;
		istd::CString outputFile;
	};

	typedef std::list<QueueItem> ItemQueue;

	bool m_finishThread;

	istd::TDelPtr<iprm::IParamsSet> m_runParameterPtr;

	ItemQueue m_processingQueue;
};


} // namespace iqtfpf


#endif // !iqtfpf_CHotfolderComp_included

