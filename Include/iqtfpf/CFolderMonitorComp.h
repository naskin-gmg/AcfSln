#ifndef iqtfpf_CFolderMonitorComp_included
#define iqtfpf_CFolderMonitorComp_included


// Qt includes
#include <QFileSystemWatcher>
#include <QFileInfo>
#include <QTimer>
#include <QThread>
#include <QDir>


// ACF includes
#include "imod/CSingleModelObserverBase.h"

#include "icomp/CComponentBase.h"

#include "ibase/TLoggerCompWrap.h"

#include "iqt/CCriticalSection.h"


// AcfSln includes
#include "ifpf/IDirectoryMonitor.h"
#include "ifpf/CDirectoryMonitorParams.h"


namespace iqtfpf
{


/**
	Component for monitoring of the file folder.
*/
class CFolderMonitorComp:
			public QThread,
			public ibase::CLoggerComponentBase,
			public ifpf::CDirectoryMonitorParams,
			virtual public ifpf::IDirectoryMonitor
{
	Q_OBJECT

public:
	typedef ibase::CLoggerComponentBase BaseClass;
	typedef QThread BaseClass2;
	typedef ifpf::CDirectoryMonitorParams BaseClass3;

	I_BEGIN_COMPONENT(CFolderMonitorComp);
		I_REGISTER_INTERFACE(ifpf::IDirectoryMonitor);
		I_REGISTER_INTERFACE(ifpf::IDirectoryMonitorParams);
	I_END_COMPONENT;

	CFolderMonitorComp();

	// reimplemented (ifpf::IDirectoryMonitor)
	virtual istd::CStringList GetChangedFileItems(int changeFlags) const;

	// reimplemented (ibase::IFileListProvider)
	virtual istd::CStringList GetFileList() const;

	// reimplemented (icomp::IComponent)
	virtual void OnComponentCreated();
	virtual void OnComponentDestroyed();

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);

	// reimplemented (istd::IChangeable)
	virtual void OnEndChanges(int changeFlags, istd::IPolymorphic* changeParamsPtr);

protected:
	// reimplemented (QThread)
	virtual void run();

private slots:
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
};


} // namespace iqtfpf


#endif // !iqtfpf_CFolderMonitorComp_included


