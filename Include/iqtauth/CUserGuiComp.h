#ifndef _qqtauth_CUserGuiComp_included
#define _qqtauth_CUserGuiComp_included


// ACF includes
#include <imod/CMultiModelDispatcherBase.h>
#include <ilog/TLoggerCompWrap.h>
#include <iqtgui/TDesignerGuiObserverCompBase.h>

// ACF-Solutions includes
#include <iauth/IRightsProvider.h>
#include <iauth/ILogin.h>
#include <iauth/IUsersManager.h>
#include <iauth/IPasswordChanger.h>

#include <GeneratedFiles/iqtauth/ui_CUserGuiComp.h>


namespace iqtauth
{


class CUserGuiComp:
			public ilog::TLoggerCompWrap<
						iqtgui::TDesignerGuiObserverCompBase<
									Ui::CUserGuiComp, iauth::IUsersManager> >,
			protected imod::CMultiModelDispatcherBase,
			virtual public iauth::IPasswordChanger
{
	Q_OBJECT

public:
	typedef ilog::TLoggerCompWrap<
				iqtgui::TDesignerGuiObserverCompBase<
							Ui::CUserGuiComp, iauth::IUsersManager> > BaseClass;

	I_BEGIN_COMPONENT(CUserGuiComp);
		I_REGISTER_INTERFACE(iauth::IPasswordChanger);
		I_ASSIGN(m_rightsProviderIfPtr, "RightsProvider", "RightsProvider", false, "RightsProvider");
		I_ASSIGN_TO(m_rightsModelIfPtr, m_rightsProviderIfPtr, true);
		I_ASSIGN_TO(m_userLoginIfPtr, m_rightsProviderIfPtr, true);
		I_ASSIGN(m_usersManagerRightIdAttrPtr, "UsersManagerRightId", "Right ID for user manager", false, "");
		I_ASSIGN(m_changePasswordRightIdAttrPtr, "ChangePasswordRightsId", "Right ID for changing password", false, "");
	I_END_COMPONENT;

	CUserGuiComp();

	// reimplemented (iauth::IPasswordChanger)
	virtual bool TryChangePassword(iauth::CUser& user) const;

protected:
	void UpdateButtonsState();

	// reimplemented (iqtgui::CGuiComponentBase)
	virtual void OnGuiShown();

	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated() override;
	virtual void OnComponentDestroyed() override;

	// reimplemented (imod::CMultiModelDispatcherBase)
	virtual void OnModelChanged(int modelId, const istd::IChangeable::ChangeSet& changeSet);

protected Q_SLOTS:
	virtual void on_PushChangePassword_clicked();
	virtual void on_PushOpenUserManager_clicked();

private:
	I_REF(iauth::IRightsProvider, m_rightsProviderIfPtr);
	I_REF(imod::IModel, m_rightsModelIfPtr);
	I_REF(iauth::ILogin, m_userLoginIfPtr);

	I_ATTR(QByteArray, m_usersManagerRightIdAttrPtr);
	I_ATTR(QByteArray, m_changePasswordRightIdAttrPtr);

	mutable class CUserManagerDialog* m_userManagerDialogPtr;
	mutable class CChangePasswordDialog* m_passwordDialogPtr;
};

} // namespace iqtauth


#endif	// !_qqtauth_CUserGuiComp_included


