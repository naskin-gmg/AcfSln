#include "CScaleCalibrationEditorComp.h"
#include <iostream>
#include <imath/CVarVector.h>
#include <QtGui/qmessagebox.h>


namespace iqtcam
{


void CScaleCalibrationEditorComp::UpdateModel() const
{
	BaseClass::UpdateModel();

	imeas::INumericValue* model = dynamic_cast<imeas::INumericValue*>(GetModelPtr());
	if (model == NULL){
		return;
	}

	imath::CVarVector vec;
	vec.SetElementsCount(2, 0);
	vec.SetElement(0, ScaleXSpinBox->value());
	vec.SetElement(1, ScaleYSpinBox->value());

	model->SetValues(vec);
}


void CScaleCalibrationEditorComp::OnGuiCreated()
{
	connect(ScaleXSpinBox, SIGNAL(valueChanged(double)), this, SLOT(OnValueChanged(double)));
	connect(ScaleYSpinBox, SIGNAL(valueChanged(double)), this, SLOT(OnValueChanged(double)));

	if (!m_circleProviderPtr.IsValid()){
		CalibrationGroupBox->setVisible(false);
	}

	if (NominalRadiusSpinBox->value() == 0){
		CalibrateButton->setDisabled(true);
	}
}


void CScaleCalibrationEditorComp::UpdateGui(int)
{
	imeas::INumericValue* model = dynamic_cast<imeas::INumericValue*>(GetModelPtr());
	if (model == NULL){
		return;
	}

	imath::CVarVector vec = model->GetValues();
	if (vec.GetElementsCount() >= 2){
		ScaleYSpinBox->setValue(vec[1]);
	}
	if (vec.GetElementsCount() >= 1){
		ScaleXSpinBox->setValue(vec[0]);
	}
}


void CScaleCalibrationEditorComp::OnValueChanged(double)
{
	DoUpdateModel();
}


void CScaleCalibrationEditorComp::on_CalibrateButton_clicked()
{
	if (!m_circleProviderPtr.IsValid()){
		return;
	}

	double nominalRadius = NominalRadiusSpinBox->value();
	if (nominalRadius == 0){
		return; // avoid division by zero
	}

	for (int i = 0; i < m_circleProviderPtr->GetValuesCount(); i++){
		const imeas::INumericValue& value = m_circleProviderPtr->GetNumericValue(i);
		if (!value.IsValueTypeSupported(imeas::INumericValue::VTI_RADIUS)){
			continue;
		}

		imath::CVarVector vec = value.GetValues();
		if (vec.GetElementsCount() > 0){
			double scaleX = vec[2] / nominalRadius;
			double scaleY = scaleX;
			if (vec.GetElementsCount() > 3){
				scaleX = vec[3] / nominalRadius;
			}

			ScaleXSpinBox->setValue(scaleX);
			ScaleYSpinBox->setValue(scaleY);

			DoUpdateModel();

			break;
		}
	}
}


void CScaleCalibrationEditorComp::on_NominalRadiusSpinBox_valueChanged(double d)
{
	CalibrateButton->setDisabled(d == 0 || !m_circleProviderPtr.IsValid());
}

} // namespace iqtcam

