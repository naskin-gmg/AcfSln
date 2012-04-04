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

	istd::CChangeNotifier notifier(NULL);

	if ((objectPtr->GetMinScore() * 100) != MinScoreSB->value()){
		notifier.SetPtr(objectPtr);
		objectPtr->SetMinScore(MinScoreSB->value() / 100.0);
	}

	const istd::CRange& rotationRange = objectPtr->GetRotationRange();
	if (		rotationRange.GetMinValue() != MinRotationSB->value() || 
				rotationRange.GetMaxValue() != MaxRotationSB->value() ){
		notifier.SetPtr(objectPtr);
		objectPtr->SetRotationRange(istd::CRange(MinRotationSB->value(), MaxRotationSB->value()));
	}

	const istd::CRange& scaleRange = objectPtr->GetScaleRange();
	if (		scaleRange.GetMinValue() != MinScaleSB->value() || 
				scaleRange.GetMaxValue() != MaxScaleSB->value()	){
		notifier.SetPtr(objectPtr);
		objectPtr->SetScaleRange(istd::CRange(MinScaleSB->value(), MaxScaleSB->value()));
	}

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
				constraintsPtr->GetRotationRangeConstraints().GetMaxValue()
			);

			MaxRotationSB->setRange(
				constraintsPtr->GetRotationRangeConstraints().GetMinValue(),
				constraintsPtr->GetRotationRangeConstraints().GetMaxValue()
			);
		}

		if (!constraintsPtr->IsScaleRangeSupported()){
			ScalingCB->setEnabled(false);
			MinScaleSB->setEnabled(false);
			MaxScaleSB->setEnabled(false);
		}
		else{
			MinScaleSB->setRange(
				constraintsPtr->GetScaleRangeConstraints().GetMinValue(),
				constraintsPtr->GetScaleRangeConstraints().GetMaxValue()
			);

			MaxScaleSB->setRange(
				constraintsPtr->GetScaleRangeConstraints().GetMinValue(),
				constraintsPtr->GetScaleRangeConstraints().GetMaxValue()
			);
		}
	}
}


// protected methods

// reimplemented (iqtgui::TGuiObserverWrap)

void CGeneralSearchParamsGuiComp::OnGuiModelAttached()
{
	BaseClass::OnGuiModelAttached();

	QObject::connect(MinScoreSB, SIGNAL(valueChanged(double)), this, SLOT(OnParamsChanged(double)));
	QObject::connect(MinRotationSB, SIGNAL(valueChanged(double)), this, SLOT(OnParamsChanged(double)));
	QObject::connect(MaxRotationSB, SIGNAL(valueChanged(double)), this, SLOT(OnParamsChanged(double)));
	QObject::connect(MinScaleSB, SIGNAL(valueChanged(double)), this, SLOT(OnParamsChanged(double)));
	QObject::connect(MaxScaleSB, SIGNAL(valueChanged(double)), this, SLOT(OnParamsChanged(double)));
}


void CGeneralSearchParamsGuiComp::OnGuiModelDetached()
{
	QObject::disconnect(MinScoreSB, SIGNAL(valueChanged(double)), this, SLOT(OnParamsChanged(double)));
	QObject::disconnect(MinRotationSB, SIGNAL(valueChanged(double)), this, SLOT(OnParamsChanged(double)));
	QObject::disconnect(MaxRotationSB, SIGNAL(valueChanged(double)), this, SLOT(OnParamsChanged(double)));
	QObject::disconnect(MinScaleSB, SIGNAL(valueChanged(double)), this, SLOT(OnParamsChanged(double)));
	QObject::disconnect(MaxScaleSB, SIGNAL(valueChanged(double)), this, SLOT(OnParamsChanged(double)));

	BaseClass::OnGuiModelDetached();
}


void CGeneralSearchParamsGuiComp::UpdateGui(int /*updateFlags*/)
{
	I_ASSERT(IsGuiCreated());

	iipr::ISearchParams* objectPtr = GetObjectPtr();
	if (objectPtr != NULL){
		iqt::CSignalBlocker block(MinScoreSB);
		MinScoreSB->setValue(objectPtr->GetMinScore());

		iqt::CSignalBlocker block2(MinRotationSB);
		MinRotationSB->setValue(objectPtr->GetRotationRange().GetMinValue());

		iqt::CSignalBlocker block3(MaxRotationSB);
		MaxRotationSB->setValue(objectPtr->GetRotationRange().GetMaxValue());

		iqt::CSignalBlocker block4(MinScaleSB);
		MinScaleSB->setValue(objectPtr->GetScaleRange().GetMinValue());

		iqt::CSignalBlocker block5(MaxScaleSB);
		MaxScaleSB->setValue(objectPtr->GetScaleRange().GetMaxValue());
	}
}


// protected slots

void CGeneralSearchParamsGuiComp::OnParamsChanged(double /*value*/)
{
	if (!IsUpdateBlocked()){
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
