#include <iqtsig/CTriggerParamsGuiComp.h>


#include <istd/CChangeNotifier.h>

#include <isig/ITriggerConstraints.h>


namespace iqtsig
{


// protected methods

// reimplemented (iqtgui::TGuiObserverWrap)

void CTriggerParamsGuiComp::OnGuiModelAttached()
{
	BaseClass::OnGuiModelAttached();

	Q_ASSERT(IsGuiCreated());
	const isig::ITriggerParams* objectPtr = GetObservedObject();
	Q_ASSERT(objectPtr != NULL);

	const isig::ITriggerConstraints* constraintsPtr = objectPtr->GetTriggerConstraints();
	if (constraintsPtr != NULL){
		NoTriggerRB->setVisible(constraintsPtr->IsTriggerModeSupported(isig::ITriggerParams::TM_NONE));
		ContinuousRB->setVisible(constraintsPtr->IsTriggerModeSupported(isig::ITriggerParams::TM_CONTINUOUS));
		RisingEdgeRB->setVisible(constraintsPtr->IsTriggerModeSupported(isig::ITriggerParams::TM_RISING_EDGE));
		FallingEdgeRB->setVisible(constraintsPtr->IsTriggerModeSupported(isig::ITriggerParams::TM_FALLING_EDGE));
		PositiveLevelRB->setVisible(constraintsPtr->IsTriggerModeSupported(isig::ITriggerParams::TM_POSITIVE_LEVEL));
		NegativeLevelRB->setVisible(constraintsPtr->IsTriggerModeSupported(isig::ITriggerParams::TM_NEGATIVE_LEVEL));
		SoftwareTriggerRB->setVisible(constraintsPtr->IsTriggerModeSupported(isig::ITriggerParams::TM_SOFTWARE));
	}
	else{
		NoTriggerRB->setVisible(true);
		ContinuousRB->setVisible(true);
		RisingEdgeRB->setVisible(true);
		FallingEdgeRB->setVisible(true);
		PositiveLevelRB->setVisible(true);
		NegativeLevelRB->setVisible(true);
		SoftwareTriggerRB->setVisible(true);
	}
}


void CTriggerParamsGuiComp::UpdateModel() const
{
	Q_ASSERT(IsGuiCreated());

	isig::ITriggerParams* objectPtr = GetObservedObject();
	Q_ASSERT(objectPtr != NULL);

	int triggerMode = -1;

	if (NoTriggerRB->isChecked()){
		triggerMode = isig::ITriggerParams::TM_NONE;
	}
	else if (ContinuousRB->isChecked()){
		triggerMode = isig::ITriggerParams::TM_CONTINUOUS;
	}
	else if (RisingEdgeRB->isChecked()){
		triggerMode = isig::ITriggerParams::TM_RISING_EDGE;
	}
	else if (FallingEdgeRB->isChecked()){
		triggerMode = isig::ITriggerParams::TM_FALLING_EDGE;
	}
	else if (PositiveLevelRB->isChecked()){
		triggerMode = isig::ITriggerParams::TM_POSITIVE_LEVEL;
	}
	else if (NegativeLevelRB->isChecked()){
		triggerMode = isig::ITriggerParams::TM_NEGATIVE_LEVEL;
	}
	else if (SoftwareTriggerRB->isChecked()){
		triggerMode = isig::ITriggerParams::TM_SOFTWARE;
	}

	if ((triggerMode >= 0) && (triggerMode != objectPtr->GetTriggerMode())){
		istd::CChangeNotifier notifier(objectPtr);

		objectPtr->SetTriggerMode(triggerMode);
	}
}


void CTriggerParamsGuiComp::UpdateGui(const istd::IChangeable::ChangeSet& /*changeSet*/)
{
	Q_ASSERT(IsGuiCreated());

	const isig::ITriggerParams* objectPtr = GetObservedObject();
	if (objectPtr != NULL){
		int triggerMode = objectPtr->GetTriggerMode();

		NoTriggerRB->setChecked(triggerMode == isig::ITriggerParams::TM_NONE);
		ContinuousRB->setChecked(triggerMode == isig::ITriggerParams::TM_CONTINUOUS);
		RisingEdgeRB->setChecked(triggerMode == isig::ITriggerParams::TM_RISING_EDGE);
		FallingEdgeRB->setChecked(triggerMode == isig::ITriggerParams::TM_FALLING_EDGE);
		PositiveLevelRB->setChecked(triggerMode == isig::ITriggerParams::TM_POSITIVE_LEVEL);
		NegativeLevelRB->setChecked(triggerMode == isig::ITriggerParams::TM_NEGATIVE_LEVEL);
		SoftwareTriggerRB->setChecked(triggerMode == isig::ITriggerParams::TM_SOFTWARE);
	}
}


// protected slots

void CTriggerParamsGuiComp::on_NoTriggerRB_toggled(bool /*state*/)
{
	DoUpdateModel();
}


void CTriggerParamsGuiComp::on_ContinuousRB_toggled(bool /*state*/)
{
	DoUpdateModel();
}


void CTriggerParamsGuiComp::on_RisingEdgeRB_toggled(bool /*state*/)
{
	DoUpdateModel();
}


void CTriggerParamsGuiComp::on_FallingEdgeRB_toggled(bool /*state*/)
{
	DoUpdateModel();
}


void CTriggerParamsGuiComp::on_PositiveLevelRB_toggled(bool /*state*/)
{
	DoUpdateModel();
}


void CTriggerParamsGuiComp::on_NegativeLevelRB_toggled(bool /*state*/)
{
	DoUpdateModel();
}


void CTriggerParamsGuiComp::on_SoftwareTriggerRB_toggled(bool /*state*/)
{
	DoUpdateModel();
}


} // namespace iqtsig


