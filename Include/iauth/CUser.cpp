#include "iauth/CUser.h"


namespace iauth
{


CUser::CUser(const istd::CString& userName, const istd::CString& password, int userGroup)
:	m_userName(userName), m_password(password), m_userGroup(userGroup)
{
}


CUser::CUser(const CUser& user)
{
	m_userName = user.m_userName;
	m_password = user.m_password;
	m_userGroup = user.m_userGroup;
}


const istd::CString& CUser::GetUserName() const
{
	return m_userName;
}


bool CUser::SetUserName(const istd::CString& name)
{
	m_userName = name;

	return true;
}


const istd::CString& CUser::GetPassword() const
{
	return m_password;
}


bool CUser::SetPassword(const istd::CString& password)
{
	if (!password.IsEmpty()){
		m_password = password;

		return true;
	}

	return false;
}


int CUser::GetUserGroup() const
{
	return m_userGroup;
}


void CUser::SetUserGroup(int group)
{
	m_userGroup = group;
}


bool CUser::CheckPassword(const istd::CString& password) const
{
	return (m_password == password);
}


void CUser::ResetPassword()
{
	m_password = "";
}


bool CUser::Serialize(iser::IArchive& archive)
{
	static iser::CArchiveTag nameTag("Name", "Name of User");
	static iser::CArchiveTag passwordTag("Password", "Password");
	static iser::CArchiveTag groupIdTag("GroupID", "ID of group");

	bool retVal = true;

	retVal = retVal && archive.BeginTag(nameTag);
	retVal = retVal && archive.Process(m_userName);
	retVal = retVal && archive.EndTag(nameTag);

	retVal = retVal && archive.BeginTag(passwordTag);
	retVal = retVal && archive.Process(m_password);
	retVal = retVal && archive.EndTag(passwordTag);

	retVal = retVal && archive.BeginTag(groupIdTag);
	retVal = retVal && archive.Process(m_userGroup);
	retVal = retVal && archive.EndTag(groupIdTag);

	return retVal;
}


}	// namespace iauth


