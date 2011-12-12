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


CSimpleLoginComp::User& CSimpleLoginComp::GetUser(int index) const
{
	I_ASSERT(index >= 0);
	I_ASSERT(index < int(m_users.size()));

	return const_cast<User&>(m_users[index]);
}


int CSimpleLoginComp::FindUserIndex(const istd::CString& name) const
{
	int usersCount = GetUsersCount();

	for (int i = 0; i < usersCount; ++i){
		User& user = GetUser(i);

		if (user.name == name){
			return i;
		}
	}

	return -1;
}


bool CSimpleLoginComp::AddUser(const User& user)
{
	if (FindUserIndex(user.name) < 0){
		m_users.push_back(user);

		return true;
	}

	return false;
}


// reimplemented (iauth::ILogin)

bool CSimpleLoginComp::IsUserLogged() const
{
	return (m_loggedUserIndex >= 0);
}


bool CSimpleLoginComp::Login(const istd::CString& userName, const istd::CString& password)
{
	int userIndex = FindUserIndex(userName);
	if (userIndex >= 0){
		User& user = GetUser(userIndex);

		if (user.password == password){
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

		const User& user = GetUser(m_loggedUserIndex);

		I_ASSERT(user.group < m_userLevelsIfPtr.GetCount());

		if (user.group >= 0){
			groupId = user.group + 1;
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
	static iser::CArchiveTag nameTag("Name", "Name of User");
	static iser::CArchiveTag passwordTag("Password", "Password");
	static iser::CArchiveTag groupIdTag("GroupID", "ID of group");

	int usersCount = GetUsersCount();

	bool retVal = archive.BeginMultiTag(usersTag, userTag, usersCount);
	if (!retVal){
		return false;
	}

	if (!archive.IsStoring()){
		m_users.resize(usersCount);
	}

	for (int i = 0; i < usersCount; ++i){
		User& user = m_users[i];

		retVal = retVal && archive.BeginTag(userTag);

		retVal = retVal && archive.BeginTag(nameTag);
		retVal = retVal && archive.Process(user.name);
		retVal = retVal && archive.EndTag(nameTag);

		retVal = retVal && archive.BeginTag(passwordTag);
		retVal = retVal && archive.Process(user.password);
		retVal = retVal && archive.EndTag(passwordTag);

		retVal = retVal && archive.BeginTag(groupIdTag);
		retVal = retVal && archive.Process(user.group);
		retVal = retVal && archive.EndTag(groupIdTag);

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
				User user;
				user.name = m_defaultUsersAttrPtr[i];

				if (		m_defaultUserPasswordsAttrPtr.IsValid() &&
							(i < m_defaultUserPasswordsAttrPtr.GetCount())){
					user.password = m_defaultUserPasswordsAttrPtr[i];
				}

				if (		m_defaultUserLevelsAttrPtr.IsValid() &&
							(i < m_defaultUserLevelsAttrPtr.GetCount())){
					int group = m_defaultUserLevelsAttrPtr[i];
					if (group < groupsCount){
						user.group = m_defaultUserLevelsAttrPtr[i];
					}
					else{
						user.group = groupsCount - 1;
					}
				}
				else{
					user.group = 0;
				}

				AddUser(user);
			}
		}
	}
}


} // namespace iauth


