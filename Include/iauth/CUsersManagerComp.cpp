#include <iauth/CUsersManagerComp.h>


// ACF includes
#include <istd/CChangeNotifier.h>
#include <iser/IArchive.h>


namespace iauth
{


const istd::IChangeable::ChangeSet s_addUserChangeSet(iauth::IUsersManager::CF_USER_ADDED, QObject::tr("Add user"));
const istd::IChangeable::ChangeSet s_removeUserChangeSet(iauth::IUsersManager::CF_USER_REMOVED, QObject::tr("User remove"));
const istd::IChangeable::ChangeSet s_renameUserChangeSet(iauth::IUsersManager::CF_USER_RENAMED, QObject::tr("User renamed"));


// reimplemented (iauth::IUsersManager)

int CUsersManagerComp::GetUsersCount() const
{
	return m_users.count();
}


int CUsersManagerComp::FindUserIndex(const QString& name) const
{
	int usersCount = m_users.count();

	for (int i = 0; i < usersCount; ++i){
		if (m_users[i].GetUserName() == name){
			return i;
		}
	}

	return -1;
}


const CUser& CUsersManagerComp::GetUser(int index) const
{
	Q_ASSERT(index >= 0);
	Q_ASSERT(index < int(m_users.size()));

	return m_users[index];
}


CUser& CUsersManagerComp::GetUserRef(int index)
{
	Q_ASSERT(index >= 0);
	Q_ASSERT(index < int(m_users.size()));

	return m_users[index];
}


void CUsersManagerComp::Reset()
{
	m_users.clear();

	int groupsCount = GetUserGroupsCount();
	if ((groupsCount < 1) || !m_defaultUsersAttrPtr.IsValid()){
		return;
	}

	int usersCount = int(m_defaultUsersAttrPtr.GetCount());
	for (int i = 0; i < usersCount; ++i){
		QString userName(m_defaultUsersAttrPtr[i]);
		QString password;
		if (			m_defaultUserPasswordsAttrPtr.IsValid() &&
						(i < m_defaultUserPasswordsAttrPtr.GetCount())){
			password = m_defaultUserPasswordsAttrPtr[i];
		}

		int userGroup = 0;
		if (			m_defaultUserLevelsAttrPtr.IsValid() &&
						(i < m_defaultUserLevelsAttrPtr.GetCount())){
			userGroup = m_defaultUserLevelsAttrPtr[i];
		}

		if (userGroup >= groupsCount){
			userGroup = groupsCount - 1;
		}

		CUser* userPtr = AddUser(userName);
		if (userPtr != NULL){
			userPtr->SetPassword(password);
			userPtr->SetUserGroup(userGroup);
		}
	}
}


// reimplemented (iauth::IUsersManager)

CUser* CUsersManagerComp::AddUser(const QString& userName)
{
	if (FindUserIndex(userName) >= 0){
		return NULL;
	}

	istd::CChangeNotifier changePtr(this, &s_addUserChangeSet);

	m_users.push_back(CUser(userName));

	return &m_users.back();
}


bool CUsersManagerComp::DeleteUser(const QString& userName)
{
	for (Users::iterator iter = m_users.begin(); iter != m_users.end(); ++iter){
		if (iter->GetUserName() == userName){
			istd::CChangeNotifier changePtr(this, &s_removeUserChangeSet);
			
			m_users.erase(iter);

			return true;
		}
	}

	return false;
}


bool CUsersManagerComp::RenameUser(int userIndex, const QString& userName)
{
	Q_ASSERT(userIndex >= 0);
	Q_ASSERT(userIndex < int(m_users.size()));

	// checking if the new user name does not exist a second time
	if (FindUserIndex(userName) >= 0){
		return false;
	}

	istd::CChangeNotifier notifier(this, &s_renameUserChangeSet);
	Q_UNUSED(notifier);

	return m_users[userIndex].SetUserName(userName);
}


int CUsersManagerComp::GetUserGroupsCount() const
{
	if (m_userLevelNamesAttrPtr.IsValid()){
		return int(m_userLevelNamesAttrPtr.GetCount());
	}

	return 0;
}


const QString& CUsersManagerComp::GetUserGroupName(int groupIndex) const
{
	Q_ASSERT(groupIndex >= 0);
	Q_ASSERT(groupIndex < int(m_userLevels.size()));

	return m_userLevels[groupIndex];
}


// reimplemented (iser::ISerializable)

bool CUsersManagerComp::Serialize(iser::IArchive& archive)
{
	static iser::CArchiveTag usersTag("Users", "List of users", iser::CArchiveTag::TT_MULTIPLE);
	static iser::CArchiveTag userTag("User", "User", iser::CArchiveTag::TT_GROUP, &usersTag);

	bool isStoring = archive.IsStoring();
	istd::CChangeNotifier notifier(isStoring? NULL: this);
	Q_UNUSED(notifier);

	int usersCount = GetUsersCount();

	bool retVal = archive.BeginMultiTag(usersTag, userTag, usersCount);

	if (retVal){
		if (!isStoring){
			m_users.resize(usersCount);
		}

		for (int i = 0; retVal && i < usersCount; ++i){
			CUser& user = m_users[i];

			retVal = retVal && archive.BeginTag(userTag);
			retVal = retVal && user.Serialize(archive);
			retVal = retVal && archive.EndTag(userTag);
		}	

		retVal = retVal && archive.EndTag(usersTag);
	}

	if (!retVal){
		Reset();
	}

	return retVal;
}


// protected methods

// reimplemented (icomp::CComponentBase)

void CUsersManagerComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	Reset();

	int levelsCount = int(m_userLevelNamesAttrPtr.GetCount());

	m_userLevels.resize(levelsCount);

	for (int i = 0; i < levelsCount; ++i){
		m_userLevels[i] = m_userLevelNamesAttrPtr[i];
	}
}


}	// namespace iauth


