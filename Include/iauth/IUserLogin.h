#ifndef iauth_IUserLogin_included
#define iauth_IUserLogin_included


// ACF-Solutions includes
#include "iauth/ILogin.h"
#include "iauth/CUser.h"


namespace iauth
{


/**
	Interface supporting login operation.
*/
class IUserLogin: public ILogin
{
public:
	/**
		Returns the currently logged on user.
		\return	user is currently logged on, or NULL, if no user is logged.
	*/
	virtual CUser* GetLoggedUser() const = 0;
};


} // namespace iauth


#endif // !iauth_IUserLogin_included


