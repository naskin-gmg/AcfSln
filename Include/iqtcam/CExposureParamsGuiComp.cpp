#include <iqtcam/CExposureParamsGuiComp.h>


// ACF includes
#include <istd/CChangeGroup.h>

// ACF-Solutions includes
#include <icam/IExposureConstraints.h>


namespace iqtcam
{


// protected methods

// reimplemented (iqtgui::TGuiObserverWrap)

void CExposureParamsGuiComp::OnGuiCreated()
{
	BaseClass::OnGuiCreated();

	if (m_fixedLabelSizeAttrPtr.IsValid()) {
		ShutterTimeLabel->setFixedWidth(*m_fixedLabelSizeAttrPtr);
		DelayTimeLabel->setFixedWidth(*m_fixedLabelSizeAttrPtr);
		EenDelayTimeLabel->setFixedWidth(*m_fixedLabelSizeAttrPtr);
		GainLabel->setFixedWidth(*m_fixedLabelSizeAttrPtr);
	}
}


void CExposureParamsGuiComp::OnGuiModelAttached()
{
	BaseClass::OnGuiModelAttached();

	QObject::connect(ShutterTimeSB, SIGNAL(valueChanged(double)), this, SLOT(OnParamsChanged(double)));
	QObject::connect(DelayTimeSB, SIGNAL(valueChanged(double)), this, SLOT(OnParamsChanged(double)));
	QObject::connect(EenDelayTimeSB, SIGNAL(valueChanged(double)), this, SLOT(OnParamsChanged(double)));
	QObject::connect(GainSB, SIGNAL(valueChanged(int)), this, SLOT(OnParamsChanged(int)));
}


void CExposureParamsGuiComp::OnGuiModelDetached()
{
	QObject::disconnect(ShutterTimeSB, SIGNAL(valueChanged(double)), this, SLOT(OnParamsChanged(double)));
	QObject::disconnect(DelayTimeSB, SIGNAL(valueChanged(double)), this, SLOT(OnParamsChanged(double)));
	QObject::disconnect(EenDelayTimeSB, SIGNAL(valueChanged(double)), this, SLOT(OnParamsChanged(double)));
	QObject::disconnect(GainSB, SIGNAL(valueChanged(int)), this, SLOT(OnParamsChanged(int)));

	if (*m_hideUnusedParamsAttrPtr) {
		ShutterTimeLabel->setVisible(false);
		ShutterTimeSB->setVisible(false);
		ShutterTimeUnitLabel->setVisible(false);

		DelayTimeLabel->setVisible(false);
		DelayTimeSB->setVisible(false);
		DelayTimeUnitLabel->setVisible(false);

		EenDelayTimeLabel->setVisible(false);
		EenDelayTimeSB->setVisible(false);
		EenDelayTimeUnitLabel->setVisible(false);

		GainLabel->setVisible(false);
		GainSB->setVisible(false);
		GainUnitLabel->setVisible(false);
	}
	else {
		ShutterTimeLabel->setEnabled(false);
		ShutterTimeSB->setEnabled(false);
		ShutterTimeUnitLabel->setEnabled(false);

		DelayTimeLabel->setEnabled(false);
		DelayTimeSB->setEnabled(false);
		DelayTimeUnitLabel->setEnabled(false);

		EenDelayTimeLabel->setEnabled(false);
		EenDelayTimeSB->setEnabled(false);
		EenDelayTimeUnitLabel->setEnabled(false);

		GainLabel->setEnabled(false);
		GainSB->setEnabled(false);
		GainUnitLabel->setEnabled(false);
	}

	BaseClass::OnGuiModelDetached();
}


void CExposureParamsGuiComp::UpdateModel() const
{
	Q_ASSERT(IsGuiCreated());

	icam::IExposureParams* objectPtr = GetObservedObject();
	Q_ASSERT(objectPtr != NULL);

	double tolerance = 0.9e-6;

	istd::CChangeGroup changeGroup(objectPtr);
	Q_UNUSED(changeGroup);

	double shutterTime = ShutterTimeSB->value() * 0.000001;
	if (ShutterTimeSB->isVisible() && (qAbs(objectPtr->GetShutterTime() - shutterTime) > tolerance)){
		objectPtr->SetShutterTime(shutterTime);
	}

	double delayTime = DelayTimeSB->value() * 0.000001;
	if (DelayTimeSB->isVisible() && (qAbs(objectPtr->GetDelayTime() - delayTime) > tolerance)){
		objectPtr->SetDelayTime(delayTime);
	}

	double eenDelayTime = EenDelayTimeSB->value() * 0.000001;
	if (EenDelayTimeSB->isVisible() && (qAbs(objectPtr->GetEenDelay() - eenDelayTime) > tolerance)){
		objectPtr->SetEenDelay(eenDelayTime);
	}

	int gain = GainSB->value();
	if (GainSB->isVisible() && objectPtr->GetGainValue() != gain) {
		objectPtr->SetGainValue(gain);
	}
}


void CExposureParamsGuiComp::UpdateGui(const istd::IChangeable::ChangeSet& /*changeSet*/)
{
	Q_ASSERT(IsGuiCreated());

	AdjustVisibility();

	icam::IExposureParams* objectPtr = GetObservedObject();
	if (objectPtr != NULL){
		const int cameraIndex = objectPtr->GetCurrentCameraIndex();
		const icam::IExposureConstraints* constrainsPtr = objectPtr->GetExposureConstraints();
		if (constrainsPtr != NULL) {
			double shutterMin = constrainsPtr->GetShutterTimeRange(cameraIndex).GetMinValue() * 1000000.0;
			double shutterMax = constrainsPtr->GetShutterTimeRange(cameraIndex).GetMaxValue() * 1000000.0;
			
			if (m_exposureLimitAttrPtr.IsValid()) {
				shutterMax = qMin<double>(shutterMax, *m_exposureLimitAttrPtr);
			}

			ShutterTimeSB->setMinimum(shutterMin);
			ShutterTimeSB->setMaximum(shutterMax);
			ShutterTimeSB->setToolTip(QString(tr("Shutter time range [%1, %2]")).arg(shutterMin).arg(shutterMax));

			DelayTimeSB->setMinimum(constrainsPtr->GetDelayTimeRange(cameraIndex).GetMinValue() * 1000000.0);
			DelayTimeSB->setMaximum(constrainsPtr->GetDelayTimeRange(cameraIndex).GetMaxValue() * 1000000.0);

			EenDelayTimeSB->setMinimum(constrainsPtr->GetEenDelayRange(cameraIndex).GetMinValue() * 1000000.0);
			EenDelayTimeSB->setMaximum(constrainsPtr->GetEenDelayRange(cameraIndex).GetMaxValue() * 1000000.0);

			GainSB->setMinimum(constrainsPtr->GetGainRange(cameraIndex).GetMinValue());
			GainSB->setMaximum(constrainsPtr->GetGainRange(cameraIndex).GetMaxValue());
		}

		ShutterTimeSB->setValue(objectPtr->GetShutterTime() * 1000000.0);
		DelayTimeSB->setValue(objectPtr->GetDelayTime() * 1000000.0);
		EenDelayTimeSB->setValue(objectPtr->GetEenDelay() * 1000000.0);
		GainSB->setValue(objectPtr->GetGainValue());
	}
}


// protected slots

void CExposureParamsGuiComp::OnParamsChanged(double /*value*/)
{
	DoUpdateModel();
}


void CExposureParamsGuiComp::OnParamsChanged(int /*value*/)
{
	DoUpdateModel();
}


void CExposureParamsGuiComp::AdjustVisibility()
{
	bool isShutterTimeSupported = false;
	bool isDelayTimeSupported = false;
	bool isEenDelayTimeSupported = false;
	bool isGainSupported = false;

	const icam::IExposureParams* objectPtr = GetObservedObject();
	if (objectPtr != NULL) {
		const int cameraIndex = objectPtr->GetCurrentCameraIndex();
		const icam::IExposureConstraints* constraintsPtr = objectPtr->GetExposureConstraints();
		if (constraintsPtr != NULL) {
			isShutterTimeSupported = !constraintsPtr->GetShutterTimeRange(cameraIndex).IsEmpty() && constraintsPtr->GetShutterTimeRange(cameraIndex).IsValid();
			isDelayTimeSupported = !constraintsPtr->GetDelayTimeRange(cameraIndex).IsEmpty() && constraintsPtr->GetDelayTimeRange(cameraIndex).IsValid();
			isEenDelayTimeSupported = !constraintsPtr->GetEenDelayRange(cameraIndex).IsEmpty() && constraintsPtr->GetEenDelayRange(cameraIndex).IsValid();
			isGainSupported = !constraintsPtr->GetGainRange(cameraIndex).IsEmpty() && constraintsPtr->GetGainRange(cameraIndex).IsValid();
		}
	}

	if (*m_hideUnusedParamsAttrPtr) {
		ShutterTimeLabel->setVisible(isShutterTimeSupported);
		ShutterTimeSB->setVisible(isShutterTimeSupported);
		ShutterTimeUnitLabel->setVisible(isShutterTimeSupported);

		DelayTimeLabel->setVisible(isDelayTimeSupported);
		DelayTimeSB->setVisible(isDelayTimeSupported);
		DelayTimeUnitLabel->setVisible(isDelayTimeSupported);

		EenDelayTimeLabel->setVisible(isEenDelayTimeSupported);
		EenDelayTimeSB->setVisible(isEenDelayTimeSupported);
		EenDelayTimeUnitLabel->setVisible(isEenDelayTimeSupported);

		GainLabel->setVisible(isGainSupported);
		GainSB->setVisible(isGainSupported);
		GainUnitLabel->setVisible(isGainSupported);
	}
	else
	{
		ShutterTimeLabel->setEnabled(isShutterTimeSupported);
		ShutterTimeSB->setEnabled(isShutterTimeSupported);
		ShutterTimeUnitLabel->setEnabled(isShutterTimeSupported);

		DelayTimeLabel->setEnabled(isDelayTimeSupported);
		DelayTimeSB->setEnabled(isDelayTimeSupported);
		DelayTimeUnitLabel->setEnabled(isDelayTimeSupported);

		EenDelayTimeLabel->setEnabled(isEenDelayTimeSupported);
		EenDelayTimeSB->setEnabled(isEenDelayTimeSupported);
		EenDelayTimeUnitLabel->setEnabled(isEenDelayTimeSupported);

		GainLabel->setEnabled(isGainSupported);
		GainSB->setEnabled(isGainSupported);
		GainUnitLabel->setEnabled(isGainSupported);
	}
}


} // namespace iqtcam


