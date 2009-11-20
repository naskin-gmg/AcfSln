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
	ifpf::IDirectoryMonitorParams* objectPtr = GetObjectPtr();
	if (objectPtr != NULL && !IsUpdateBlocked()){
		UpdateBlocker blocker(const_cast<CDirectoryMonitorParamsGui*>(this));

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
	
		QTreeWidgetItemIterator iter(FileFiltersList);
		istd::CStringList fileFilters;
		while (*iter){
			fileFilters.push_back(iqt::GetCString((*iter)->text(0)));

			++iter;
		}

		objectPtr->SetFileFilters(fileFilters);
	}
}


void CDirectoryMonitorParamsGui::UpdateEditor(int /*updateFlags*/)
{
	ifpf::IDirectoryMonitorParams* objectPtr = GetObjectPtr();
	if (objectPtr != NULL){
		iqt::CSignalBlocker block(this, true);

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

		istd::CStringList fileFilters = objectPtr->GetFileFilters();

		for (int fileFilterIndex = 0; fileFilterIndex < int(fileFilters.size()); fileFilterIndex++){
			AddFilter(iqt::GetQString(fileFilters[fileFilterIndex]));
		}
	}
}


// reimplemented (iqtgui::CGuiComponentBase)

void CDirectoryMonitorParamsGui::OnGuiCreated()
{
	BaseClass::OnGuiCreated();

	FileFiltersList->setEditTriggers(QAbstractItemView::DoubleClicked |  QAbstractItemView::EditKeyPressed);

	FileFiltersList->setItemDelegate(new iqtgui::CItemDelegate(20, FileFiltersList));
}


// protected slots

void CDirectoryMonitorParamsGui::on_FilesCheck_toggled(bool/* isChecked*/)
{
	UpdateModel();
}


void CDirectoryMonitorParamsGui::on_DirsCheck_toggled(bool/* isChecked*/)
{
	UpdateModel();
}


void CDirectoryMonitorParamsGui::on_DrivesCheck_toggled(bool/* isChecked*/)
{
	UpdateModel();
}


void CDirectoryMonitorParamsGui::on_ModifiedCheck_toggled(bool/* isChecked*/)
{
	UpdateModel();
}


void CDirectoryMonitorParamsGui::on_AddedCheck_toggled(bool/* isChecked*/)
{
	UpdateModel();
}


void CDirectoryMonitorParamsGui::on_RemovedCheck_toggled(bool/* isChecked*/)
{
	UpdateModel();
}


void CDirectoryMonitorParamsGui::on_AttributesCheck_toggled(bool/* isChecked*/)
{
	UpdateModel();
}


void CDirectoryMonitorParamsGui::on_PoolingIntervallSpin_valueChanged(double/* isChecked*/)
{
	UpdateModel();
}


void CDirectoryMonitorParamsGui::on_AddFilterButton_clicked()
{
	AddFilter("<Filter>");

	UpdateModel();
}


void CDirectoryMonitorParamsGui::on_RemoveFilterButton_clicked()
{
	QList<QTreeWidgetItem*> selectedItems = FileFiltersList->selectedItems();

	if (!selectedItems.empty()){
		for (QList<QTreeWidgetItem*>::iterator index = selectedItems.begin(); index != selectedItems.end(); index++){
			QTreeWidgetItem* itemPtr = FileFiltersList->takeTopLevelItem(FileFiltersList->indexOfTopLevelItem(*index));

			if (itemPtr != NULL){
				delete itemPtr;
			}
		}

		UpdateModel();
	}
}


void CDirectoryMonitorParamsGui::on_FileFiltersList_itemSelectionChanged()
{
	QList<QTreeWidgetItem*> selectedItems = FileFiltersList->selectedItems();

	RemoveFilterButton->setEnabled(!selectedItems.empty());
}


void CDirectoryMonitorParamsGui::on_FileFiltersList_itemChanged(QTreeWidgetItem* /*item*/, int /*column*/)
{
	UpdateModel();
}


// private methods

void CDirectoryMonitorParamsGui::AddFilter(const QString& filter)
{
	iqt::CSignalBlocker block(FileFiltersList);

	QTreeWidgetItem* newFilterItem = new QTreeWidgetItem(FileFiltersList);
	newFilterItem->setText(0, filter);
	newFilterItem->setFlags(Qt::ItemIsEditable | Qt::ItemIsEnabled | Qt::ItemIsSelectable);

	FileFiltersList->addTopLevelItem(newFilterItem);
}


void CDirectoryMonitorParamsGui::ResetEditor()
{
	while (FileFiltersList->topLevelItemCount() > 0){
		delete FileFiltersList->takeTopLevelItem(0);
	}	
}


} // namespace iqtfpf


