#ifndef iauth_IPasswordChanger_included
#define iauth_IPasswordChanger_included


// ACF includes
#include "istd/IPolymorphic.h"

// ACF-Solutions includes
#include "iauth/CUser.h"


namespace iauth
{


class IPasswordChanger: virtual public istd::IPolymorphic
{
public:
	/**
		Try change user password.
		\return	true	if success.
	*/
	virtual bool TryChangePassword(CUser& user) const = 0;
};


} // namespace iauth


#endif	// !iauth_IPasswordChanger_included


