#ifndef iauth_CUsersManagerComp_included
#define iauth_CUsersManagerComp_included


// STL includes
#include <vector>

// ACF includes
#include "iser/CArchiveTag.h"
#include "icomp/CComponentBase.h"

// ACF-Solutions includes
#include "iauth/IUsersManager.h"
#include "iauth/CUser.h"


namespace iauth
{


/**
	Allows manage of list of users.
*/
class CUsersManagerComp:
			public icomp::CComponentBase,
			virtual public IUsersManager
{
public:
	typedef icomp::CComponentBase BaseClass;

	I_BEGIN_COMPONENT(CUsersManagerComp);
		I_REGISTER_INTERFACE(IUsersManager);
		I_REGISTER_INTERFACE(iser::ISerializable);
		I_ASSIGN_MULTI_0(m_userLevelNamesAttrPtr, "UserLevelNames", "Names of the user levels", false);
		I_ASSIGN_MULTI_0(m_defaultUsersAttrPtr, "DefaultUsers", "List of default users", false);
		I_ASSIGN_MULTI_0(m_defaultUserPasswordsAttrPtr, "DefaultUserPasswords", "List of default user passwords", false);
		I_ASSIGN_MULTI_0(m_defaultUserLevelsAttrPtr, "DefaultUserLevels", "Default user levels", false);
	I_END_COMPONENT;	

	void Reset();

	// reimplemented (iauth::IUsersManager)
	virtual int GetUsersCount() const;
	virtual int FindUserIndex(const istd::CString& name) const;
	virtual const CUser& GetUser(int index) const;
	virtual CUser& GetUserRef(int index);
	virtual CUser* AddUser(const istd::CString& name);
	virtual bool DeleteUser(const istd::CString& name);
	virtual bool RenameUser(int userIndex, const istd::CString& userName);
	virtual int GetUserGroupsCount() const;
	virtual const istd::CString& GetUserGroupName(int groupIndex) const;

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);

protected:
	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated();

private:
	I_MULTIATTR(istd::CString, m_userLevelNamesAttrPtr);
	I_MULTIATTR(istd::CString, m_defaultUsersAttrPtr);
	I_MULTIATTR(istd::CString, m_defaultUserPasswordsAttrPtr);
	I_MULTIATTR(int, m_defaultUserLevelsAttrPtr);

	typedef std::vector<CUser> Users;
	Users m_users;

	typedef std::vector<istd::CString> UserLevels;
	UserLevels m_userLevels;
};


} // namespace iauth


#endif // !iauth_CUsersManagerComp_included


