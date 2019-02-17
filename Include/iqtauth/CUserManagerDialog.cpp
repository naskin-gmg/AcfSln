#include <iqtauth/CUserManagerDialog.h>


// Qt includes
#include<QtCore/QtGlobal>
#if QT_VERSION >= 0x050000
#include <QtWidgets/QListView>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QComboBox>
#else
#include <QtGui/QMessageBox>
#include <QtGui/QLineEdit>
#include <QtGui/QComboBox>
#endif

// ACF includes
#include <istd/CChangeNotifier.h>
#include <iser/CMemoryReadArchive.h>
#include <iqt/iqt.h>


namespace iqtauth
{


CUserManagerDialog::CUserManagerDialog(const iauth::ILogin& login, iauth::IUsersManager& manager)
	:m_login(login),
	m_manager(manager)
{
	const iauth::CUser* loggedUserPtr = m_login.GetLoggedUser();
	if (!m_manager.Serialize(m_storedUsersManager) || (loggedUserPtr == NULL)){
		reject();

		return;
	}

	setupUi(this);

	connect(this, SIGNAL(rejected()), SLOT(OnCancel()));

	TreeUserList->setItemDelegate(new CUserManagerItemDelegate(*this));

	//setting up the window title for the CUserManagerDialog
	QString userName = loggedUserPtr->GetUserName();
	int groupIndex = loggedUserPtr->GetUserGroup();

	setWindowTitle(
				windowTitle().
				arg(userName).
				arg(m_manager.GetUserGroupName(groupIndex)));

	//Setting up the GUI elements and filling the user list
	UpdateUserList();
}


void CUserManagerDialog::SaveCurUsername(const QString& userName)
{
	m_curUserName = userName;
}


// protected slots

void CUserManagerDialog::on_AddUserButton_clicked()
{
	int newUserSuffix = 0;
	QString userName;
	do{
		userName = tr("User_%1").arg(++newUserSuffix);
	} while (m_manager.FindUserIndex(userName) >= 0);

	m_manager.AddUser(userName);

	//updating screen
	UpdateUserList();

	//finding the just added user on the screen
	QList<QTreeWidgetItem *> itemList = TreeUserList->findItems(userName, Qt::MatchExactly, 0);
	if (itemList.size() > 0){
		itemList.at(0)->setSelected(true);
	}
}


void CUserManagerDialog::on_ResetPasswordButton_clicked()
{
	QTreeWidgetItem* curItemPtr = TreeUserList->currentItem();
	if (curItemPtr != NULL){
		QString userName = curItemPtr->data(0, Qt::DisplayRole).toString();

		int userIndex = m_manager.FindUserIndex(userName);

		iauth::CUser& user = m_manager.GetUserRef(userIndex);

		user.ResetPassword();
	}
}


void CUserManagerDialog::on_RemoveUserButton_clicked()
{
	QTreeWidgetItem* curItemPtr = TreeUserList->currentItem();
	if (curItemPtr != NULL){
		QString userName = curItemPtr->data(0, Qt::DisplayRole).toString();
		QString currentUserName = m_login.GetLoggedUser()->GetUserName();

		if (currentUserName == userName){
			QMessageBox::warning(this, tr("Error"), tr("You cannot remove yourself"));
			return;
		}

		if (m_manager.DeleteUser(userName)){
			//update user list
			UpdateUserList();
		}
		else{
			QMessageBox::warning(this, tr("Error"), tr("The user could not be removed"));
		}
	}
}


void CUserManagerDialog::OnCancel()
{
	iser::CMemoryReadArchive restoreArchive(m_storedUsersManager);
	if (!m_manager.Serialize(restoreArchive)){
		QMessageBox::warning(this, tr("Error"), tr("Cannot restore user list."));
	}
}


void CUserManagerDialog::UpdateUserList()
{
	TreeUserList->clear();

	const iauth::CUser* loggedUserPtr = m_login.GetLoggedUser();

	if (loggedUserPtr != NULL){
		int loggedUserLevel = loggedUserPtr->GetUserGroup();

		int usersCount = m_manager.GetUsersCount();
		for (int i = 0; i < usersCount; ++i){
			const iauth::CUser& user = m_manager.GetUser(i);
			if (user.GetUserGroup() < loggedUserLevel){
				QTreeWidgetItem* itemPtr;
				itemPtr = new QTreeWidgetItem(QTreeWidgetItem::Type);
                itemPtr->setText(LC_NAME_COLUMN, user.GetUserName());
				itemPtr->setText(LC_GROUP, m_manager.GetUserGroupName(user.GetUserGroup()));
				itemPtr->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsEditable);
                itemPtr->setData(LC_NAME_COLUMN, Qt::UserRole, i);
				itemPtr->setData(LC_GROUP, Qt::UserRole, i);

				TreeUserList->addTopLevelItem(itemPtr);
			}
		}
	}
}


// public methods of embedded class CUserManagerItemDelegate

CUserManagerDialog::CUserManagerItemDelegate::CUserManagerItemDelegate(CUserManagerDialog& parent)
	:	m_parent(parent)
{
	m_loggedUserLevel = -1;

	const iauth::CUser* userPtr = m_parent.m_login.GetLoggedUser();
	if (userPtr != NULL){
		m_loggedUserLevel = userPtr->GetUserGroup();
	}
}


// reimplemented (QItemDelegate)

QWidget* CUserManagerDialog::CUserManagerItemDelegate::createEditor(
				QWidget* parent, 
				const QStyleOptionViewItem& option, 
				const QModelIndex& index) const
{
	switch (index.column()){
    case LC_NAME_COLUMN:
		{
			QString text = index.data().toString();
			QLineEdit* lineEditPtr = new QLineEdit(parent);
			lineEditPtr->setText(text);
			m_parent.SaveCurUsername(text);
			return lineEditPtr;
		}

	case LC_GROUP:
		{
			QComboBox* comboBoxPtr = new QComboBox(parent);
			comboBoxPtr->setView(new QListView()); // Qt bug workaround to set up the style from css

			for (int i = 0; i < m_loggedUserLevel; ++i){
				comboBoxPtr->addItem(m_parent.m_manager.GetUserGroupName(i));
			}
			comboBoxPtr->setCurrentIndex(comboBoxPtr->findText(index.data().toString()));

			return comboBoxPtr;
		}

	default:
		return BaseClass::createEditor(parent, option, index);
	}
}


// reimplemented (QAbstractItemDelegate)

void CUserManagerDialog::CUserManagerItemDelegate::setModelData(
				QWidget* editor, 
				QAbstractItemModel* model, 
				const QModelIndex& index) const
{
	switch (index.column()){
    case LC_NAME_COLUMN:
		{
			QLineEdit* lineEditPtr = dynamic_cast<QLineEdit*>(editor);
			Q_ASSERT(lineEditPtr != NULL);	// was created by createEditor(...)

			//editor is a QLineEdit
			QString userName = lineEditPtr->text();
			model->setData(index, userName);

			istd::CChangeNotifier notifier(&m_parent.m_manager);

			int userIndex = model->data(index, Qt::UserRole).toInt();
			m_parent.m_manager.RenameUser(userIndex, userName);

			return;
		}

	case LC_GROUP:
		{
			QComboBox* comboBoxPtr = dynamic_cast<QComboBox*>(editor);
			Q_ASSERT(comboBoxPtr != NULL);	// was created by createEditor(...)
			comboBoxPtr->setView(new QListView()); // Qt bug workaround to set up the style from css

			int groupIndex = comboBoxPtr->currentIndex();
			QString groupName = m_parent.m_manager.GetUserGroupName(groupIndex);
			model->setData(index, groupName);

			istd::CChangeNotifier notifier(&m_parent.m_manager);

			int userIndex = model->data(index, Qt::UserRole).toInt();
			iauth::CUser& user = m_parent.m_manager.GetUserRef(userIndex);

			user.SetUserGroup(groupIndex);

			return;
		}
	}
}


// reimplemented (QWidget)

QSize CUserManagerDialog::CUserManagerItemDelegate::sizeHint( const QStyleOptionViewItem& option, const QModelIndex& index) const
{
	QFontMetrics fontMetrics(option.font);

	QSize size = QItemDelegate::sizeHint(option, index);
	size.setHeight(qMax(fontMetrics.height(), 25));

	return size;
}


} // namespace iqtauth


