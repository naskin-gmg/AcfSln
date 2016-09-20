#include <iqtauth/CUserGuiComp.h>


// ACF includes
#include <istd/CChangeNotifier.h>
#include <iser/CMemoryReadArchive.h>

// ACF-Solutions includes
#include <iqtauth/CChangePasswordDialog.h>
#include <iqtauth/CUserManagerDialog.h>


namespace iqtauth
{


CUserGuiComp::CUserGuiComp()
	:m_userManagerDialogPtr(NULL),
	m_passwordDialogPtr(NULL)
{
}


// reimplemented (iauth::IPasswordChanger)

bool CUserGuiComp::TryChangePassword(iauth::CUser& user) const
{
	iauth::IUsersManager* managerPtr = GetObservedObject();
	if (managerPtr != NULL){
		istd::CChangeNotifier notifier(managerPtr);

		CChangePasswordDialog dialog(*managerPtr, user);
		m_passwordDialogPtr = &dialog;
		dialog.exec();
		m_passwordDialogPtr = NULL;

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
	UnregisterAllModels();

	BaseClass::OnComponentDestroyed();
}


// reimplemented (imod::CMultiModelDispatcherBase)

void CUserGuiComp::OnModelChanged(int /*modelId*/, const istd::IChangeable::ChangeSet& /*changeSet*/)
{
	if (m_userLoginIfPtr.IsValid() && m_userLoginIfPtr->GetLoggedUser() == NULL){
		if (m_passwordDialogPtr != NULL){
			m_passwordDialogPtr->reject();
			m_passwordDialogPtr = NULL;
		}

		if (m_userManagerDialogPtr != NULL){
			m_userManagerDialogPtr->reject();
			m_userManagerDialogPtr = NULL;
		}
	}

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
		iauth::IUsersManager* managerPtr = GetObservedObject();
		if (managerPtr != NULL){
			istd::CChangeNotifier notifier(managerPtr);

			CUserManagerDialog dialog(*m_userLoginIfPtr, *managerPtr);
			m_userManagerDialogPtr = &dialog;
			dialog.exec();
			m_userManagerDialogPtr = NULL;
		}
	}
}


} // namespace iqtauth


