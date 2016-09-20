#include <iauth/CUserLoginComp.h>


// ACF includes
#include <istd/CChangeNotifier.h>


namespace iauth
{


const istd::IChangeable::ChangeSet s_loginChangeSet(iauth::ILogin::CF_LOGIN, QObject::tr("Login"));
const istd::IChangeable::ChangeSet s_logoutChangeSet(iauth::ILogin::CF_LOGOUT, QObject::tr("Logout"));


// reimplemented (iauth::ILogin)

CUser* CUserLoginComp::GetLoggedUser() const
{
	if (m_usersManagerIfPtr.IsValid()){
		int userIndex = m_usersManagerIfPtr->FindUserIndex(m_loggedUserName);
		if (userIndex >= 0){
			return &m_usersManagerIfPtr->GetUserRef(userIndex);
		}
	}

	return NULL;
}


bool CUserLoginComp::Login(const QString& userName, const QString& password)
{
	if (m_usersManagerIfPtr.IsValid()){
		int userIndex = m_usersManagerIfPtr->FindUserIndex(userName);
		if (userIndex >= 0){
			CUser& user = m_usersManagerIfPtr->GetUserRef(userIndex);
			if (user.CheckPassword(password)){
				if (m_passwordChangerIfPtr.IsValid() && password.isEmpty()){
					if (!m_passwordChangerIfPtr->TryChangePassword(user)){
						return false;
					}
				}

				istd::CChangeNotifier notifier(this, &s_loginChangeSet);
				Q_UNUSED(notifier);

				m_loggedUserName = userName;

				return true;
			}
		}
	}

	return false;
}


bool CUserLoginComp::Logout()
{
	if (m_usersManagerIfPtr.IsValid()){
		if (!m_loggedUserName.isEmpty()){
			istd::CChangeNotifier notifier(this, &s_logoutChangeSet);
			Q_UNUSED(notifier);

			m_loggedUserName.clear();

			return true;
		}
	}
	return false;
}


// reimplemented (iauth::IRightsProvider)

bool CUserLoginComp::HasRight(const QByteArray& operationId,	bool beQuiet) const
{
	int levelId = 0;
	if (!m_loggedUserName.isEmpty() && m_usersManagerIfPtr.IsValid()){
		int userIndex = m_usersManagerIfPtr->FindUserIndex(m_loggedUserName);
		if (userIndex >= 0){
			const CUser& user = m_usersManagerIfPtr->GetUser(userIndex);
			levelId = user.GetUserGroup();
		}
	}

	if (m_userLevelsIfPtr.IsValid()){
		IRightsProvider* groupRightsPtr = NULL;

		if (levelId < m_userLevelsIfPtr.GetCount()){
			groupRightsPtr = m_userLevelsIfPtr[levelId];
		}
		else if (m_userLevelsIfPtr.GetCount() > 0){
			groupRightsPtr = m_userLevelsIfPtr[0];
		}

		if (groupRightsPtr != NULL){
			return groupRightsPtr->HasRight(operationId, beQuiet);
		}
	}

	return false;
}


} // namespace iauth


