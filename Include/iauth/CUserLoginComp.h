#ifndef iauth_CUserLoginComp_included
#define iauth_CUserLoginComp_included


// ACF includes
#include <imod/TModelWrap.h>
#include <icomp/CComponentBase.h>

// ACF-Solutions includes
#include <iauth/IRightsProvider.h>
#include <iauth/ILogin.h>
#include <iauth/IUsersManager.h>
#include <iauth/IPasswordChanger.h>


namespace iauth
{

/**
	Implementation of user login allowing to get rights for current logged user.
*/
class CUserLoginComp:
			public icomp::CComponentBase,
			virtual public ILogin,
			virtual public IRightsProvider
{
public:
	typedef icomp::CComponentBase BaseClass;

	I_BEGIN_COMPONENT(CUserLoginComp);
		I_REGISTER_INTERFACE(IRightsProvider);
		I_REGISTER_INTERFACE(ILogin);
		I_REGISTER_INTERFACE(ILogin);
		I_ASSIGN_MULTI_0(m_userLevelsIfPtr, "UserLevels", "User levels", true);
		I_ASSIGN(m_usersManagerIfPtr, "UsersManager", "Users manager", true, "UsersManager");
		I_ASSIGN(m_passwordChangerIfPtr, "PasswordChanger", "Passwort manager", false, "PasswordChanger");
	I_END_COMPONENT;		

	// reimplemented (iauth::ILogin)
	virtual CUser* GetLoggedUser() const;
	virtual bool Login(const QString& userName, const QString& password);
	virtual bool Logout();

	// reimplemented (iauth::IRightsProvider)
	virtual bool HasRight(
					const QByteArray& operationId,
					bool beQuiet = false) const;

private:
	I_MULTIREF(IRightsProvider, m_userLevelsIfPtr);
	I_REF(IUsersManager, m_usersManagerIfPtr);
	I_REF(IPasswordChanger, m_passwordChangerIfPtr);

	QString m_loggedUserName;
};


} // namespace iauth


#endif // !iauth_CUserLoginComp_included


