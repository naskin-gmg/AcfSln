#include "iqtauth/CChangePasswordDialog.h"


// Qt includes
#include<QtCore/QtGlobal>
#if QT_VERSION >= 0x050000
#include <QtWidgets/QMessageBox>
#else
#include <QtGui/QMessageBox>
#endif

// ACF includes
#include "iqt/iqt.h"
#include "istd/CChangeNotifier.h"


namespace iqtauth
{


CChangePasswordDialog::CChangePasswordDialog(iauth::IUsersManager& manager, iauth::CUser& loggedUser)
:	m_usersManager(manager),
	m_loggedUser(loggedUser)
{
	setupUi(this);

	setWindowTitle(windowTitle().arg(m_loggedUser.GetUserName()));
}


// protected methods

void CChangePasswordDialog::ResetGui()
{
	OldPasswordEdit->clear();
	NewPasswordEdit->clear();
	ConfirmPasswordEdit->clear();
}


// protected slots

void CChangePasswordDialog::on_ButtonBox_accepted()
{
	//checking if the lineEdit boxes for the new password contain the same
	if (NewPasswordEdit->text() != ConfirmPasswordEdit->text()){
		QMessageBox::warning(this, tr("Error"), tr("The new password is not entered correctly the second time.\nPlease correct the new passwords."));

		return;
	}

	QString oldPassword = OldPasswordEdit->text();
	QString newPassword = NewPasswordEdit->text();

	//checking if the old password and the new password are not the same
	if (oldPassword == newPassword){
		QMessageBox::warning(this, tr("Error"), tr("The old password and the new password are similar!\nUse another password."));

		return;
	}

	if (!m_loggedUser.CheckPassword(oldPassword)){
		QMessageBox::warning(this, tr("Error"), tr("The old password is not correct.\nReenter the passwords."));

		ResetGui();
	}


	istd::CChangeNotifier notifier(&m_usersManager);

	if (m_loggedUser.SetPassword(newPassword)){
		accept();
	}
	else{
		QMessageBox::warning(this, tr("Error"), tr("The new password is not conform with guide lines.\nUse other password."));

		ResetGui();
	}
}


} // namespace iqtauth


