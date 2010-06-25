#ifndef iqtfpf_CDirectoryMonitorComp_included
#define iqtfpf_CDirectoryMonitorComp_included


// Qt includes
#include <QFileSystemWatcher>
#include <QFileInfo>
#include <QTimer>
#include <QThread>
#include <QDir>
#include <QDateTime>


// ACF includes
#include "imod/TSingleModelObserverBase.h"

#include "icomp/CComponentBase.h"

#include "ibase/TLoggerCompWrap.h"

#include "iqt/CCriticalSection.h"

#include "iprm/IParamsSet.h"


// AcfSln includes
#include "ifpf/IDirectoryMonitor.h"
#include "ifpf/IDirectoryMonitorParams.h"
#include "ifpf/IMonitoringSessionManager.h"


namespace iqtfpf
{


/**
	Component for monitoring of the file folder.
*/
class CDirectoryMonitorComp:
			public QThread,
			public ibase::CLoggerComponentBase,
			virtual public ifpf::IDirectoryMonitor
{
	Q_OBJECT

public:
	typedef ibase::CLoggerComponentBase BaseClass;
	typedef QThread BaseClass2;

	I_BEGIN_COMPONENT(CDirectoryMonitorComp);
		I_REGISTER_INTERFACE(ifpf::IDirectoryMonitor);
		I_ASSIGN(m_paramsSetCompPtr, "ParamsSet", "Default parameter set for the directory monitoring", false, "ParamsSet");
		I_ASSIGN(m_paramsSetModelCompPtr, "ParamsSet", "Default parameter set for the directory monitoring", false, "ParamsSet");
		I_ASSIGN(m_monitoringSessionManagerCompPtr, "SessionManager", "Provider of previous monitoring sessions", false, "SessionManager");
		I_ASSIGN(m_directoryPathIdAttrPtr, "DirectoryPathParamsId", "Parameter ID of the path to be observed in the parameter set", true, "DirectoryPathParamsId");
		I_ASSIGN(m_directoryMonitorParamsIdAttrPtr, "DirectoryMonitorParamsId", "ID of the directory observing parameters in the parameter set", true, "DirectoryMonitorParamsId");
		I_ASSIGN(m_autoStartAttrPtr, "AutoStart", "If enabled, start the directory monitoring after initialization", false, false);
	I_END_COMPONENT;

	CDirectoryMonitorComp();

	// reimplemented (ifpf::IDirectoryMonitor)
	virtual istd::CStringList GetChangedFileItems(int changeFlags) const;
	virtual bool StartObserving(const iprm::IParamsSet* paramsSetPtr = NULL);
	virtual void StopObserving();

	// reimplemented (ibase::IFileListProvider)
	virtual istd::CStringList GetFileList() const;

	// reimplemented (icomp::IComponent)
	virtual void OnComponentCreated();
	virtual void OnComponentDestroyed();

protected:
	// reimplemented (QThread)
	virtual void run();

private Q_SLOTS:
	/**
		Delegate folder change event via istd::CChangeNotifier from main thread.
	*/
	void OnFolderChanged(int changeFlags);

	/**
		Notification about changes in a given directory.
	*/
	void OnDirectoryChangeNotification(const QString& directory);

Q_SIGNALS:
	/**
		Signal is emitted from observing thread to notifiy the main thread about changes in a directory.
	*/
	void FolderChanged(int changeFlags);

private:
	void SetFolderPath(const QString& folderPath);
	void StartObserverThread();
	void StopObserverThread();
	void ResetFiles();
	bool ConnectToParameterModel(const iprm::IParamsSet& paramsSet);
	void DisconnectFromParameterModel();
	void UpdateMonitoringSession() const;

private:
	struct FileSystemChanges
	{
		QStringList addedFiles;
		QStringList removedFiles;
		QStringList modifiedFiles;
		QStringList attributeChangedFiles;
	};

	class MonitoringParamsObserver: public imod::TSingleModelObserverBase<ifpf::IDirectoryMonitorParams>
	{
	public:
		typedef imod::TSingleModelObserverBase<ifpf::IDirectoryMonitorParams> BaseClass;

		MonitoringParamsObserver(CDirectoryMonitorComp& parent);

		// reimplemented (imod::IObserver)
		virtual bool OnDetached(imod::IModel* modelPtr);
		virtual void AfterUpdate(imod::IModel* modelPtr, int updateFlags, istd::IPolymorphic* updateParamsPtr);

	private:
		CDirectoryMonitorComp& m_parent;
	};

	class DirectoryParamsObserver: public imod::TSingleModelObserverBase<iprm::IFileNameParam>
	{
	public:
		typedef imod::TSingleModelObserverBase<iprm::IFileNameParam> BaseClass;

		DirectoryParamsObserver(CDirectoryMonitorComp& parent);

		// reimplemented (imod::IObserver)
		virtual bool OnDetached(imod::IModel* modelPtr);
		virtual void AfterUpdate(imod::IModel* modelPtr, int updateFlags, istd::IPolymorphic* updateParamsPtr);

	private:
		CDirectoryMonitorComp& m_parent;
	};

	struct FileItem
	{
		QString filePath;
		QDateTime modifiedTime;
		QFile::Permissions permissions;

		bool operator== (const FileItem& fileItem) const
		{
			return (filePath == fileItem.filePath);
		}
	};

	typedef std::vector<FileItem> FileItems;

	FileItems m_directoryFiles;

	mutable iqt::CCriticalSection m_lock;

	bool m_finishThread;

	FileSystemChanges m_folderChanges;

	QDir m_currentDirectory;

	int m_directoryPendingChangesCounter;

	QFileSystemWatcher m_directoryWatcher;

	// Model shadows:
	double m_poolingFrequency;
	QStringList m_fileFilterExpressions;
	int m_observingItemTypes;
	int m_observingChanges;

	// Directory monitoring parameter model
	const imod::IModel* m_directoryPathModelPtr;
	const imod::IModel* m_directoryMonitorParamsModelPtr;

	MonitoringParamsObserver m_monitoringParamsObserver;
	DirectoryParamsObserver m_directoryParamsObserver;

	// Component attributes
	I_REF(iprm::IParamsSet, m_paramsSetCompPtr);
	I_REF(imod::IModel, m_paramsSetModelCompPtr);
	I_REF(ifpf::IMonitoringSessionManager, m_monitoringSessionManagerCompPtr);
	I_ATTR(istd::CString, m_directoryPathIdAttrPtr);
	I_ATTR(istd::CString, m_directoryMonitorParamsIdAttrPtr);
	I_ATTR(bool, m_autoStartAttrPtr);
};


} // namespace iqtfpf


#endif // !iqtfpf_CDirectoryMonitorComp_included


