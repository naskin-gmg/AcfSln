#ifndef icmpstr_CRegistryPreviewComp_included
#define icmpstr_CRegistryPreviewComp_included


// Qt includes
#include <QtCore/QProcess>
#include <QtCore/QDir>
#if QT_VERSION >= 0x050000
#include <QtWidgets/QLabel>
#else
#include <QtGui/QLabel>
#endif

// ACF includes
#include "icomp/IComponentEnvironmentManager.h"
#include "ilog/TLoggerCompWrap.h"
#include "ifile/IFileNameParam.h"

// ACF-Solutions includes
#include "icmpstr/IRegistryPreview.h"


namespace icmpstr
{


class CRegistryPreviewComp:
			public QObject,
			public ilog::CLoggerComponentBase,
			public IRegistryPreview
{
	Q_OBJECT
public:
	typedef ilog::CLoggerComponentBase BaseClass;

#ifndef QT_NO_PROCESS
	typedef QProcess::ProcessState ProcessState;
#else
	typedef int ProcessState;
#endif

	I_BEGIN_COMPONENT(CRegistryPreviewComp);
		I_REGISTER_INTERFACE(icmpstr::IRegistryPreview);
		I_ASSIGN(m_versionInfoCompPtr, "VersionInfo", "Version info used to serialize temporary registry file", false, "VersionInfo");
		I_ASSIGN(m_commandFileNameCompPtr, "CommandFileName", "Parameter storing command file name", true, "CommandFileName");
		I_ASSIGN(m_envManagerCompPtr, "EnvironmentManager", "Registry loader used to retrive package configuration", false, "EnvironmentManager");
	I_END_COMPONENT;

	CRegistryPreviewComp();

	// reimplemented (icmpstr::IRegistryPreview)
	virtual bool StartRegistry(const icomp::IRegistry& registry);
	virtual bool IsRunning() const;
	virtual void AbortRegistry();

protected:
	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated();
	virtual void OnComponentDestroyed();

protected Q_SLOTS:
	void OnStateChanged(ProcessState state);
	void OnReadyReadStandardError();
	void OnReadyReadStandardOutput();
private:
	QString m_tempFileName;
#ifndef QT_NO_PROCESS
	QProcess m_process;
#endif
	bool m_isRunning;

	I_REF(ifile::IFileNameParam, m_commandFileNameCompPtr);
	I_REF(icomp::IComponentEnvironmentManager, m_envManagerCompPtr);
	I_REF(iser::IVersionInfo, m_versionInfoCompPtr);
};


} // namespace icmpstr


#endif // !icmpstr_CRegistryPreviewComp_included


