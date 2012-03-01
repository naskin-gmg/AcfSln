#ifndef iauth_ILogin_included
#define iauth_ILogin_included


// Qt includes
#include <QtCore/QString>

// ACF includes
#include "istd/IPolymorphic.h"
#include "iauth/CUser.h"


namespace iauth
{


/*
	Interface supporting login operation.
*/
class ILogin: virtual public istd::IPolymorphic
{
public:
	/**
		Returns the currently logged on user.
		\return	user is currently logged on, or NULL, if no user is logged.
	*/
	virtual CUser* GetLoggedUser() const = 0;

	/**
		Log in user.
		\return	true, if user is accepted.
	*/
	virtual bool Login(const QString& userName, const QString& password) = 0;

	/**
		Log in user.
		\return	true, if logout successful.
	*/
	virtual bool Logout() = 0;
};


} // namespace iauth


#endif // !iauth_ILogin_included
