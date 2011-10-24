#include "iqtipr/CCircleFinderParamsGuiComp.h"


// ACF includes
#include "istd/TChangeNotifier.h"


namespace iqtipr
{


// reimplemented (imod::IModelEditor)

void CCircleFinderParamsGuiComp::UpdateModel() const
{
	I_ASSERT(IsGuiCreated());

	iipr::ICircleFinderParams* objectPtr = GetObjectPtr();
	I_ASSERT(objectPtr != NULL);

	objectPtr->SetOutlierEliminationEnabled(EnableOutliersElimination->isChecked());
	objectPtr->SetMinOutlierDistance(MinOutliersDistance->value());
	objectPtr->SetCaliperMode(CaliperMode->currentIndex());

	if (MaxRaysCount->text() == MaxRaysCount->specialValueText()){
		objectPtr->SetRaysCount(-1);
	}
	else{
		objectPtr->SetRaysCount(MaxRaysCount->value());
	}
}


// protected methods

// reimplemented (iqtgui::TGuiObserverWrap)

void CCircleFinderParamsGuiComp::UpdateGui(int /*updateFlags*/)
{
	I_ASSERT(IsGuiCreated());

	iipr::ICircleFinderParams* objectPtr = GetObjectPtr();
	if (objectPtr != NULL){
		EnableOutliersElimination->setChecked(objectPtr->IsOutlierEliminationEnabled());

		MinOutliersDistance->setVisible(objectPtr->IsOutlierEliminationEnabled());
		MinOutliersDistanceLabel->setVisible(objectPtr->IsOutlierEliminationEnabled());
		CaliperMode->setCurrentIndex(objectPtr->GetCaliperMode());

		MinOutliersDistance->setValue(objectPtr->GetMinOutlierDistance());

		if (objectPtr->GetRaysCount() < 0){
			MaxRaysCount->setValue(MaxRaysCount->minimum());
		}
		else{
			MaxRaysCount->setValue(objectPtr->GetRaysCount());
		}
	}
}


// protected slots

void CCircleFinderParamsGuiComp::on_EnableOutliersElimination_toggled(bool /*value*/)
{
	if (!IsUpdateBlocked()){
		UpdateBlocker updateBlocker(this);

		UpdateModel();
	}
}


void CCircleFinderParamsGuiComp::on_MinOutliersDistance_valueChanged(double /*value*/)
{
	if (!IsUpdateBlocked()){
		UpdateBlocker updateBlocker(this);

		UpdateModel();
	}
}


void CCircleFinderParamsGuiComp::on_MaxRaysCount_valueChanged(int /*value*/)
{
	if (!IsUpdateBlocked()){
		UpdateBlocker updateBlocker(this);

		UpdateModel();
	}
}


void CCircleFinderParamsGuiComp::on_CaliperMode_currentIndexChanged(int /*index*/)
{
	if (!IsUpdateBlocked()){
		UpdateBlocker updateBlocker(this);

		UpdateModel();
	}
}


} // namespace iqtipr


