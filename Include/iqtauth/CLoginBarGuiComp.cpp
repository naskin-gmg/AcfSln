#include "iqtauth/CLoginBarGuiComp.h"


// Qt includes
#include <QtGui/QMessageBox>


namespace iqtauth
{


CLoginBarGuiComp::CLoginBarGuiComp()
:	m_autoLogoutMilisec(60*1000)
{
}


// protected methods

bool CLoginBarGuiComp::eventFilter(QObject *obj, QEvent *event)
{
	if (m_loginIfPtr.IsValid()){
		bool isLogged = (m_loginIfPtr->GetLoggedUser() != NULL);

		if (isLogged && event->type() == QEvent::KeyRelease || event->type() == QEvent::MouseButtonRelease){
			// Auto log off functionality is activated
			if (m_autoLogoutMilisec > 0){
				m_autoLogoutTimer.start(m_autoLogoutMilisec);
			}
		}
	}

	// standard event processing
	return BaseClass::eventFilter(obj, event);
}


void CLoginBarGuiComp::UpdateButtonsState()
{
	if (m_loginIfPtr.IsValid()){
		bool isLogged = (m_loginIfPtr->GetLoggedUser() != NULL);

		LoginButton->setVisible(!isLogged);
		LogoutButton->setVisible(isLogged);
		UserEdit->setEnabled(!isLogged);
		PasswordFrame->setVisible(!isLogged);
		if (!isLogged){
			m_autoLogoutTimer.stop();
			qApp->removeEventFilter(this);
			m_autoLogoutMilisec = 0;
		}
	}
}


// reimplemented (iqtgui::CGuiComponentBase)

void CLoginBarGuiComp::OnGuiCreated()
{
	connect(&m_autoLogoutTimer, SIGNAL(timeout()), this, SLOT(on_LogoutButton_clicked()));

	UpdateButtonsState();
}


// protected slots

void CLoginBarGuiComp::on_LoginButton_clicked()
{
	if (m_loginIfPtr.IsValid()){
		QString userName = UserEdit->text();
		QString password = PasswordEdit->text();
		if (m_loginIfPtr->Login(userName, password)){
			if (m_autoLogoutMinutesAttrPtr.IsValid()){
				Q_ASSERT(*m_autoLogoutMinutesAttrPtr > 0);

				m_autoLogoutMilisec = *m_autoLogoutMinutesAttrPtr * 60 * 1000;
				m_autoLogoutTimer.start(m_autoLogoutMilisec);
				
				qApp->installEventFilter(this);
			}

			UpdateButtonsState();
		}
		else{
			QMessageBox::information(NULL, tr("Error"), tr("Wrong password"));
		}
	}

	PasswordEdit->setText("");
}


void CLoginBarGuiComp::on_LogoutButton_clicked()
{
	if (m_loginIfPtr.IsValid() && m_loginIfPtr->Logout()){
		UpdateButtonsState();
	}
}


} // namespace iqtauth


