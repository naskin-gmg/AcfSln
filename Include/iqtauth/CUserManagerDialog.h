#ifndef _qqtauth_CUserManagerDialog_included
#define _qqtauth_CUserManagerDialog_included


// Qt includes
#include <QItemDelegate>
#include <QDialog>


// ACF includes
#include "istd/CString.h"
#include "iser/CMemoryWriteArchive.h"

// ACF-Solutions includes
#include "iauth/IUserLogin.h"
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
	CUserManagerDialog(const iauth::IUserLogin& login, iauth::IUsersManager& manager);
	
	void SaveCurUsername(const istd::CString& Username);
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

	const iauth::IUserLogin& m_login;
	iauth::IUsersManager& m_manager;

	void ResetGui();
	void UpdateUserList();

	istd::CString m_curUserName;
	
	void* userManagerMemBufferPtr;
	int userManagerMemBufferSize;

	iser::CMemoryWriteArchive m_storedUsersManager;
};


} // namespace iqtauth


#endif	// !_qqtauth_CUserManagerDialog_included


