#ifndef _qqtauth_CUserManagerDialog_included
#define _qqtauth_CUserManagerDialog_included


// Qt includes
#include <QtCore/QString>
#include<QtCore/QtGlobal>
#if QT_VERSION >= 0x050000
#include <QtWidgets/QItemDelegate>
#include <QtWidgets/QDialog>
#else
#include <QtGui/QItemDelegate>
#include <QtGui/QDialog>
#endif

// ACF includes
#include "iser/CMemoryWriteArchive.h"

// ACF-Solutions includes
#include "iauth/ILogin.h"
#include "iauth/IUsersManager.h"
#include "iqtauth/Generated/ui_CUserManagerDialog.h"


namespace iqtauth
{


class CUserManagerDialog:
			public QDialog, 
			public Ui::CUserManagerDialog
{
	Q_OBJECT
public:
	CUserManagerDialog(const iauth::ILogin& login, iauth::IUsersManager& manager);
	
	void SaveCurUsername(const QString& Username);
	int GetUserGroupSize();

protected Q_SLOTS:
	void on_AddUserButton_clicked();
	void on_ResetPasswordButton_clicked();
	void on_RemoveUserButton_clicked();
	void OnCancel();

private:
	enum ListColumn
	{
		LC_NAME,
		LC_GROUP
	};

	class CUserManagerItemDelegate: public QItemDelegate
	{
	public:
		typedef QItemDelegate BaseClass;

		CUserManagerItemDelegate(CUserManagerDialog& parent);

		// reimplemented (QItemDelegate)
		virtual QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const;

		// reimplemented (QAbstractItemDelegate)
		virtual void setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const;

		// reimplemented (QWidget)
		virtual QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const;

	private:
		CUserManagerDialog& m_parent;
		int m_loggedUserLevel;
	};

	const iauth::ILogin& m_login;
	iauth::IUsersManager& m_manager;

	void ResetGui();
	void UpdateUserList();

	QString m_curUserName;
	
	void* userManagerMemBufferPtr;
	int userManagerMemBufferSize;

	iser::CMemoryWriteArchive m_storedUsersManager;
};


} // namespace iqtauth


#endif	// !_qqtauth_CUserManagerDialog_included


