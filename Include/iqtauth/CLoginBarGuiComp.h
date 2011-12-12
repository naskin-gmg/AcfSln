#ifndef _qqtauth_CLoginBarGuiComp_included
#define _qqtauth_CLoginBarGuiComp_included


// Qt includes
#include <QTimer>

// ACF includes
#include "iqtgui/TDesignerGuiCompBase.h"

// ACF-Solutions includes
#include "iauth/ILogin.h"

#include "generated/ui_CLoginBarGuiComp.h"


namespace iqtauth
{


class CLoginBarGuiComp: public iqtgui::TDesignerGuiCompBase<Ui::CLoginBarGuiComp>
{
	Q_OBJECT
public:
	typedef iqtgui::TDesignerGuiCompBase<Ui::CLoginBarGuiComp> BaseClass;

	I_BEGIN_COMPONENT(CLoginBarGuiComp);
		I_ASSIGN(m_loginIfPtr, "Login", "Login logic component", true, "Login");
		I_ASSIGN(m_autoLogoutMinutesAttrPtr, "AutoLogoutMinutes", "Time intervall for the automatic logout", false, 60);
	I_END_COMPONENT;

	CLoginBarGuiComp();

protected:
	void UpdateButtonsState();

	// reimplemented (QWidget)
	virtual bool eventFilter(QObject *obj, QEvent *event);

	// reimplemented (iqtgui::CGuiComponentBase)
	virtual void OnGuiCreated();

protected Q_SLOTS:
	virtual void on_LoginButton_clicked();
	virtual void on_LogoutButton_clicked();

private:
	QTimer m_autoLogoutTimer;
	int m_autoLogoutMilisec;

	I_REF(iauth::ILogin, m_loginIfPtr);
	I_ATTR(int, m_autoLogoutMinutesAttrPtr);
};


} // namespace iqtauth


#endif // !_qqtauth_CLoginBarGuiComp_included



