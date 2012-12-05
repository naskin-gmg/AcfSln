#include "CScaleCalibrationEditorComp.h"
#include <iostream>
#include <imath/CVarVector.h>
#define SHOWDEBUG(x) std::cerr<<__FILE__<<"@"<<__LINE__<<":"<<#x << "=`" << x << "'" << std::endl


namespace iqtmeas
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
}


void CScaleCalibrationEditorComp::UpdateGui(int)
{
	imeas::INumericValue* model = dynamic_cast<imeas::INumericValue*>(GetModelPtr());
	if (model == NULL){
		return;
	}

	imath::CVarVector vec = model->GetValues();
	if (vec.GetElementsCount() < 2){
		return;
	}

	ScaleXSpinBox->setValue(vec[0]);
	ScaleYSpinBox->setValue(vec[1]);
}


void CScaleCalibrationEditorComp::UpdateEditor(int changeFlags)
{
	UpdateGui(changeFlags);
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
		return;
	}

	for (int i = 0; i < m_circleProviderPtr->GetValuesCount(); i++){
		const imeas::INumericValue& value = m_circleProviderPtr->GetNumericValue(i);
		if (!value.IsValueTypeSupported(imeas::INumericValue::VTI_RADIUS)){
			continue;
		}

		imath::CVarVector vec = value.GetValues();
		if (vec.GetElementsCount() > 0){
			CalibrationGroupBox->setVisible(true);

			double scale = vec[0] / nominalRadius;
			ScaleXSpinBox->setValue(scale);
			ScaleYSpinBox->setValue(scale);

			DoUpdateModel();

			break;
		}
	}
}


} // namespace iqtmeas

