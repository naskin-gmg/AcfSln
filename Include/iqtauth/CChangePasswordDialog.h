#ifndef _qqtauth_CChangePasswordDialog_included
#define _qqtauth_CChangePasswordDialog_included


// Qt inclueds
#include<QtCore/QtGlobal>
#if QT_VERSION >= 0x050000
#include <QtWidgets/QDialog>
#else
#include <QtGui/QDialog>
#endif

// ACF-Solutions includes
#include "iauth/IUsersManager.h"
#include "iauth/CUser.h"
#include "Generated/ui_CChangePasswordDialog.h"


namespace iqtauth
{


class CChangePasswordDialog:
			public QDialog,
			public Ui::CChangePasswordDialog
{
	Q_OBJECT

public:
	typedef QDialog BaseClass;

	CChangePasswordDialog(iauth::IUsersManager& manager, iauth::CUser& loggedUser);

protected:
	void ResetGui();

protected Q_SLOTS:
	virtual void on_ButtonBox_accepted();

private:
	iauth::IUsersManager& m_usersManager;
	iauth::CUser& m_loggedUser;
};


} // namespace iqtauth


#endif	// !_qqtauth_CChangePasswordDialog_included
