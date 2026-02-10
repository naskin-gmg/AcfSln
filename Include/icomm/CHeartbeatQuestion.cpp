// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#include <icomm/CHeartbeatQuestion.h>


// ACF includes
#include <istd/CChangeNotifier.h>
#include <iser/IArchive.h>


namespace icomm
{


CHeartbeatQuestion::CHeartbeatQuestion()
:	m_questionId(-1)
{
}


CHeartbeatQuestion::CHeartbeatQuestion(
				quint32 questionId,
				const QString& applicationName,
				const QString& applicationType)
	:m_questionId(questionId),
	m_applicationName(applicationName),
	m_applicationType(applicationType)
{
}


bool CHeartbeatQuestion::operator==(const CHeartbeatQuestion& question)
{
	return			(m_applicationName == question.m_applicationName) &&
					(m_applicationType == question.m_applicationType) &&
					(m_questionId == question.m_questionId);
}


// reimplemented (iser::ISerializable)

bool CHeartbeatQuestion::Serialize(iser::IArchive& archive)
{
	static iser::CArchiveTag applicationNameTag("ApplicationName", "Name of application");
	static iser::CArchiveTag applicationTypeTag("ApplicationType", "Type of application");
	static iser::CArchiveTag questionIdTag("QuestionId", "ID of this question");

	istd::CChangeNotifier notifier(archive.IsStoring()? NULL: this);

	bool result = true;

	result = result && archive.BeginTag(applicationNameTag);
	result = result && archive.Process(m_applicationName);
	result = result && archive.EndTag(applicationNameTag);

	result = result && archive.BeginTag(applicationTypeTag);
	result = result && archive.Process(m_applicationType);
	result = result && archive.EndTag(applicationTypeTag);

	result = result && archive.BeginTag(questionIdTag);
	result = result && archive.Process(m_questionId);
	result = result && archive.EndTag(questionIdTag);

	return result;
}


} // namespace icomm


