#include "iqtfpf/CDirectoryMonitorParamsGui.h"


// Qt includes
#include <QHeaderView>


// ACF includes
#include "istd/TChangeNotifier.h"

#include "iqt/CSignalBlocker.h"

#include "iqtgui/CItemDelegate.h"


namespace iqtfpf
{


// reimplemented (imod::IModelEditor)

void CDirectoryMonitorParamsGui::UpdateModel() const
{
	I_ASSERT(IsGuiCreated());

	ifpf::IDirectoryMonitorParams* objectPtr = GetObjectPtr();
	I_ASSERT(objectPtr != NULL);

	objectPtr->SetPoolingIntervall(PoolingIntervallSpin->value());

	int observedItems = 0;
	if (FilesCheck->isChecked()){
		observedItems = observedItems | ifpf::IDirectoryMonitorParams::OI_FILES;
	}

	if (DirsCheck->isChecked()){
		observedItems = observedItems | ifpf::IDirectoryMonitorParams::OI_DIR;
	}

	if (DrivesCheck->isChecked()){
		observedItems = observedItems | ifpf::IDirectoryMonitorParams::OI_DRIVES;
	}

	objectPtr->SetObservedItemTypes(observedItems);

	int observedChanges = 0;
	if (ModifiedCheck->isChecked()){
		observedChanges = observedChanges | ifpf::IDirectoryMonitorParams::OC_MODIFIED;
	}

	if (AddedCheck->isChecked()){
		observedChanges = observedChanges | ifpf::IDirectoryMonitorParams::OC_ADD;
	}

	if (RemovedCheck->isChecked()){
		observedChanges = observedChanges | ifpf::IDirectoryMonitorParams::OC_REMOVE;
	}

	if (AttributesCheck->isChecked()){
		observedChanges = observedChanges | ifpf::IDirectoryMonitorParams::OC_ATTR_CHANGED;
	}

	objectPtr->SetObservedChanges(observedChanges);

	QTreeWidgetItemIterator acceptIter(AcceptPatternsList);
	istd::CStringList acceptPatterns;
	while (*acceptIter){
		acceptPatterns.push_back(iqt::GetCString((*acceptIter)->text(0)));

		++acceptIter;
	}

	objectPtr->SetAcceptPatterns(acceptPatterns);

	QTreeWidgetItemIterator ignoreIter(IgnorePatternsList);
	istd::CStringList ignorePatterns;
	while (*ignoreIter){
		ignorePatterns.push_back(iqt::GetCString((*ignoreIter)->text(0)));

		++ignoreIter;
	}

	objectPtr->SetIgnorePatterns(ignorePatterns);
}

	
// reimplemenented (iqtgui::TGuiObserverWrap)

void CDirectoryMonitorParamsGui::UpdateGui(int /*updateFlags*/)
{
	I_ASSERT(IsGuiCreated());

	ifpf::IDirectoryMonitorParams* objectPtr = GetObjectPtr();
	if (objectPtr != NULL){
		ResetEditor();

		PoolingIntervallSpin->setValue(objectPtr->GetPoolingIntervall());

		int observedItems = objectPtr->GetObservedItemTypes();
		FilesCheck->setChecked((observedItems & ifpf::IDirectoryMonitorParams::OI_FILES) != 0);
		DirsCheck->setChecked((observedItems & ifpf::IDirectoryMonitorParams::OI_DIR) != 0);
		DrivesCheck->setChecked((observedItems & ifpf::IDirectoryMonitorParams::OI_DRIVES) != 0);

		int observedChanges = objectPtr->GetObservedChanges();
		ModifiedCheck->setChecked((observedChanges & ifpf::IDirectoryMonitorParams::OC_MODIFIED) != 0);
		AddedCheck->setChecked((observedChanges & ifpf::IDirectoryMonitorParams::OC_ADD) != 0);
		RemovedCheck->setChecked((observedChanges & ifpf::IDirectoryMonitorParams::OC_REMOVE) != 0);
		AttributesCheck->setChecked((observedChanges & ifpf::IDirectoryMonitorParams::OC_ATTR_CHANGED) != 0);

		istd::CStringList acceptPatterns = objectPtr->GetAcceptPatterns();
		for (int patternIndex = 0; patternIndex < int(acceptPatterns.size()); patternIndex++){
			AddPattern(iqt::GetQString(acceptPatterns[patternIndex]), AcceptPatternsList);
		}

		istd::CStringList ignorePatterns = objectPtr->GetIgnorePatterns();
		for (int patternIndex = 0; patternIndex < int(ignorePatterns.size()); patternIndex++){
			AddPattern(iqt::GetQString(ignorePatterns[patternIndex]), IgnorePatternsList);
		}
	}
}


// reimplemented (iqtgui::CGuiComponentBase)

void CDirectoryMonitorParamsGui::OnGuiCreated()
{
	BaseClass::OnGuiCreated();

	AcceptPatternsList->setEditTriggers(QAbstractItemView::DoubleClicked |  QAbstractItemView::EditKeyPressed);

	AcceptPatternsList->setItemDelegate(new iqtgui::CItemDelegate(20, AcceptPatternsList));

	IgnorePatternsList->setEditTriggers(QAbstractItemView::DoubleClicked |  QAbstractItemView::EditKeyPressed);

	IgnorePatternsList->setItemDelegate(new iqtgui::CItemDelegate(20, AcceptPatternsList));
}


// protected slots

void CDirectoryMonitorParamsGui::on_FilesCheck_toggled(bool/* isChecked*/)
{
	OnModelUpdate();
}


void CDirectoryMonitorParamsGui::on_DirsCheck_toggled(bool/* isChecked*/)
{
	OnModelUpdate();
}


void CDirectoryMonitorParamsGui::on_DrivesCheck_toggled(bool/* isChecked*/)
{
	OnModelUpdate();
}


void CDirectoryMonitorParamsGui::on_ModifiedCheck_toggled(bool/* isChecked*/)
{
	OnModelUpdate();
}


void CDirectoryMonitorParamsGui::on_AddedCheck_toggled(bool/* isChecked*/)
{
	OnModelUpdate();
}


void CDirectoryMonitorParamsGui::on_RemovedCheck_toggled(bool/* isChecked*/)
{
	OnModelUpdate();
}


void CDirectoryMonitorParamsGui::on_AttributesCheck_toggled(bool/* isChecked*/)
{
	OnModelUpdate();
}


void CDirectoryMonitorParamsGui::on_PoolingIntervallSpin_valueChanged(double/* isChecked*/)
{
	OnModelUpdate();
}


void CDirectoryMonitorParamsGui::on_AddAcceptPatternButton_clicked()
{
	AddPattern("<Filter>", AcceptPatternsList);

	OnModelUpdate();
}


void CDirectoryMonitorParamsGui::on_RemoveAcceptPatternButton_clicked()
{
	RemoveSelectedPatterns(AcceptPatternsList);
}


void CDirectoryMonitorParamsGui::on_AcceptPatternsList_itemSelectionChanged()
{
	QList<QTreeWidgetItem*> selectedItems = AcceptPatternsList->selectedItems();

	RemoveAcceptPatternButton->setEnabled(!selectedItems.empty());
}


void CDirectoryMonitorParamsGui::on_AcceptPatternsList_itemChanged(QTreeWidgetItem* /*item*/, int /*column*/)
{
	OnModelUpdate();
}


void CDirectoryMonitorParamsGui::on_AddIgnorePatternButton_clicked()
{
	AddPattern("<Filter>", IgnorePatternsList);

	OnModelUpdate();
}


void CDirectoryMonitorParamsGui::on_RemoveIgnorePatternButton_clicked()
{
	RemoveSelectedPatterns(IgnorePatternsList);
}


void CDirectoryMonitorParamsGui::on_IgnorePatternsList_itemSelectionChanged()
{
	QList<QTreeWidgetItem*> selectedItems = IgnorePatternsList->selectedItems();

	RemoveIgnorePatternButton->setEnabled(!selectedItems.empty());
}


void CDirectoryMonitorParamsGui::on_IgnorePatternsList_itemChanged(QTreeWidgetItem* /*item*/, int /*column*/)
{
	OnModelUpdate();
}


// private methods

void CDirectoryMonitorParamsGui::AddPattern(const QString& filter, QTreeWidget* treeView)
{
	iqt::CSignalBlocker block(treeView);

	QTreeWidgetItem* newFilterItem = new QTreeWidgetItem(treeView);
	newFilterItem->setText(0, filter);
	newFilterItem->setFlags(Qt::ItemIsEditable | Qt::ItemIsEnabled | Qt::ItemIsSelectable);

	treeView->addTopLevelItem(newFilterItem);
}


void CDirectoryMonitorParamsGui::RemoveSelectedPatterns(QTreeWidget* treeView)
{
	QList<QTreeWidgetItem*> selectedItems = treeView->selectedItems();

	if (!selectedItems.empty()){
		for (QList<QTreeWidgetItem*>::iterator index = selectedItems.begin(); index != selectedItems.end(); index++){
			QTreeWidgetItem* itemPtr = treeView->takeTopLevelItem(AcceptPatternsList->indexOfTopLevelItem(*index));

			if (itemPtr != NULL){
				delete itemPtr;
			}
		}

		OnModelUpdate();
	}
}


void CDirectoryMonitorParamsGui::ResetEditor()
{
	while (AcceptPatternsList->topLevelItemCount() > 0){
		delete AcceptPatternsList->takeTopLevelItem(0);
	}	

	while (IgnorePatternsList->topLevelItemCount() > 0){
		delete IgnorePatternsList->takeTopLevelItem(0);
	}	
}


void CDirectoryMonitorParamsGui::OnModelUpdate()
{
	if (!IsUpdateBlocked()){
		UpdateBlocker updateBlocker(this);

		UpdateModel();
	}
}


} // namespace iqtfpf


