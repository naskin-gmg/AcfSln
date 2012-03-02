#include "iauth/CSimpleLoginComp.h"


// ACF includes
#include "istd/TChangeNotifier.h"

#include "iser/IArchive.h"


namespace iauth
{


CSimpleLoginComp::CSimpleLoginComp()
:	m_loggedUserIndex(-1)
{
}


int CSimpleLoginComp::GetUsersCount() const
{
	return int(m_users.size());
}


CUser& CSimpleLoginComp::GetUser(int index) const
{
	I_ASSERT(index >= 0);
	I_ASSERT(index < int(m_users.size()));

	return const_cast<CUser&>(m_users[index]);
}


int CSimpleLoginComp::FindUserIndex(const QString& name) const
{
	int usersCount = GetUsersCount();

	for (int i = 0; i < usersCount; ++i){
		CUser& user = GetUser(i);

		if (user.GetUserName() == name){
			return i;
		}
	}

	return -1;
}


bool CSimpleLoginComp::AddUser(const CUser& user)
{
	if (FindUserIndex(user.GetUserName()) < 0){
		m_users.push_back(user);

		return true;
	}

	return false;
}


// reimplemented (iauth::ILogin)

CUser* CSimpleLoginComp::GetLoggedUser() const
{
	if (m_loggedUserIndex >= 0){
		return const_cast<CUser*>(&m_users[m_loggedUserIndex]);
	}

	return NULL;
}


bool CSimpleLoginComp::Login(const QString& userName, const QString& password)
{
	int userIndex = FindUserIndex(userName);
	if (userIndex >= 0){
		CUser& user = GetUser(userIndex);

		if (user.CheckPassword(password)){
			istd::TChangeNotifier<IRightsProvider> updatePtr(this);

			m_loggedUserIndex = userIndex;

			return true;
		}
	}

	return false;
}


bool CSimpleLoginComp::Logout()
{
	if (m_loggedUserIndex >= 0){
		istd::TChangeNotifier<IRightsProvider> updatePtr(this);

		m_loggedUserIndex = -1;

		return true;
	}

	return false;
}


// reimplemented (iauth::IRightsProvider)

bool CSimpleLoginComp::HasRight(
			const std::string& operationId,
			bool beQuiet) const
{
	int groupId = 0;
	if (m_loggedUserIndex >= 0){
		I_ASSERT(m_loggedUserIndex < GetUsersCount());

		const CUser& user = GetUser(m_loggedUserIndex);

		int userGroupId = user.GetUserGroup();
		I_ASSERT(userGroupId < m_userLevelsIfPtr.GetCount());

		if (userGroupId >= 0){
			groupId = userGroupId + 1;
		}
	}

	IRightsProvider* groupRightsPtr = m_userLevelsIfPtr[groupId];	
	if (groupRightsPtr != NULL){
		return groupRightsPtr->HasRight(operationId, beQuiet);
	}

	return false;
}


// reimplemented (iser::ISerializable)

bool CSimpleLoginComp::Serialize(iser::IArchive& archive)
{
	static iser::CArchiveTag usersTag("Users", "List of users");
	static iser::CArchiveTag userTag("User", "User");

	int usersCount = GetUsersCount();

	bool retVal = archive.BeginMultiTag(usersTag, userTag, usersCount);
	if (!retVal){
		return false;
	}

	if (!archive.IsStoring()){
		m_users.resize(usersCount);
	}

	for (int i = 0; i < usersCount; ++i){
		CUser& user = m_users[i];

		retVal = retVal && archive.BeginTag(userTag);

		retVal = retVal && user.Serialize(archive);

		retVal = retVal && archive.EndTag(userTag);
	}

	retVal = retVal && archive.EndTag(usersTag);

	return retVal;
}


// protected methods

// reimplemented (icomp::CComponentBase)

void CSimpleLoginComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	if (m_defaultUsersAttrPtr.IsValid()){
		int groupsCount = m_userLevelsIfPtr.GetCount();
		if (groupsCount > 0){
			int usersCount = m_defaultUsersAttrPtr.GetCount();
			for (int i = 0; i < usersCount; ++i){
				CUser user;
				user.SetUserName(m_defaultUsersAttrPtr[i]);

				if (		m_defaultUserPasswordsAttrPtr.IsValid() &&
							(i < m_defaultUserPasswordsAttrPtr.GetCount())){
					user.SetPassword(m_defaultUserPasswordsAttrPtr[i]);
				}

				if (		m_defaultUserLevelsAttrPtr.IsValid() &&
							(i < m_defaultUserLevelsAttrPtr.GetCount())){
					int group = m_defaultUserLevelsAttrPtr[i];
					if (group < groupsCount){
						user.SetUserGroup(m_defaultUserLevelsAttrPtr[i]);
					}
					else{
						user.SetUserGroup(groupsCount - 1);
					}
				}
				else{
					user.SetUserGroup(0);
				}

				AddUser(user);
			}
		}
	}
}


} // namespace iauth


