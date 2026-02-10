// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#include <icomm/CUdpServersManagerComp.h>


// Qt includes
#include <QtCore/QCoreApplication>
#include <QtCore/QElapsedTimer>

// ACF includes
#include <istd/CChangeNotifier.h>
#include <iser/CMemoryReadArchive.h>
#include <iser/CMemoryWriteArchive.h>


namespace icomm
{


CUdpServersManagerComp::CUdpServersManagerComp()
	:m_maxLostHeartbeats(10)
{
	connect(&m_heartbeatSocket, SIGNAL(readyRead()), this, SLOT(OnHeartbeatRead()));
	connect(&m_timer, SIGNAL(timeout()), this, SLOT(OnTimerTick()));
}


// reimplemented (icomm::IHeartbeatServersManager)

bool CUdpServersManagerComp::FindServers(FoundServers& result)
{
	QElapsedTimer timer;

	if (m_includedeartbeatServerPortAttrPtr.IsValid() && m_applicationInfoCompPtr.IsValid()){
		icomm::CHeartbeatQuestion question;
		question.SetApplicationName(m_applicationInfoCompPtr->GetApplicationAttribute(ibase::IApplicationInfo::AA_APPLICATION_NAME));
		question.SetQuestionId(quint32(-1));
		iser::CMemoryWriteArchive questionArchive(&m_applicationInfoCompPtr->GetVersionInfo());
		if (question.Serialize(questionArchive)){
			m_answers.clear();

			m_heartbeatSocket.writeDatagram(
							(const char*)questionArchive.GetBuffer(),
							questionArchive.GetBufferSize(),
							QHostAddress::Broadcast,
							*m_includedeartbeatServerPortAttrPtr);

			double searchTime = 2;
			if (m_searchTimeAttrPtr.IsValid()){
				searchTime = *m_searchTimeAttrPtr;
			}

			for (;;){
				double restTime = searchTime - timer.elapsed() * 0.001;
				if (restTime > 0){
					QCoreApplication::processEvents(QEventLoop::ExcludeUserInputEvents, int(searchTime * 1000));
				}
				else{
					break;
				}
			}

			result = m_answers;

			return true;
		}
	}

	return false;
}


icomm::CHeartbeatAnswer& CUdpServersManagerComp::GetServerInfoImpl(int index) const
{
	Q_ASSERT(index >= 0);
	Q_ASSERT(index < int(m_servers.size()));

	return const_cast<icomm::CHeartbeatAnswer&>(m_servers[index].stationInfo);
}


int CUdpServersManagerComp::CreateServerInfo(int index, int copyFromIndex)
{
	ServerInfo newElement;
	if (copyFromIndex >= 0){
		Q_ASSERT(copyFromIndex < int(m_servers.size()));

		newElement.stationInfo = m_servers[copyFromIndex].stationInfo;
	}

	if (index >= 0){
		Q_ASSERT(index < int(m_servers.size()));

		ServerInfos::iterator iter = m_servers.begin();
		iter += index;

		m_servers.insert(iter, newElement);

		return index;
	}
	else{
		m_servers.push_back(newElement);

		return int(m_servers.size()) - 1;
	}
}


bool CUdpServersManagerComp::RemoveServerInfo(int index)
{
	if ((index >= 0) && (index < int(m_servers.size()))){
		ServerInfos::iterator iter = m_servers.begin();
		iter += index;

		m_servers.erase(iter);

		return true;
	}

	return false;
}


// reimplemented (icomm::IServersManager)

int CUdpServersManagerComp::GetServersCount() const
{
	return int(m_servers.size());
}


const icomm::IServerInfo& CUdpServersManagerComp::GetServerInfo(int index) const
{
	return m_servers[index].stationInfo;
}


// reimplemented (iser::ISerializable)

bool CUdpServersManagerComp::Serialize(iser::IArchive& archive)
{
	static iser::CArchiveTag serverInfoTag("ServerInfo", "Server info");

	istd::CChangeNotifier notifier(archive.IsStoring()? NULL: this);

	int serversCount = int(m_servers.size());

	bool retVal = true;
	retVal = retVal && archive.BeginMultiTag(serverInfoTag, serverInfoTag, serversCount);

	if (!archive.IsStoring()){
		m_servers.clear();
		m_servers.resize(serversCount);
	}

	for (int i = 0; retVal && (i < serversCount); ++i){
		QString name;
		QString description;
		quint32 version;
		QString computerName;
		ServerInfo& serverInfo = m_servers[i];

		if (archive.IsStoring()){
			name = serverInfo.stationInfo.GetName();
			description = serverInfo.stationInfo.GetDescription();
			serverInfo.stationInfo.GetVersionInfo().GetVersionNumber(0, version);
			computerName = serverInfo.stationInfo.GetComputerName();
		}

		retVal = retVal && archive.BeginTag(serverInfoTag);
		retVal = retVal && archive.Process(name);
		retVal = retVal && archive.Process(description);
		retVal = retVal && archive.Process(version);
		retVal = retVal && archive.Process(computerName);
		retVal = retVal && archive.EndTag(serverInfoTag);

		if (retVal && !archive.IsStoring()){
			serverInfo.stationInfo.SetName(name);
			serverInfo.stationInfo.SetDescription(description);
			serverInfo.stationInfo.SetComputerName(computerName);
		}
	}
	return retVal;
}


// public slots

void CUdpServersManagerComp::OnHeartbeatRead()
{
	while (m_heartbeatSocket.hasPendingDatagrams()){
		int datagramSize = m_heartbeatSocket.pendingDatagramSize();
		Q_ASSERT(datagramSize >= 0);

		QByteArray datagram;
		datagram.resize(datagramSize);
		m_heartbeatSocket.readDatagram(datagram.data(), datagram.size());

		iser::CMemoryReadArchive answerArchive(datagram.data(), datagram.size());

		icomm::CHeartbeatAnswer answer;
		if (answer.Serialize(answerArchive)){
			int questionId = answer.GetQuestionId();
			if (questionId >= 0){
				if (questionId < int(m_servers.size())){
					ServerInfo& serverInfo = m_servers[questionId];
					serverInfo.stationInfo.SetVersion(answer.GetVersion());
					serverInfo.stationInfo.SetStationState(answer.GetStationState());
					serverInfo.stationInfo.SetLoggedUser(answer.GetLoggedUser());
					serverInfo.stationInfo.SetLoggedComputer(answer.GetLoggedComputer());
					serverInfo.stationInfo.SetLoggedIp(answer.GetLoggedIp());
					serverInfo.lostHeartbeats = 0;
				}
			}
			else{
				m_answers.push_back(answer);
			}
		}
	}
}


void CUdpServersManagerComp::OnTimerTick()
{
	if (m_applicationInfoCompPtr.IsValid() && m_includedeartbeatServerPortAttrPtr.IsValid()){
		icomm::CHeartbeatQuestion question;
		question.SetApplicationName(m_applicationInfoCompPtr->GetApplicationAttribute(ibase::IApplicationInfo::AA_APPLICATION_NAME));

		int serversCount = int(m_servers.size());
		for (int i = 0; i < serversCount; ++i){
			ServerInfo& serverInfo = m_servers[i];
			if (serverInfo.lookupId < 0){
				serverInfo.lookupId = QHostInfo::lookupHost(
					serverInfo.stationInfo.GetComputerName(),
								this,
								SLOT(OnHostInfoFound(QHostInfo)));
			}

			if (serverInfo.lostHeartbeats > m_maxLostHeartbeats){
				serverInfo.stationInfo.SetStationState(icomm::CHeartbeatAnswer::S_UNKNOWN);
			}
			else{
				serverInfo.lostHeartbeats++;
			}

			if (!serverInfo.address.isNull()){
				question.SetQuestionId(i);
				iser::CMemoryWriteArchive questionArchive(&m_applicationInfoCompPtr->GetVersionInfo());
				if (question.Serialize(questionArchive)){
					m_heartbeatSocket.writeDatagram(
									(const char*)questionArchive.GetBuffer(),
									questionArchive.GetBufferSize(),
									serverInfo.address,
									*m_includedeartbeatServerPortAttrPtr);
				}
			}
		}
	}
}


void CUdpServersManagerComp::OnHostInfoFound(const QHostInfo& info)
{
	int lookupId = info.lookupId();
	int serversCount = int(m_servers.size());

	for (int i = 0; i < serversCount; ++i){
		ServerInfo& serverInfo = m_servers[i];
		if (serverInfo.lookupId == lookupId){
			QList<QHostAddress> addresses = info.addresses();
			if (!addresses.isEmpty()){
				serverInfo.address = addresses.first();
			}
			else{
				serverInfo.address = QHostAddress::Null;
			}
		}
	}
}


// protected methods

// reimplemented (icomp::CComponentBase)

void CUdpServersManagerComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	if (m_includedeartbeatClientPortAttrPtr.IsValid()){
		m_heartbeatSocket.bind(*m_includedeartbeatClientPortAttrPtr);
	}

	if (m_timerPeriodAttrPtr.IsValid()){
		m_timer.setInterval(int(*m_timerPeriodAttrPtr * 1000));
		m_timer.start();
	}

	if (m_maxLostHeartbeatsAttrPtr.IsValid()){
		m_maxLostHeartbeats = *m_maxLostHeartbeatsAttrPtr;
	}
}


void CUdpServersManagerComp::OnComponentDestroyed()
{
	BaseClass::OnComponentDestroyed();
	m_timer.stop();
}


} // namespace icomm



