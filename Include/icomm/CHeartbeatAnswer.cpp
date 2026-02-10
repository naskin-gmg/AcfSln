// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#include <icomm/CHeartbeatAnswer.h>


// ACF includes
#include <istd/CChangeNotifier.h>
#include <iser/IArchive.h>


namespace icomm
{


class CHearBeatQuestion;


CHeartbeatAnswer::CHeartbeatAnswer()
:	m_versionNumber(0),
	m_stationState(S_UNKNOWN),
	m_versionInfo(NULL)
{
}


CHeartbeatAnswer::CHeartbeatAnswer(const CHeartbeatQuestion& question)
:	BaseClass(question),
	m_versionNumber(0),
	m_stationState(S_UNKNOWN),
	m_versionInfo(NULL)
{
}


CHeartbeatAnswer::CHeartbeatAnswer(const CHeartbeatAnswer& answer)
	:BaseClass(answer),
	m_versionNumber(answer.m_versionNumber),
	m_computerName(answer.m_computerName),
	m_stationName(answer.m_stationName),
	m_stationDescription(answer.m_stationDescription),
	m_stationState(answer.m_stationState),
	m_loggedUser(answer.m_loggedUser),
	m_loggedComputer(answer.m_loggedComputer),
	m_loggedIp(answer.m_loggedIp),
	m_versionInfo(answer.m_versionInfo)
{
}


void CHeartbeatAnswer::SetStationState(State state)
{
	m_stationState = state;
}


void CHeartbeatAnswer::SetLoggedUser(const QString& userName)
{
	m_loggedUser = userName;
}


void CHeartbeatAnswer::SetLoggedComputer(const QString& computerName)
{
	m_loggedComputer = computerName;
}


void CHeartbeatAnswer::SetLoggedIp(const QByteArray& ip)
{
	m_loggedIp = ip;
}


CHeartbeatAnswer& CHeartbeatAnswer::operator=(const CHeartbeatAnswer& answer)
{
	BaseClass::operator=(answer);

	m_versionNumber = answer.m_versionNumber;
	m_computerName = answer.m_computerName;
	m_stationName = answer.m_stationName;
	m_stationDescription = answer.m_stationDescription;
	m_stationState = answer.m_stationState;
	m_loggedUser = answer.m_loggedUser;
	m_loggedComputer = answer.m_loggedComputer;
	m_loggedIp = answer.m_loggedIp;

	return *this;
}


bool CHeartbeatAnswer::operator==(const CHeartbeatAnswer& question)
{
	return		BaseClass::operator==(question) &&
				(m_versionNumber == question.m_versionNumber) &&
				(m_computerName == question.m_computerName) &&
				(m_stationName == question.m_stationName) &&
				(m_stationDescription == question.m_stationDescription) &&
				(m_stationState == question.m_stationState) &&
				(m_loggedUser == question.m_loggedUser) &&
				(m_loggedComputer == question.m_loggedComputer) &&
				(m_loggedIp == question.m_loggedIp);
}


// reimplemented (ibase::IApplicationInfo)

QString CHeartbeatAnswer::GetApplicationAttribute(int /*attributeId*/, bool /*allowTranslation*/) const
{
	return "";
}


int CHeartbeatAnswer::GetMainVersionId() const
{
	return -1;
}


const iser::IVersionInfo& CHeartbeatAnswer::GetVersionInfo() const
{
	return m_versionInfo;
}


// reimplemented (icomm::IStationState)

CHeartbeatAnswer::State CHeartbeatAnswer::GetStationState() const
{
	return m_stationState;
}


const QString& CHeartbeatAnswer::GetLoggedUser() const
{
	return m_loggedUser;
}


const QString& CHeartbeatAnswer::GetLoggedComputer() const
{
	return m_loggedComputer;
}


const QByteArray& CHeartbeatAnswer::GetLoggedIp() const
{
	return m_loggedIp;
}


// reimplemented (iprm:INameParam)

const QString& CHeartbeatAnswer::GetName() const
{
	return m_stationName;
}


void CHeartbeatAnswer::SetName(const QString& name)
{
	m_stationName = name;
}


bool CHeartbeatAnswer::IsNameFixed() const
{
	return false;
}


// reimplemented (qprd::IStationInfo)

const QString& CHeartbeatAnswer::GetDescription() const
{
	return m_stationDescription;
}


void CHeartbeatAnswer::SetDescription(const QString& description)
{
	m_stationDescription = description;
}


// reimplemented (iser::ISerializable)

bool CHeartbeatAnswer::Serialize(iser::IArchive& archive)
{
	static iser::CArchiveTag versionNumberTag("VersionNumber", "Station version number");
	static iser::CArchiveTag computerNameTag("ComputerName", "OS name of computer");
	static iser::CArchiveTag stationNameTag("StationName", "Logical name of station");
	static iser::CArchiveTag stationDescriptionTag("StationDescription", "User defined description of station");
	static iser::CArchiveTag stationStateTag("StationState", "State of station");
	static iser::CArchiveTag loggedUserTag("LoggedUser", "Name of logged user");
	static iser::CArchiveTag loggedComputerTag("LoggedComputer", "Computer name of logged user");
	static iser::CArchiveTag loggedIpTag("LoggedIP", "IP address of logged user");

	istd::CChangeNotifier notifier(archive.IsStoring()? NULL: this);

	bool result = BaseClass::Serialize(archive);

	result = result && archive.BeginTag(versionNumberTag);
	result = result && archive.Process(m_versionNumber);
	result = result && archive.EndTag(versionNumberTag);

	result = result && archive.BeginTag(computerNameTag);
	result = result && archive.Process(m_computerName);
	result = result && archive.EndTag(computerNameTag);

	result = result && archive.BeginTag(stationNameTag);
	result = result && archive.Process(m_stationName);
	result = result && archive.EndTag(stationNameTag);

	result = result && archive.BeginTag(stationNameTag);
	result = result && archive.Process(m_stationName);
	result = result && archive.EndTag(stationNameTag);

	result = result && archive.BeginTag(stationDescriptionTag);
	result = result && archive.Process(m_stationDescription);
	result = result && archive.EndTag(stationDescriptionTag);

	result = result && archive.BeginTag(stationStateTag);
	int stationState = int(m_stationState);
	result = result && archive.Process(stationState);
	m_stationState = State(stationState);
	result = result && archive.EndTag(stationStateTag);

	if (result && (m_stationState >= S_LOGGED)){
		result = result && archive.BeginTag(loggedUserTag);
		result = result && archive.Process(m_loggedUser);
		result = result && archive.EndTag(loggedUserTag);

		result = result && archive.BeginTag(loggedComputerTag);
		result = result && archive.Process(m_loggedComputer);
		result = result && archive.EndTag(loggedComputerTag);

		result = result && archive.BeginTag(loggedIpTag);
		result = result && archive.Process(m_loggedIp);
		result = result && archive.EndTag(loggedIpTag);
	}

	return result;
}


} // namespace icomm

