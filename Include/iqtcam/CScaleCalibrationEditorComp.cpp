// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#include <iqtcam/CScaleCalibrationEditorComp.h>


// STL includes
#include <iostream>

// Qt includes
#include<QtCore/QtGlobal>
#if QT_VERSION >= 0x050000
#include <QtWidgets/QMessageBox>
#else
#include <QtGui/QMessageBox>
#endif

// ACF includes
#include <istd/CChangeGroup.h>
#include <imath/CVarVector.h>
#include <i2d/CCircle.h>

// ACF-Solutions includes
#include <iinsp/ISupplier.h>


namespace iqtcam
{


// protected methods

// reimplemented (iqtgui::TGuiObserverWrap)

void CScaleCalibrationEditorComp::UpdateModel() const
{
	imeas::INumericValue* model = dynamic_cast<imeas::INumericValue*>(GetObservedModel());
	if (model == NULL){
		return;
	}

	imath::CVarVector vec;
	vec.SetElementsCount(2, 0);
	vec.SetElement(0, ScaleXSpinBox->value());
	vec.SetElement(1, ScaleYSpinBox->value());

	model->SetValues(vec);
}


void CScaleCalibrationEditorComp::UpdateGui(const istd::IChangeable::ChangeSet& /*changeSet*/)
{
	imeas::INumericValue* model = dynamic_cast<imeas::INumericValue*>(GetObservedModel());
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


// reimplemented (iqtgui::CGuiComponentBase)

void CScaleCalibrationEditorComp::OnGuiCreated()
{
	BaseClass::OnGuiCreated();

	connect(ScaleXSpinBox, SIGNAL(valueChanged(double)), this, SLOT(OnValueChanged(double)));
	connect(ScaleYSpinBox, SIGNAL(valueChanged(double)), this, SLOT(OnValueChanged(double)));

	if (!m_circleProviderCompPtr.IsValid()){
		CalibrationGroupBox->setVisible(false);
	}

	if (NominalRadiusSpinBox->value() == 0){
		CalibrateButton->setDisabled(true);
	}
}


// protected slots

void CScaleCalibrationEditorComp::OnValueChanged(double)
{
	DoUpdateModel();
}


void CScaleCalibrationEditorComp::on_CalibrateButton_clicked()
{
	if (!m_circleProviderCompPtr.IsValid()){
		return;
	}

	double nominalRadius = NominalRadiusSpinBox->value();
	if (nominalRadius == 0){
		return; // avoid division by zero
	}

	if (m_calibrationDialogCompPtr.IsValid()){
		int retVal = m_calibrationDialogCompPtr->ExecuteDialog(this);
		if (retVal == QDialog::Rejected){
			return;
		}
	}

	iinsp::ISupplier* supplierPtr = dynamic_cast<iinsp::ISupplier*>(m_circleProviderCompPtr.GetPtr());
	if (supplierPtr != NULL){
		istd::CChangeGroup changeGroup(supplierPtr);

		supplierPtr->InvalidateSupplier();
		supplierPtr->EnsureWorkInitialized();
		supplierPtr->EnsureWorkFinished();

		if (supplierPtr->GetWorkStatus() != iinsp::ISupplier::WS_OK){
			return;
		}
	}

	for (int i = 0; i < m_circleProviderCompPtr->GetFeaturesCount(); i++){
		i2d::CCircle circle;
		if (!circle.CopyFrom(m_circleProviderCompPtr->GetFeature(i), istd::IChangeable::CM_CONVERT)){
			continue;
		}

		double scaleX = circle.GetRadius() / nominalRadius;
		double scaleY = scaleX;

		ScaleXSpinBox->setValue(scaleX);
		ScaleYSpinBox->setValue(scaleY);

		DoUpdateModel();

		break;
	}
}


void CScaleCalibrationEditorComp::on_NominalRadiusSpinBox_valueChanged(double d)
{
	CalibrateButton->setDisabled(d == 0 || !m_circleProviderCompPtr.IsValid());
}

} // namespace iqtcam

