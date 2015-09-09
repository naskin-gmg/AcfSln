#include "iqtipr/CCircleFinderParamsGuiComp.h"


// ACF includes
#include "istd/CChangeNotifier.h"


namespace iqtipr
{


// reimplemented (imod::IModelEditor)

void CCircleFinderParamsGuiComp::UpdateModel() const
{
	Q_ASSERT(IsGuiCreated());

	iipr::ICircleFinderParams* objectPtr = GetObservedObject();
	Q_ASSERT(objectPtr != NULL);

	objectPtr->SetOutlierEliminationEnabled(EnableOutliersElimination->isChecked());

	double distanceDisplayFactor = 1;

	const imath::IUnitInfo* unitInfoPtr = objectPtr->GetDistanceUnitInfo();
	if (unitInfoPtr != NULL){
		distanceDisplayFactor = unitInfoPtr->GetDisplayMultiplicationFactor();
	}

	objectPtr->SetMinOutlierDistance(MinOutliersDistance->value() / distanceDisplayFactor);
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

void CCircleFinderParamsGuiComp::UpdateGui(const istd::IChangeable::ChangeSet& /*changeSet*/)
{
	Q_ASSERT(IsGuiCreated());

	bool isUnitKnown = false;

	iipr::ICircleFinderParams* objectPtr = GetObservedObject();
	if (objectPtr != NULL){
		EnableOutliersElimination->setChecked(objectPtr->IsOutlierEliminationEnabled());

		MinOutliersDistance->setVisible(objectPtr->IsOutlierEliminationEnabled());
		MinOutliersDistanceLabel->setVisible(objectPtr->IsOutlierEliminationEnabled());
		CaliperMode->setCurrentIndex(objectPtr->GetCaliperMode());

		double distanceDisplayFactor = 1;
		QString unitName;

		const imath::IUnitInfo* unitInfoPtr = objectPtr->GetDistanceUnitInfo();
		if (unitInfoPtr != NULL){
			unitName = unitInfoPtr->GetUnitName();
			distanceDisplayFactor = unitInfoPtr->GetDisplayMultiplicationFactor();
		}

		MinOutliersDistance->setValue(objectPtr->GetMinOutlierDistance() * distanceDisplayFactor);

		UnitLabel->setText(unitName);
		isUnitKnown = !unitName.isEmpty();

		if (objectPtr->GetRaysCount() < 0){
			MaxRaysCount->setValue(MaxRaysCount->minimum());
		}
		else{
			MaxRaysCount->setValue(objectPtr->GetRaysCount());
		}
	}

	UnitLabel->setVisible(isUnitKnown);
}


// protected slots

void CCircleFinderParamsGuiComp::on_EnableOutliersElimination_toggled(bool /*value*/)
{
	DoUpdateModel();
}


void CCircleFinderParamsGuiComp::on_MinOutliersDistance_valueChanged(double /*value*/)
{
	DoUpdateModel();
}


void CCircleFinderParamsGuiComp::on_MaxRaysCount_valueChanged(int /*value*/)
{
	DoUpdateModel();
}


void CCircleFinderParamsGuiComp::on_CaliperMode_currentIndexChanged(int /*index*/)
{
	DoUpdateModel();
}


} // namespace iqtipr


