#include "iqtipr/CGeneralSearchParamsGuiComp.h"


// ACF includes
#include "istd/TChangeNotifier.h"
#include "iqt/CSignalBlocker.h"
#include "iipr/ISearchConstraints.h"


namespace iqtipr
{


// reimplemented (imod::IModelEditor)

void CGeneralSearchParamsGuiComp::UpdateModel() const
{
	I_ASSERT(IsGuiCreated());

	iipr::ISearchParams* objectPtr = GetObjectPtr();
	I_ASSERT(objectPtr != NULL);

	istd::CChangeNotifier notifier(objectPtr);

	objectPtr->SetMinScore(MinScoreSB->value() / 100.0);

	objectPtr->SetNominalModelsCount(ModelOccurenceSB->value());

	if (RotationCB->isChecked()){
		objectPtr->SetRotationRange(istd::CRange(MinRotationSB->value(), MaxRotationSB->value()));
	}
	else{
		objectPtr->SetRotationRange(istd::CRange(0,0));
	}

	if (ScalingCB->isChecked()){
		objectPtr->SetScaleRange(istd::CRange(MinScaleSB->value(), MaxScaleSB->value()));
	}
	else{
		objectPtr->SetScaleRange(istd::CRange(0,0));
	}
}


// protected methods

// reimplemented (iqtgui::TGuiObserverWrap)

void CGeneralSearchParamsGuiComp::OnGuiModelAttached()
{
	BaseClass::OnGuiModelAttached();

	connect(MinScoreSB, SIGNAL(valueChanged(int)), this, SLOT(OnParamsChanged()));
	connect(ModelOccurenceSB, SIGNAL(valueChanged(int)), this, SLOT(OnParamsChanged()));
	connect(MinRotationSB, SIGNAL(valueChanged(double)), this, SLOT(OnParamsChanged()));
	connect(MaxRotationSB, SIGNAL(valueChanged(double)), this, SLOT(OnParamsChanged()));
	connect(MinScaleSB, SIGNAL(valueChanged(double)), this, SLOT(OnParamsChanged()));
	connect(MaxScaleSB, SIGNAL(valueChanged(double)), this, SLOT(OnParamsChanged()));
	connect(ScalingCB, SIGNAL(toggled(bool)), this, SLOT(OnParamsChanged()));
	connect(RotationCB, SIGNAL(toggled(bool)), this, SLOT(OnParamsChanged()));
}


void CGeneralSearchParamsGuiComp::UpdateGui(int /*updateFlags*/)
{
	I_ASSERT(IsGuiCreated());

	iipr::ISearchParams* objectPtr = GetObjectPtr();
	if (objectPtr != NULL){
		MinScoreSB->setValue(objectPtr->GetMinScore() * 100);
		ModelOccurenceSB->setValue(objectPtr->GetNominalModelsCount());
		MinRotationSB->setValue(objectPtr->GetRotationRange().GetMinValue());
		MaxRotationSB->setValue(objectPtr->GetRotationRange().GetMaxValue());
		MinScaleSB->setValue(objectPtr->GetScaleRange().GetMinValue());
		MaxScaleSB->setValue(objectPtr->GetScaleRange().GetMaxValue());

		const iipr::ISearchConstraints* constraintsPtr = objectPtr->GetSearchConstraints();
		if (constraintsPtr != NULL){
			if (!constraintsPtr->IsRotationRangeSupported()){
				RotationCB->setEnabled(false);
				MinRotationSB->setEnabled(false);
				MaxRotationSB->setEnabled(false);
			}
			else{
				MinRotationSB->setRange(
							constraintsPtr->GetRotationRangeConstraints().GetMinValue(),
							constraintsPtr->GetRotationRangeConstraints().GetMaxValue());

				MaxRotationSB->setRange(
							constraintsPtr->GetRotationRangeConstraints().GetMinValue(),
							constraintsPtr->GetRotationRangeConstraints().GetMaxValue());

				RotationCB->setChecked(!constraintsPtr->GetRotationRangeConstraints().IsEmpty());
			}

			if (!constraintsPtr->IsScaleRangeSupported()){
				ScalingCB->setEnabled(false);
				MinScaleSB->setEnabled(false);
				MaxScaleSB->setEnabled(false);
			}
			else{
				MinScaleSB->setRange(
							constraintsPtr->GetScaleRangeConstraints().GetMinValue(),
							constraintsPtr->GetScaleRangeConstraints().GetMaxValue());

				MaxScaleSB->setRange(
							constraintsPtr->GetScaleRangeConstraints().GetMinValue(),
							constraintsPtr->GetScaleRangeConstraints().GetMaxValue());

				ScalingCB->setChecked(!constraintsPtr->GetScaleRangeConstraints().IsEmpty());
			}
		}
	}
}


// protected slots

void CGeneralSearchParamsGuiComp::OnParamsChanged()
{
	if (!IsUpdateBlocked() && GetModelPtr() != NULL){
		UpdateBlocker updateBlocker(this);

		UpdateModel();
	}
}


void CGeneralSearchParamsGuiComp::on_RotationCB_toggled(bool state)
{
	MinRotationSB->setEnabled(state);
	MaxRotationSB->setEnabled(state);
}


void CGeneralSearchParamsGuiComp::on_ScalingCB_toggled(bool state)
{
	MinScaleSB->setEnabled(state);
	MaxScaleSB->setEnabled(state);
}


} // namespace iqtipr
