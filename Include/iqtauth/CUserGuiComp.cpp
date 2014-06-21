#include "iqtauth/CUserGuiComp.h"


// ACF includes
#include "istd/CChangeNotifier.h"
#include "iser/CMemoryReadArchive.h"

// ACF-Solutions includes
#include "iqtauth/CChangePasswordDialog.h"
#include "iqtauth/CUserManagerDialog.h"


namespace iqtauth
{


// reimplemented (iauth::IPasswordChanger)

bool CUserGuiComp::TryChangePassword(iauth::CUser& user) const
{
	iauth::IUsersManager* managerPtr = GetObjectPtr();
	if (managerPtr != NULL){
		istd::CChangeNotifier notifier(managerPtr);

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
			isUsersManagerAllowed = m_rightsProviderIfPtr->HasRight(*m_usersManagerRightIdAttrPtr, false);
		}

		if (m_changePasswordRightIdAttrPtr.IsValid()){
			isChangePasswordAllowed = m_rightsProviderIfPtr->HasRight(*m_changePasswordRightIdAttrPtr, false);
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

void CUserGuiComp::OnModelChanged(int /*modelId*/, const istd::IChangeable::ChangeSet& /*changeSet*/)
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
		iauth::IUsersManager* managerPtr = GetObjectPtr();
		if (managerPtr != NULL){
			istd::CChangeNotifier notifier(managerPtr);

			CUserManagerDialog dialog(*m_userLoginIfPtr, *managerPtr);

			dialog.exec();
		}
	}
}


} // namespace iqtauth


