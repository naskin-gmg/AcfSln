#include <icalibgui/CSimpleLensCorrectionEditorComp.h>


// ACF includes
#include <istd/CChangeGroup.h>


namespace icalibgui
{


// protected methods

// reimplemented (iqtgui::TGuiObserverWrap)

void CSimpleLensCorrectionEditorComp::UpdateModel() const
{
	Q_ASSERT(IsGuiCreated());

	icalib::CSimpleLensCorrection* objectPtr = GetObservedObject();
	Q_ASSERT(objectPtr != NULL);

	i2d::CVector2d opticalCenter(CenterXSB->value(), CenterYSB->value());
	double scaleFactor = ScaleSB->value() * 0.01;
	double distortionFactor = CorrectionFactorSB->value() * 0.000001;

	istd::CChangeGroup changeGroup(objectPtr);
	Q_UNUSED(changeGroup);

	objectPtr->SetOpticalCenter(opticalCenter);
	objectPtr->SetDistortionFactor(distortionFactor);
	objectPtr->SetScaleFactor(scaleFactor);
}


void CSimpleLensCorrectionEditorComp::UpdateGui(const istd::IChangeable::ChangeSet& /*changeSet*/)
{
	Q_ASSERT(IsGuiCreated());

	const icalib::CSimpleLensCorrection* objectPtr = GetObservedObject();
	if (objectPtr != NULL){
		const i2d::CVector2d& opticalCenter = objectPtr->GetOpticalCenter();
		double distortionFactor = objectPtr->GetDistortionFactor();
		double scaleFactor = objectPtr->GetScaleFactor();

		CenterXSB->setValue(opticalCenter.GetX());
		CenterYSB->setValue(opticalCenter.GetY());

		CorrectionFactorSB->setValue(distortionFactor * 1000000);
		ScaleSB->setValue(scaleFactor * 100);
	}

	bool isEnabled = (objectPtr != NULL);

	CenterXSB->setEnabled(isEnabled);
	CenterYSB->setEnabled(isEnabled);
	CorrectionFactorSB->setEnabled(isEnabled);
	ScaleSB->setEnabled(isEnabled);
}


void CSimpleLensCorrectionEditorComp::OnGuiCreated()
{
	BaseClass::OnGuiCreated();

	CalibrateButton->setVisible(m_calibProviderCompPtr.IsValid());

	QObject::connect(CenterXSB, SIGNAL(valueChanged(double)), this, SLOT(OnParamsChanged(double)));
	QObject::connect(CenterYSB, SIGNAL(valueChanged(double)), this, SLOT(OnParamsChanged(double)));
	QObject::connect(CorrectionFactorSB, SIGNAL(valueChanged(double)), this, SLOT(OnParamsChanged(double)));
	QObject::connect(ScaleSB, SIGNAL(valueChanged(double)), this, SLOT(OnParamsChanged(double)));

	if (m_calibProviderCompPtr.IsValid() && m_calibProviderModelCompPtr.IsValid()){
		RegisterModel(m_calibProviderModelCompPtr.GetPtr(), 0);
	}
}


void CSimpleLensCorrectionEditorComp::OnGuiDestroyed()
{
	UnregisterAllModels();

	QObject::disconnect(CenterXSB, SIGNAL(valueChanged(double)), this, SLOT(OnParamsChanged(double)));
	QObject::disconnect(CenterYSB, SIGNAL(valueChanged(double)), this, SLOT(OnParamsChanged(double)));
	QObject::disconnect(CorrectionFactorSB, SIGNAL(valueChanged(double)), this, SLOT(OnParamsChanged(double)));
	QObject::disconnect(ScaleSB, SIGNAL(valueChanged(double)), this, SLOT(OnParamsChanged(double)));

	BaseClass::OnGuiDestroyed();
}


// reimplemented (iqtgui::CGuiComponentBase)

void CSimpleLensCorrectionEditorComp::OnModelChanged(int /*modelId*/, const istd::IChangeable::ChangeSet& /*changeSet*/)
{
	Q_ASSERT(m_calibProviderCompPtr.IsValid());

	const i2d::ICalibration2d* calibrationPtr = m_calibProviderCompPtr->GetCalibration();
	CalibrateButton->setEnabled(calibrationPtr != NULL);
}


// protected slots

void CSimpleLensCorrectionEditorComp::on_CalibrateButton_clicked()
{
	Q_ASSERT(m_calibProviderCompPtr.IsValid());

	const i2d::ICalibration2d* newCalibrationPtr = m_calibProviderCompPtr->GetCalibration();

	icalib::CSimpleLensCorrection* objectPtr = GetObservedObject();
	if ((objectPtr != NULL) && (newCalibrationPtr != NULL)){
		objectPtr->CopyFrom(*newCalibrationPtr);
	}
}


void CSimpleLensCorrectionEditorComp::OnParamsChanged(double /*value*/)
{
	DoUpdateModel();
}


} // namespace icalibgui


