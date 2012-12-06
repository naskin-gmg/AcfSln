/* 
 * File:   CScaleCalibrationEditorComp.h
 * Author: SOL
 *
 * Created on 29 listopad 2012, 13:50
 */

#ifndef iqtcam_CScaleCalibrationEditorComp_included
#define	iqtcam_CScaleCalibrationEditorComp_included


// Qt includes
#include <QtGui/QFrame>
#include <QtGui/QGroupBox>


// Acf includes
#include "iqtgui/TDesignerGuiObserverCompBase.h"
#include <istd/TPointerVector.h>

// AcfSln includes
#include <imeas/INumericValueProvider.h>


#include "Generated/ui_CScaleCalibrationEditorComp.h"

namespace iqtcam
{


class CScaleCalibrationEditorComp:
public iqtgui::TDesignerGuiObserverCompBase<
Ui::CScaleCalibrationEditorComp, imeas::INumericValue>
{
public:
	typedef iqtgui::TDesignerGuiObserverCompBase<
	Ui::CScaleCalibrationEditorComp, imeas::INumericValue> BaseClass;

	I_BEGIN_COMPONENT(CScaleCalibrationEditorComp);
	I_ASSIGN(m_circleProviderPtr, "CircleProvider", "External circle provider providing radius used for scale calibration\n"
			"The provider must provide a numeric value of type VTI_RADIUS", false, "CircleProvider");
	I_REGISTER_INTERFACE(imod::IModelEditor);
	I_END_COMPONENT;

protected:
	// reimplemented (imod::IModelEditor)
	virtual void UpdateModel() const;

	// reimplemented (iqtgui::CGuiComponentBase)
	virtual void OnGuiCreated();

	// reimplemented (iqtgui::TGuiObserverWrap)
	virtual void UpdateGui(int changeFlags);

	Q_OBJECT
	private Q_SLOTS:
	void OnValueChanged(double);
	void on_CalibrateButton_clicked();
	void on_NominalRadiusSpinBox_valueChanged(double d);

private:
	I_REF(imeas::INumericValueProvider, m_circleProviderPtr);
};


} // namespace iqtcam


#endif	/* iqtcam_CScaleCalibrationEditorComp_included */

