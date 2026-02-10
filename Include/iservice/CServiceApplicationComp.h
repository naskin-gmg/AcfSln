// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#ifndef iservice_CServiceApplicationComp_included
#define iservice_CServiceApplicationComp_included


// Qt includes
#include <QtGui/QIcon>
#if QT_VERSION >= 0x050000
#include <QtWidgets/QSystemTrayIcon>
#else
#include <QtGui/QSystemTrayIcon>
#endif

// ACF includes
#include <ibase/IApplication.h>
#include <ilog/TLoggerCompWrap.h>
#include <icomp/CComponentBase.h>

#ifndef Q_OS_MAC
#ifndef Q_OS_LINUX
// ACF-Solutions includes
#include <iservice/QtService.h>
#endif
#endif


namespace iservice
{


/**
	Qt based component for service-based application.
*/
class CServiceApplicationComp:
			public QObject,
			public ilog::CLoggerComponentBase,
			public ibase::IApplication
{
	Q_OBJECT
public:
	typedef ilog::CLoggerComponentBase BaseClass;
	typedef QObject BaseClass2;

	I_BEGIN_COMPONENT(CServiceApplicationComp);
		I_REGISTER_INTERFACE(ibase::IApplication);
		I_ASSIGN(m_applicationCompPtr, "ApplicationInstance", "Service application object", true, "Application");
		I_ASSIGN(m_serviceDescriptionAttrPtr, "SeriviceDescription", "Service description", true, "This services provides...");
		I_ASSIGN(m_serviceNameAttrPtr, "ServiceName", "The name of the service", true, "MyService");
		I_ASSIGN(m_manualStartupAttrPtr, "ManualStartup", "If enabled, the service is registered with manual start up", false, false);
	I_END_COMPONENT;

	QStringList GetApplicationArguments(int argc, char** argv) const;

	// reimplemented (ibase::IApplication)
	virtual bool InitializeApplication(int argc, char** argv);
	virtual int Execute(int argc, char** argv);
	virtual QString GetHelpText() const;
	virtual QStringList GetApplicationArguments() const;

	// reimplemented (QObject)
	virtual bool eventFilter(QObject* sourcePtr, QEvent* eventPtr);

protected Q_SLOTS:
	void OnTrayIconActivated(QSystemTrayIcon::ActivationReason reason);

#ifdef Q_OS_WIN

protected:
	class CService: public QtServiceBase
	{
	public:
		typedef QtServiceBase BaseClass;

		CService(CServiceApplicationComp& parent,
					ibase::IApplication& application,
					int serviceArgc,
					char** serviceArgv,
					const QString &name);

	protected:
		// reimplemented (QtServiceBase)
		virtual void start();
		virtual void stop();
		virtual void pause();
		virtual void resume();
		virtual void createApplication(int &argc, char **argv);
		virtual int executeApplication();

	private:
		QVector<char*> GetApplicationArguments() const;

	private:
		CServiceApplicationComp& m_parent;
		ibase::IApplication& m_application;
		QVector<QByteArray> m_applicationArguments;
	};

	istd::TDelPtr<CService> m_servicePtr;
#endif // Q_OS_WIN

private:
	I_REF(ibase::IApplication, m_applicationCompPtr);
	I_TEXTATTR(m_serviceDescriptionAttrPtr);
	I_ATTR(QString, m_serviceNameAttrPtr);
	I_ATTR(bool, m_manualStartupAttrPtr);
};


} // namespace iservice


#endif // iservice_CServiceApplicationComp_included

