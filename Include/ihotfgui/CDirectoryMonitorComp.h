#ifndef ihotfgui_CDirectoryMonitorComp_included
#define ihotfgui_CDirectoryMonitorComp_included


// Qt includes
#include <QtCore/QFileSystemWatcher>
#include <QtCore/QThread>
#include <QtCore/QDir>
#include <QtCore/QSet>
#include <QtCore/QMutex>

// ACF includes
#include "imod/TSingleModelObserverBase.h"
#include "icomp/CComponentBase.h"
#include "ilog/TLoggerCompWrap.h"
#include "iprm/IParamsSet.h"
#include "ifile/IFileNameParam.h"

// AcfSln includes
#include "ihotf/IFileSystemChangeStorage.h"
#include "ihotf/IDirectoryMonitor.h"
#include "ihotf/IDirectoryMonitorParams.h"
#include "ihotf/IMonitoringSessionManager.h"


namespace ihotfgui
{


/**
	Component for monitoring of the file folder.
*/
class CDirectoryMonitorComp:
			public QThread,
			public ilog::CLoggerComponentBase,
			virtual public ihotf::IDirectoryMonitor
{
	Q_OBJECT

public:
	typedef ilog::CLoggerComponentBase BaseClass;
	typedef QThread BaseClass2;

	I_BEGIN_COMPONENT(CDirectoryMonitorComp);
		I_REGISTER_INTERFACE(ihotf::IDirectoryMonitor);
		I_ASSIGN(m_paramsSetCompPtr, "ParamsSet", "Default parameter set for the directory monitoring", false, "ParamsSet");
		I_ASSIGN_TO(m_paramsSetModelCompPtr, m_paramsSetCompPtr, false);
		I_ASSIGN(m_monitoringSessionManagerCompPtr, "SessionManager", "Provider of previous monitoring sessions", false, "SessionManager");
		I_ASSIGN(m_fileSystemChangeStorageCompPtr, "FileSystemChangeStorage", "File storage", true, "FileSystemChangeStorage");
		I_ASSIGN(m_directoryPathIdAttrPtr, "DirParamId", "Parameter ID of the path to be observed in the parameter set", true, "DirParamId");
		I_ASSIGN(m_directoryMonitorParamsIdAttrPtr, "DirectoryMonitorParamsId", "ID of the directory observing parameters in the parameter set", true, "DirectoryMonitorParamsId");
		I_ASSIGN(m_autoStartAttrPtr, "AutoStart", "If enabled, start the directory monitoring after initialization", false, false);
	I_END_COMPONENT;

	CDirectoryMonitorComp();

	// reimplemented (ihotf::IDirectoryMonitor)
	virtual bool IsRunning() const;
	virtual bool StartObserving(const iprm::IParamsSet* paramsSetPtr = NULL);
	virtual void StopObserving();

protected:
	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated();
	virtual void OnComponentDestroyed();

	// reimplemented (QThread)
	virtual void run();

private Q_SLOTS:
	/**
		Delegate folder change event via istd::CChangeNotifier from main thread.
	*/
	void OnFolderChanged(const istd::IChangeable::ChangeSet& changeSet);

	/**
		Notification about changes in a given directory.
	*/
	void OnDirectoryChangeNotification(const QString& directory);

Q_SIGNALS:
	/**
		Signal is emitted from observing thread to notifiy the main thread about changes in a directory.
	*/
	void FolderChanged(const istd::IChangeable::ChangeSet& changeSet);

private:
	struct FileAccessInfo;

	void SetFolderPath(const QString& folderPath);
	void StartObserverThread();
	void StopObserverThread();
	void ResetFiles();
	bool ConnectToParameterModel(const iprm::IParamsSet& paramsSet);
	void DisconnectFromParameterModel();
	void UpdateMonitoringSession() const;
	bool HasFileAccess(const QString& filePath, FileAccessInfo* lastFileAccessInfoPtr = NULL) const;
	bool CheckFileAccess(const QString& filePath) const;

private:
	struct FileSystemChanges
	{
		QStringList addedFiles;
		QStringList removedFiles;
		QStringList modifiedFiles;
		QStringList attributeChangedFiles;
	};

	/**
		\internal
	*/
	class MonitoringParamsObserver: public imod::TSingleModelObserverBase<ihotf::IDirectoryMonitorParams>
	{
	public:
		typedef imod::TSingleModelObserverBase<ihotf::IDirectoryMonitorParams> BaseClass;

		MonitoringParamsObserver(CDirectoryMonitorComp& parent);

		// reimplemented (imod::IObserver)
		virtual void AfterUpdate(imod::IModel* modelPtr, const istd::IChangeable::ChangeSet& changeSet);

	private:
		CDirectoryMonitorComp& m_parent;
	};

	/**
		\internal
	*/
	class DirectoryParamsObserver: public imod::TSingleModelObserverBase<ifile::IFileNameParam>
	{
	public:
		typedef imod::TSingleModelObserverBase<ifile::IFileNameParam> BaseClass;

		DirectoryParamsObserver(CDirectoryMonitorComp& parent);

		// reimplemented (imod::IObserver)
		virtual void AfterUpdate(imod::IModel* modelPtr, const istd::IChangeable::ChangeSet& changeSet);

	private:
		CDirectoryMonitorComp& m_parent;
	};

	struct FileAccessInfo
	{
		FileAccessInfo()
			:fileSize(0)
		{
		}

		QDateTime accessTimeStamp;
		qint64 fileSize;
	};

	typedef QMap<QString, FileAccessInfo> FileAccessMap;

	ihotf::IMonitoringSession::FileItems m_directoryFiles;
	FileAccessMap m_nonAccessedAddedFiles;

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
	int m_lastModificationMinDifference;
	int m_folderDepth;
	int m_timestampMode;

	MonitoringParamsObserver m_monitoringParamsObserver;
	DirectoryParamsObserver m_directoryParamsObserver;

	// Component attributes
	I_REF(iprm::IParamsSet, m_paramsSetCompPtr);
	I_REF(imod::IModel, m_paramsSetModelCompPtr);
	I_REF(ihotf::IMonitoringSessionManager, m_monitoringSessionManagerCompPtr);
	I_REF(ihotf::IFileSystemChangeStorage, m_fileSystemChangeStorageCompPtr);
	I_ATTR(QByteArray, m_directoryPathIdAttrPtr);
	I_ATTR(QByteArray, m_directoryMonitorParamsIdAttrPtr);
	I_ATTR(bool, m_autoStartAttrPtr);

	bool m_lockChanges;

	mutable QMutex m_mutex;
};


} // namespace ihotfgui


#endif // !ihotfgui_CDirectoryMonitorComp_included


