// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#include <iqtipr/CGeneralSearchParamsGuiComp.h>


// ACF includes
#include <istd/CChangeNotifier.h>
#include <iqt/CSignalBlocker.h>
#include <iipr/ISearchConstraints.h>


namespace iqtipr
{


// protected methods

// reimplemented (iqtgui::TGuiObserverWrap)

void CGeneralSearchParamsGuiComp::OnGuiModelAttached()
{
	BaseClass::OnGuiModelAttached();

	connect(MinScoreSB, qOverload<int>(&QSpinBox::valueChanged), this, &CGeneralSearchParamsGuiComp::OnParamsChanged);
	connect(OccurrenceSB, qOverload<int>(&QSpinBox::valueChanged), this, &CGeneralSearchParamsGuiComp::OnParamsChanged);
	connect(MinRotationSB, qOverload<double>(&QDoubleSpinBox::valueChanged), this, &CGeneralSearchParamsGuiComp::OnParamsChanged);
	connect(MaxRotationSB, qOverload<double>(&QDoubleSpinBox::valueChanged), this, &CGeneralSearchParamsGuiComp::OnParamsChanged);
	connect(MinScaleSB, qOverload<double>(&QDoubleSpinBox::valueChanged), this, &CGeneralSearchParamsGuiComp::OnParamsChanged);
	connect(MaxScaleSB, qOverload<double>(&QDoubleSpinBox::valueChanged), this, &CGeneralSearchParamsGuiComp::OnParamsChanged);
	connect(ScalingCB, &QCheckBox::toggled, this, &CGeneralSearchParamsGuiComp::OnParamsChanged);
	connect(RotationCB, &QCheckBox::toggled, this, &CGeneralSearchParamsGuiComp::OnParamsChanged);
}


void CGeneralSearchParamsGuiComp::UpdateModel() const
{
	Q_ASSERT(IsGuiCreated());

	iipr::ISearchParams* objectPtr = GetObservedObject();
	Q_ASSERT(objectPtr != NULL);

	istd::CChangeNotifier notifier(objectPtr);

	objectPtr->SetMinScore(MinScoreSB->value() / 100.0);

	objectPtr->SetNominalModelsCount(OccurrenceSB->value());

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
		objectPtr->SetScaleRange(istd::CRange(1,1));
	}
}


void CGeneralSearchParamsGuiComp::UpdateGui(const istd::IChangeable::ChangeSet& /*changeSet*/)
{
	Q_ASSERT(IsGuiCreated());

	iipr::ISearchParams* objectPtr = GetObservedObject();
	if (objectPtr != NULL){
		MinScoreSB->setValue(objectPtr->GetMinScore() * 100);
		OccurrenceSB->setValue(objectPtr->GetNominalModelsCount());
		MinRotationSB->setValue(objectPtr->GetRotationRange().GetMinValue());
		MaxRotationSB->setValue(objectPtr->GetRotationRange().GetMaxValue());
		MinScaleSB->setValue(objectPtr->GetScaleRange().GetMinValue());
		MaxScaleSB->setValue(objectPtr->GetScaleRange().GetMaxValue());

		const iipr::ISearchConstraints* constraintsPtr = objectPtr->GetSearchConstraints();
		if (constraintsPtr != NULL){
			const istd::CIntRange& modelCountRange = constraintsPtr->GetResultsCountConstraints();
			OccurrenceSB->setRange(modelCountRange.GetMinValue(), modelCountRange.GetMaxValue());
			OccurrenceSB->setSpecialValueText(modelCountRange.GetMinValue() < 0 ? tr("All") : "");

			int supportedFlags = constraintsPtr->GetSearchSupportedFlags();
			if ((supportedFlags & iipr::ISearchConstraints::SSF_ROTATION) != 0){
				const istd::CRange& range = constraintsPtr->GetRotationRangeConstraints();
				MinRotationSB->setRange(std::max(range.GetMinValue(), MinRotationSB->minimum()), std::min(MinRotationSB->maximum(), range.GetMaxValue()));
				MaxRotationSB->setRange(std::max(range.GetMinValue(), MaxRotationSB->minimum()), std::min(MaxRotationSB->maximum(), range.GetMaxValue()));

				RotationCB->setChecked(!objectPtr->GetRotationRange().IsEmpty());
			}
			else{
				RotationCB->setEnabled(false);
				MinRotationSB->setEnabled(false);
				MaxRotationSB->setEnabled(false);
			}

			if ((supportedFlags & iipr::ISearchConstraints::SSF_SCALING) != 0){
				const istd::CRange& range = constraintsPtr->GetScaleRangeConstraints();
				MinScaleSB->setRange(range.GetMinValue(), range.GetMaxValue());
				MaxScaleSB->setRange(range.GetMinValue(), range.GetMaxValue());

				ScalingCB->setChecked(!objectPtr->GetScaleRange().IsEmpty());
			}
			else{
				ScalingCB->setEnabled(false);
				MinScaleSB->setEnabled(false);
				MaxScaleSB->setEnabled(false);
			}
		}
		else {
			RotationCB->setChecked(!objectPtr->GetRotationRange().IsEmpty());
			ScalingCB->setChecked(!objectPtr->GetScaleRange().IsEmpty());
		}
	}
}


// reimplemented (iqtgui::CGuiComponentBase)

void CGeneralSearchParamsGuiComp::OnGuiCreated()
{
	BaseClass::OnGuiCreated();

	OccurrenceLabel->setVisible(!*m_hideOccurrenceAttrPtr);
	OccurrenceSB->setVisible(!*m_hideOccurrenceAttrPtr);

	ScalingCB->setVisible(!*m_hideScalingAttrPtr);
	ScalingLabelFrom->setVisible(!*m_hideScalingAttrPtr);
	ScalingLabelTo->setVisible(!*m_hideScalingAttrPtr);
	MinScaleSB->setVisible(!*m_hideScalingAttrPtr);
	MaxScaleSB->setVisible(!*m_hideScalingAttrPtr);

	RotationCB->setVisible(!*m_hideRotationAttrPtr);
	RotationLabelFrom->setVisible(!*m_hideRotationAttrPtr);
	RotationLabelTo->setVisible(!*m_hideRotationAttrPtr);
	MinRotationSB->setVisible(!*m_hideRotationAttrPtr);
	MaxRotationSB->setVisible(!*m_hideRotationAttrPtr);
}

void CGeneralSearchParamsGuiComp::OnGuiRetranslate()
{
	BaseClass::OnGuiRetranslate();

	if (!IsUpdateBlocked()){
		UpdateBlocker blockUpdate(this);

		UpdateGui(istd::IChangeable::GetAnyChange());
	}
}


// protected slots

void CGeneralSearchParamsGuiComp::OnParamsChanged()
{
	DoUpdateModel();
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
