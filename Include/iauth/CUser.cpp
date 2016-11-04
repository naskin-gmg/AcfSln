#include <iauth/CUser.h>


// ACF includes
#include <istd/CChangeNotifier.h>
#include <iser/IArchive.h>
#include <iser/CArchiveTag.h>


namespace iauth
{


CUser::CUser(const QString& userName, const QString& password, int userGroup)
:	m_userName(userName), m_password(password), m_userGroup(userGroup)
{
}


CUser::CUser(const CUser& user)
{
	m_userName = user.m_userName;
	m_password = user.m_password;
	m_userGroup = user.m_userGroup;
}


const QString& CUser::GetUserName() const
{
	return m_userName;
}


bool CUser::SetUserName(const QString& name)
{
	m_userName = name;

	return true;
}


bool CUser::SetPassword(const QString& password)
{
	if (!password.isEmpty()){
		m_password = password;

		return true;
	}

	return false;
}


const QString& CUser::GetPassword() const
{
	return m_password;
}


int CUser::GetUserGroup() const
{
	return m_userGroup;
}


void CUser::SetUserGroup(int group)
{
	m_userGroup = group;
}


bool CUser::CheckPassword(const QString& password) const
{
	return (m_password == password);
}


void CUser::ResetPassword()
{
	m_password = "";
}


// reimplemented (iser::ISerializable)

bool CUser::Serialize(iser::IArchive& archive)
{
	static iser::CArchiveTag nameTag("Name", "Name of User", iser::CArchiveTag::TT_LEAF);
	static iser::CArchiveTag passwordTag("Password", "Password", iser::CArchiveTag::TT_LEAF);
	static iser::CArchiveTag groupIdTag("GroupID", "ID of group", iser::CArchiveTag::TT_LEAF);

	istd::CChangeNotifier notifier(archive.IsStoring()? NULL: this);
	Q_UNUSED(notifier);

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


