#ifndef iauth_CUser_included
#define iauth_CUser_included


// Qt includes
#include <QtCore/QString>

// ACF includes
#include <iser/ISerializable.h>


namespace iauth
{


class CUser: virtual public iser::ISerializable
{
public:
	CUser(const QString& userName = "", const QString& password = "", int userGroup = 0);
	CUser(const CUser& user);

	/**
		Get name of user.
	*/
	const QString& GetUserName() const;

	/**
		Get user's password.
	*/
	const QString& GetPassword() const;

	/**
		Set name of user.
		\return	true, if user name was accepted, or false if it is uncompatible with the user name guide lines.
	*/
	bool SetUserName(const QString& name);

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
		Set password of this user.
		\return	true, if password was accepted, or false if it is uncompatible with the password guide lines.
	*/
	bool SetPassword(const QString& password);

	/**
		Check, if password is correct.
	*/
	bool CheckPassword(const QString& password) const;

	/**
		Set password to be reseted.
	*/
	void ResetPassword();

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);

private:
	QString m_userName;
	QString m_password;
	int m_userGroup;
};


}	// namespace iauth


#endif //iauth_CUser_included



