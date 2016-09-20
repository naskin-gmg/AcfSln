#ifndef AuthorizationPckPck_included
#define AuthorizationPckPck_included


// ACF includes
#include <icomp/TModelCompWrap.h>
#include <icomp/TMakeComponentWrap.h>

// ACF-Solutions includes
#include <iauth/CStaticRightsProviderComp.h>
#include <iauth/CUserLoginComp.h>
#include <iauth/CSimpleLoginComp.h>
#include <iauth/CUsersManagerComp.h>
#include <iauth/CRightsBasedEnablerComp.h>

/**
	Package for security and authorization components.
*/
namespace AuthorizationPck
{


typedef iauth::CStaticRightsProviderComp StaticRightsProvider;
typedef icomp::TModelCompWrap<iauth::CUserLoginComp> UserLogin;
typedef icomp::TModelCompWrap<iauth::CSimpleLoginComp> SimpleLogin;
typedef icomp::TModelCompWrap<iauth::CUsersManagerComp> UsersManager;
typedef icomp::TModelCompWrap<iauth::CRightsBasedEnablerComp> RightsBasedEnabler;


} // namespace AuthorizationPck


#endif // !AuthorizationPckPck_included


