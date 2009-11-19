#ifndef iqtfpf_CFolderMonitorComp_included
#define iqtfpf_CFolderMonitorComp_included


// Qt includes
#include <QFileSystemWatcher>
#include <QFileInfo>
#include <QTimer>
#include <QThread>
#include <QDir>


// ACF includes
#include "imod/CMultiModelObserverBase.h"

#include "icomp/CComponentBase.h"

#include "ibase/TLoggerCompWrap.h"

#include "iqt/CCriticalSection.h"


// AcfSln includes
#include "ifpf/IDirectoryMonitor.h"
#include "ifpf/IDirectoryMonitorParams.h"
#include "ifpf/IMonitoringSessionManager.h"


namespace iqtfpf
{


/**
	Component for monitoring of the file folder.
*/
class CFolderMonitorComp:
			public QThread,
			public ibase::CLoggerComponentBase,
			virtual public ifpf::IDirectoryMonitor,
			virtual protected imod::CMultiModelObserverBase
{
	Q_OBJECT

public:
	typedef ibase::CLoggerComponentBase BaseClass;
	typedef QThread BaseClass2;

	I_BEGIN_COMPONENT(CFolderMonitorComp);
		I_REGISTER_INTERFACE(ifpf::IDirectoryMonitor);
		I_ASSIGN(m_directoryPathCompPtr, "DirectoryPathParams", "Parameter of the path to be observed", true, "DirectoryPath");
		I_ASSIGN(m_directoryPathModelCompPtr, "DirectoryPathParams", "Parameter of the path to be observed", true, "DirectoryPathParams");
		I_ASSIGN(m_directoryMonitorParamsCompPtr, "DirectoryMonitorParams", "Observing parameters", true, "DirectoryMonitorParams");
		I_ASSIGN(m_directoryMonitorParamsModelCompPtr, "DirectoryMonitorParams", "Observing parameters", true, "DirectoryMonitorParams");
		I_ASSIGN(m_monitoringSessionManagerCompPtr, "SessionManager", "Provider of previous monitoring sessions", false, "SessionManager");
	I_END_COMPONENT;

	CFolderMonitorComp();

	// reimplemented (ifpf::IDirectoryMonitor)
	virtual istd::CStringList GetChangedFileItems(int changeFlags) const;

	// reimplemented (ibase::IFileListProvider)
	virtual istd::CStringList GetFileList() const;

	// reimplemented (icomp::IComponent)
	virtual void OnComponentCreated();
	virtual void OnComponentDestroyed();

	// reimplemented (imod::IObserver)
	virtual void AfterUpdate(imod::IModel* modelPtr, int updateFlags, istd::IPolymorphic* updateParamsPtr);

protected:
	// reimplemented (QThread)
	virtual void run();

private Q_SLOTS:
	/**
		Delegate folder change event via istd::CChangeNotifier from main thread.
	*/
	void OnFolderChanged(int changeFlags);

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

private:
	struct FileSystemChanges
	{
		QStringList addedFiles;
		QStringList removedFiles;
		QStringList modifiedFiles;
		QStringList attributeChangedFiles;
	};

	QFileInfoList m_directoryFiles;

	mutable iqt::CCriticalSection m_lock;

	bool m_finishThread;
	bool m_directoryChangesConfirmed;

	FileSystemChanges m_folderChanges;

	QDir m_currentDirectory;

	// Model shadows:
	double m_poolingFrequency;
	QStringList m_fileFilterExpressions;
	int m_observingItemTypes;
	int m_observingChanges;

	// Attributes
	I_REF(iprm::IFileNameParam, m_directoryPathCompPtr);
	I_REF(imod::IModel, m_directoryPathModelCompPtr);
	I_REF(ifpf::IDirectoryMonitorParams, m_directoryMonitorParamsCompPtr);
	I_REF(imod::IModel, m_directoryMonitorParamsModelCompPtr);
	I_REF(ifpf::IMonitoringSessionManager, m_monitoringSessionManagerCompPtr);

};


} // namespace iqtfpf


#endif // !iqtfpf_CFolderMonitorComp_included


