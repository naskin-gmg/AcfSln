#include "iqtauth/CUserGuiComp.h"


// ACF includes
#include "istd/TChangeNotifier.h"
#include "iser/CMemoryReadArchive.h"

// ACF-Solutions includes
#include "iqtauth/CChangePasswordDialog.h"
#include "iqtauth/CUserManagerDialog.h"


namespace iqtauth
{


// reimplemented (iauth::IPasswordChanger)

bool CUserGuiComp::TryChangePassword(iauth::CUser& user) const
{
	istd::TChangeNotifier<iauth::IUsersManager> managerPtr(GetObjectPtr());
	if (managerPtr.IsValid()){
		CChangePasswordDialog dialog(*managerPtr, user);
		dialog.exec();

		return true;
	}

	return false;
}


// protected methods

void CUserGuiComp::UpdateButtonsState()
{
	if (!IsGuiCreated()){
		return;
	}

	bool isUserLogged = m_userLoginIfPtr.IsValid() && (m_userLoginIfPtr->GetLoggedUser() != NULL);
	bool isUsersManagerAllowed = isUserLogged;
	bool isChangePasswordAllowed = isUserLogged;

	if (isUserLogged && m_rightsProviderIfPtr.IsValid()){
		if (m_usersManagerRightIdAttrPtr.IsValid()){
			isUsersManagerAllowed = m_rightsProviderIfPtr->HasRight((*m_usersManagerRightIdAttrPtr).toStdString(), false);
		}

		if (m_changePasswordRightIdAttrPtr.IsValid()){
			isChangePasswordAllowed = m_rightsProviderIfPtr->HasRight((*m_changePasswordRightIdAttrPtr).toStdString(), false);
		}
	}

	PushOpenUserManager->setEnabled(isUsersManagerAllowed);
	PushChangePassword->setEnabled(isChangePasswordAllowed);
}


// reimplemented (iqtgui::CGuiComponentBase)

void CUserGuiComp::OnGuiShown()
{
	UpdateButtonsState();
}


// reimplemented (icomp::CComponentBase)

void CUserGuiComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	if (m_rightsProviderIfPtr.IsValid()){
		RegisterModel(m_rightsModelIfPtr.GetPtr());
	}
}


void CUserGuiComp::OnComponentDestroyed()
{
	UnregisterModel();

	BaseClass::OnComponentDestroyed();
}


// reimplemented (imod::CMultiModelDispatcherBase)

void CUserGuiComp::OnModelChanged(int /*modelId*/, int /*changeFlags*/, istd::IPolymorphic* /*updateParamsPtr*/)
{
	UpdateButtonsState();
}


// protected slots

void CUserGuiComp::on_PushChangePassword_clicked()
{
	if (m_userLoginIfPtr.IsValid()){
		iauth::CUser* loggedUserPtr = m_userLoginIfPtr->GetLoggedUser();
		if (loggedUserPtr != NULL){
			TryChangePassword(*loggedUserPtr);
		}
	}
}


void CUserGuiComp::on_PushOpenUserManager_clicked()
{

	if (m_userLoginIfPtr.IsValid()){
		istd::TChangeNotifier<iauth::IUsersManager> managerPtr(GetObjectPtr());
		if (managerPtr.IsValid()){
			CUserManagerDialog dialog(*m_userLoginIfPtr, *managerPtr);

			dialog.exec();
		}
	}
}


} // namespace iqtauth


