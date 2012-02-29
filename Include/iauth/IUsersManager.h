#ifndef iauth_IUsersManager_included
#define iauth_IUsersManager_included


// STL includes
#include <vector>

// ACF includes
#include <QtCore/QString>
#include "iser/ISerializable.h"

// ACF-Solutions includes
#include "iauth/CUser.h"


namespace iauth
{


/**
	Provide management of users.
*/
class IUsersManager: public iser::ISerializable
{
public:
	/**
		Get number of users in this collection.
	*/
	virtual int GetUsersCount() const = 0;

	/**
		Search for user with specified name.
		\return		index of user, or negative value, if no user found.
	*/
	virtual int FindUserIndex(const QString& name) const = 0;

	/**
		Get user using its index.
	*/
	virtual const CUser& GetUser(int index) const = 0;

	/**
		Get reference to user object using its index.
	*/
	virtual CUser& GetUserRef(int index) = 0;

	/**
		Add a new user to the user list
		\param QString Username: The user name of the new user. There can be each user name only once.
		\return pointer to user object, if user was added correctly, otherwise NULL.
	*/
	virtual CUser* AddUser(const QString& userName) = 0;

	/**
		Delete the user with the user name
		\param QString Username: The user name of the user which shall be deleted.
		\return true if the user was successfully deleted, otherwise false
	*/
	virtual bool DeleteUser(const QString& userName) = 0;

	/**
		The method renames the user.
		You should not call method CUser::SetUserName() directly,
		becouse of user name is used as primary key of user list.
		\param QString userIndex: user index.
		\param QString userName: the new user name.
		\return true if the user name could be changed, otherwise false.
	*/
	virtual bool RenameUser(int userIndex, const QString& userName) = 0;

	/**
		Get number of user groups.
	*/
	virtual int GetUserGroupsCount() const = 0;

	/**
		Get number of user groups.
	*/
	virtual const QString& GetUserGroupName(int groupIndex) const = 0;
};


}	// namespace iauth


#endif	// !iauth_IUsersManager_included


