#ifndef iauth_CSimpleLoginComp_included
#define iauth_CSimpleLoginComp_included


// ACF includes
#include <icomp/CComponentBase.h>
#include <imod/TModelWrap.h>
#include <iser/ISerializable.h>
#include <iser/CArchiveTag.h>

// ACF-Solutions includes
#include <iauth/ILogin.h>
#include <iauth/IRightsProvider.h>
#include <iauth/CUser.h>


namespace iauth
{


class CSimpleLoginComp:
			public icomp::CComponentBase,
			public ILogin,
			public IRightsProvider,
			public iser::ISerializable
{
public:
	typedef icomp::CComponentBase BaseClass;

	I_BEGIN_COMPONENT(CSimpleLoginComp);
		I_REGISTER_INTERFACE(ILogin);
		I_REGISTER_INTERFACE(IRightsProvider);
		I_REGISTER_INTERFACE(iser::ISerializable);
		I_ASSIGN_MULTI_0(m_userLevelsIfPtr, "UserLevels", "User levels", true);
		I_ASSIGN_MULTI_0(m_defaultUsersAttrPtr, "DefaultUsers", "DefaultUsers", false);
		I_ASSIGN_MULTI_0(m_defaultUserPasswordsAttrPtr, "DefaultUserPasswords", "DefaultUserPasswords", false);
		I_ASSIGN_MULTI_0(m_defaultUserLevelsAttrPtr, "DefaultUserLevels", "DefaultUserLevels", false);
	I_END_COMPONENT;

	CSimpleLoginComp();

	virtual int GetUsersCount() const;
	virtual CUser& GetUser(int index) const;
	virtual int FindUserIndex(const QString& name) const;
	virtual bool AddUser(const CUser& user);

	// reimplemented (iauth::ILogin)
	virtual CUser* GetLoggedUser() const;
	virtual bool Login(const QString& userName, const QString& password);
	virtual bool Logout();

	// reimplemented (iauth::IRightsProvider)
	virtual bool HasRight(
					const QByteArray& operationId,
					bool beQuiet = false) const;

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);

protected:
	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated();

private:
	I_MULTIREF(IRightsProvider, m_userLevelsIfPtr);
	I_MULTIATTR(QString, m_defaultUsersAttrPtr);
	I_MULTIATTR(QString, m_defaultUserPasswordsAttrPtr);
	I_MULTIATTR(int, m_defaultUserLevelsAttrPtr);

	typedef QVector<CUser> Users;
	Users m_users;

	int m_loggedUserIndex;
};


} // namespace iauth


#endif // !iauth_CSimpleLoginComp_included


