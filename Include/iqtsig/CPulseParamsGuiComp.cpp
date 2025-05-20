#include <iqtsig/CPulseParamsGuiComp.h>


// ACF includes
#include <istd/CChangeNotifier.h>

// ACF-Solutions includes
#include <isig/IPulseConstraints.h>


namespace iqtsig
{


// protected methods

// reimplemented (iqtgui::TGuiObserverWrap)

void CPulseParamsGuiComp::OnGuiModelAttached()
{
	BaseClass::OnGuiModelAttached();

	Q_ASSERT(IsGuiCreated());
	const isig::IPulseParams* objectPtr = GetObservedObject();
	Q_ASSERT(objectPtr != NULL);

	istd::CRange delayTimeRange(0, 0.1);
	istd::CRange pulseTimeRange(0, 0.1);
	istd::CRange relaxingTimeRange(0, 0.1);

	int supportedFlags = isig::IPulseConstraints::PSF_PULSE_DURATION;

	const isig::IPulseConstraints* constraintsPtr = objectPtr->GetPulseConstraints();
	if (constraintsPtr != NULL){
		supportedFlags = constraintsPtr->GetPulseSupportedFlags();
		delayTimeRange = constraintsPtr->GetDelayTimeRange();
		pulseTimeRange = constraintsPtr->GetPulseDurationRange();
		relaxingTimeRange = constraintsPtr->GetRelaxingTimeRange();
	}

	bool hasDelayTime = (supportedFlags & isig::IPulseConstraints::PSF_DELAY_TIME);
	DelayTimeLabel->setVisible(hasDelayTime);
	DelayTimeSB->setVisible(hasDelayTime);
	DelayTimeUnitLabel->setVisible(hasDelayTime);
	if (hasDelayTime){
		DelayTimeSB->setMinimum(delayTimeRange.GetMinValue() * 1000);
		DelayTimeSB->setMaximum(delayTimeRange.GetMaxValue() * 1000);
	}

	bool hasPulseTime = (supportedFlags & isig::IPulseConstraints::PSF_PULSE_DURATION);
	PulseTimeLabel->setVisible(hasPulseTime);
	PulseTimeSB->setVisible(hasPulseTime);
	PulseTimeUnitLabel->setVisible(hasPulseTime);
	if (hasPulseTime){
		PulseTimeSB->setMinimum(pulseTimeRange.GetMinValue() * 1000);
		PulseTimeSB->setMaximum(pulseTimeRange.GetMaxValue() * 1000);
	}

	bool hasRelaxingTime = (supportedFlags & isig::IPulseConstraints::PSF_RELAXING_TIME);
	RelaxingTimeLabel->setVisible(hasRelaxingTime);
	RelaxingTimeSB->setVisible(hasRelaxingTime);
	RelaxingTimeUnitLabel->setVisible(hasRelaxingTime);
	if (hasRelaxingTime){
		RelaxingTimeSB->setMinimum(relaxingTimeRange.GetMinValue() * 1000);
		RelaxingTimeSB->setMaximum(relaxingTimeRange.GetMaxValue() * 1000);
	}
}


void CPulseParamsGuiComp::UpdateModel() const
{
	Q_ASSERT(IsGuiCreated());

	isig::IPulseParams* objectPtr = GetObservedObject();
	Q_ASSERT(objectPtr != NULL);

	if (DelayTimeSB->isVisible() && (DelayTimeSB->value() / 1000.0 != objectPtr->GetDelayTime())){
		objectPtr->SetDelayTime(DelayTimeSB->value() / 1000.0);
	}
	if (PulseTimeSB->isVisible() && (PulseTimeSB->value() / 1000.0 != objectPtr->GetPulseDuration())){
		objectPtr->SetPulseDuration(PulseTimeSB->value() / 1000.0);
	}
	if (RelaxingTimeSB->isVisible() && (RelaxingTimeSB->value() / 1000.0 != objectPtr->GetRelaxingTime())){
		objectPtr->SetRelaxingTime(RelaxingTimeSB->value() / 1000.0);
	}
}


void CPulseParamsGuiComp::UpdateGui(const istd::IChangeable::ChangeSet& /*changeSet*/)
{
	Q_ASSERT(IsGuiCreated());

	const isig::IPulseParams* objectPtr = GetObservedObject();
	if (objectPtr != NULL){
		DelayTimeSB->setValue(objectPtr->GetDelayTime() * 1000);
		PulseTimeSB->setValue(objectPtr->GetPulseDuration() * 1000);
		RelaxingTimeSB->setValue(objectPtr->GetRelaxingTime() * 1000);
	}
}


// protected slots

void CPulseParamsGuiComp::on_DelayTimeSB_valueChanged(double /*value*/)
{
	DoUpdateModel();
}


void CPulseParamsGuiComp::on_PulseTimeSB_valueChanged(double /*value*/)
{
	DoUpdateModel();
}


void CPulseParamsGuiComp::on_RelaxingTimeSB_valueChanged(double /*value*/)
{
	DoUpdateModel();
}


} // namespace iqtsig


