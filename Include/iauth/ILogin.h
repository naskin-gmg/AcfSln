#ifndef iauth_ILogin_included
#define iauth_ILogin_included


// ACF includes
#include "istd/IPolymorphic.h"
#include <QString>


namespace iauth
{


/*
	Interface supporting login operation.
*/
class ILogin: virtual public istd::IPolymorphic
{
public:
	/**
		Check, if any user is logged to the system.
	*/
	virtual bool IsUserLogged() const = 0;
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
