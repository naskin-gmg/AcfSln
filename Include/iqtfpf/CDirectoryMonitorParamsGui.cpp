#include "iqtfpf/CDirectoryMonitorParamsGui.h"


// ACF includes
#include "istd/TChangeNotifier.h"

#include "iqt/CSignalBlocker.h"


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
	}
}


void CDirectoryMonitorParamsGui::UpdateEditor(int /*updateFlags*/)
{
	ifpf::IDirectoryMonitorParams* objectPtr = GetObjectPtr();
	if (objectPtr != NULL){
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
	}
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


} // namespace iqtfpf


