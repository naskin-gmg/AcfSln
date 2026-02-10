// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#include <icmpstr/CRegistryPreviewComp.h>


// Qt includes
#include <QtCore/QDir>
#include <QtCore/QMetaType>

// ACF includes
#include <istd/CChangeNotifier.h>
#include <ifile/CCompactXmlFileWriteArchive.h>
#include <istd/CSystem.h>


#ifndef QT_NO_PROCESS
	Q_DECLARE_METATYPE(QProcess::ProcessState);
#endif

namespace icmpstr
{


// public methods

CRegistryPreviewComp::CRegistryPreviewComp()
:	m_isRunning(false)
{
}


// reimplemented (icmpstr::IRegistryPreview)

bool CRegistryPreviewComp::StartRegistry(const icomp::IRegistry& registry)
{
#ifndef QT_NO_PROCESS
	if (IsRunning() || m_tempFileName.isEmpty() || !m_commandFileNameCompPtr.IsValid()){
		return false;
	}

	ifile::CCompactXmlFileWriteArchive archive(m_tempFileName, m_versionInfoCompPtr.GetPtr());

	if (!(const_cast<icomp::IRegistry&>(registry)).Serialize(archive)){
		SendErrorMessage(0, QString(tr("Working registry could not be saved into %1. ACF runtime execution canceled")).arg(m_tempFileName));

		return false;
	}

	QString acfExeFile = istd::CSystem::GetEnrolledPath(m_commandFileNameCompPtr->GetPath());

	QDir applicationDir(QCoreApplication::applicationDirPath());
	QString acfApplicationPath = QDir::toNativeSeparators(applicationDir.absoluteFilePath(acfExeFile));

	m_process.setWorkingDirectory(applicationDir.path());

	QStringList parameters;
	parameters << m_tempFileName;

	if (m_envManagerCompPtr.IsValid()){
		QString configFilePath = m_envManagerCompPtr->GetConfigFilePath();
		if (!configFilePath.isEmpty()){
			parameters << "-config";
			parameters << QDir::toNativeSeparators(configFilePath);
		}
	}

	SendInfoMessage(0, QString("Start ACF runtime: %1 %2").arg(acfApplicationPath).arg(parameters.join(" ")));

	m_process.start(acfApplicationPath, parameters);

	bool result = m_process.waitForStarted();
	if (!result){
		SendErrorMessage(0, QString("Unfortunately, ACF process could not be started. Check your ACF command path!"));
	}
	return result;
#endif

	return false;
}


bool CRegistryPreviewComp::IsRunning() const
{
	return m_isRunning;
}


void CRegistryPreviewComp::AbortRegistry()
{
#ifndef QT_NO_PROCESS
	m_process.terminate();
	if (!m_process.waitForFinished(5000)){
		m_process.kill();
	}
#endif
}


// protected methods

// reimplemented (icomp::CComponentBase)

void CRegistryPreviewComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

#ifndef QT_NO_PROCESS
	qRegisterMetaType<QProcess::ExitStatus>("QProcess::ExitStatus");
	qRegisterMetaType<QProcess::ProcessState>();

	m_tempFileName.clear();

	QDir tempDir = QDir::temp();
	if (tempDir.exists()){
		m_tempFileName = QDir::toNativeSeparators(tempDir.absoluteFilePath("registry_preview.acc"));
	}

	m_isRunning = false;

	connect(	&m_process,
				SIGNAL(stateChanged(QProcess::ProcessState)), 
				this, 
				SLOT(OnStateChanged(QProcess::ProcessState)), 
				Qt::QueuedConnection);

	connect(&m_process, SIGNAL(readyReadStandardError()), this, SLOT(OnReadyReadStandardError()));
	connect(&m_process, SIGNAL(readyReadStandardOutput()), this, SLOT(OnReadyReadStandardOutput()));
#endif
}


void CRegistryPreviewComp::OnComponentDestroyed()
{
#ifndef QT_NO_PROCESS
	if (IsRunning()){
		m_process.kill();

		m_process.waitForFinished(5000);
	}

	QFile::remove(m_tempFileName);
#endif
	BaseClass::OnComponentDestroyed();
}


// protected slots

#ifndef QT_NO_PROCESS
void CRegistryPreviewComp::OnStateChanged(QProcess::ProcessState state)
{
	Q_UNUSED(state);

	istd::CChangeNotifier notifier(this);

	m_isRunning = (state == QProcess::Running);
}


void CRegistryPreviewComp::OnReadyReadStandardError()
{
	QString errorOutput = m_process.readAllStandardError();
	
	errorOutput = errorOutput.simplified();

	SendErrorMessage(0, errorOutput);
}


void CRegistryPreviewComp::OnReadyReadStandardOutput()
{
	QString standardOutput = m_process.readAllStandardOutput();

	standardOutput = standardOutput.simplified();

	SendInfoMessage(0, standardOutput);
}

#endif // QT_NO_PROCESS


} // namespace icmpstr


