#ifndef iauth_CUser_included
#define iauth_CUser_included


// ACF includes
#include "istd/CString.h"

#include "iser/IArchive.h"
#include "iser/CArchiveTag.h"


namespace iauth
{


class CUser
{
public:
	CUser(const istd::CString& userName = "", const istd::CString& password = "", int userGroup = 0);
	CUser(const CUser& user);

	/**
		Get name of user.
	*/
	const istd::CString& GetUserName() const;
	/**
		Set name of user.
		\return	true, if user name was accepted, or false if it is uncompatible with the user name guide lines.
	*/
	bool SetUserName(const istd::CString& name);
	/**
		Get group number this user belongs.
		\return number of group, or -1 if this user is disabled.
	*/
	int GetUserGroup() const;
	/**
		Set group number this user belongs.
		\param	group	group number, or -1 if this user is disabled.
	*/
	void SetUserGroup(int group);
	/**
		Get password of this user.
	*/
	const istd::CString& GetPassword() const;
	/**
		Set password of this user.
		\return	true, if password was accepted, or false if it is uncompatible with the password guide lines.
	*/
	bool SetPassword(const istd::CString& password);

	/**
		Check, if password is correct.
	*/
	bool CheckPassword(const istd::CString& password) const;
	/**
		Set password to be reset.
	*/
	void ResetPassword();

	bool Serialize(iser::IArchive& archive);

private:
	istd::CString m_userName;
	istd::CString m_password;
	int m_userGroup;
};


}	// namespace iauth


#endif //iauth_CUser_included



