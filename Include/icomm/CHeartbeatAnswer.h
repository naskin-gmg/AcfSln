// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#pragma once


// Qt includes
#include <QtCore/QByteArray>

// ACF includes
#include <iser/CMinimalVersionInfo.h>
#include <icomm/IServerInfo.h>
#include <icomm/IStationState.h>
#include <icomm/CHeartbeatQuestion.h>


namespace icomm
{


class CHeartbeatAnswer:
			public icomm::CHeartbeatQuestion,
			virtual public icomm::IServerInfo,
			virtual public icomm::IStationState
{
public:
	typedef icomm::CHeartbeatQuestion BaseClass;

	CHeartbeatAnswer();
	CHeartbeatAnswer(const CHeartbeatQuestion& question);
	CHeartbeatAnswer(const CHeartbeatAnswer& answer);

	const QString& GetComputerName() const;
	void SetComputerName(const QString& name);
	quint32 GetVersion() const;
	void SetVersion(quint32 version);

	virtual void SetStationState(State state);
	virtual void SetLoggedUser(const QString& userName);
	virtual void SetLoggedComputer(const QString& computerName);
	virtual void SetLoggedIp(const QByteArray& ip);

	CHeartbeatAnswer& operator=(const CHeartbeatAnswer& answer);
	bool operator==(const CHeartbeatAnswer& question);
	bool operator!=(const CHeartbeatAnswer& question);

	// reimplemented (ibase::IApplicationInfo)
	virtual QString GetApplicationAttribute(int attributeId, bool allowTranslation = true) const override;
	virtual int GetMainVersionId() const override;
	virtual const iser::IVersionInfo& GetVersionInfo() const override;

	// reimplemented (icomm::IStationState)
	virtual State GetStationState() const override;
	virtual const QString& GetLoggedUser() const override;
	virtual const QString& GetLoggedComputer() const override;
	virtual const QByteArray& GetLoggedIp() const override;

	// reimplemented (iprm::INamParam)
	virtual const QString& GetName() const;
	virtual void SetName(const QString& name);
	virtual bool IsNameFixed() const;

	// reimplemented (qprd::IStationInfo)
	virtual const QString& GetDescription() const;
	virtual void SetDescription(const QString& description);

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive) override;

private:
	quint32 m_versionNumber;
	QString m_computerName;
	QString m_stationName;
	QString m_stationDescription;
	State m_stationState;
	QString m_loggedUser;
	QString m_loggedComputer;
	QByteArray m_loggedIp;

	iser::CMinimalVersionInfo m_versionInfo;
};


// inline methods

inline const QString& CHeartbeatAnswer::GetComputerName() const
{
	return m_computerName;
}


inline void CHeartbeatAnswer::SetComputerName(const QString& name)
{
	m_computerName = name;
}


inline quint32 CHeartbeatAnswer::GetVersion() const
{
	return m_versionNumber;
}


inline void CHeartbeatAnswer::SetVersion(quint32 version)
{
	m_versionNumber = version;
}


inline bool CHeartbeatAnswer::operator!=(const CHeartbeatAnswer& question)
{
	return !operator==(question);
}


} // namespace icomm


