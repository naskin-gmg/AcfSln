/* 
 * File:   CScaleCalibrationEditorComp.h
 * Author: SOL
 *
 * Created on 29 listopad 2012, 13:50
 */

#ifndef iqtmeas_CScaleCalibrationEditorComp_included
#define	iqtmeas_CScaleCalibrationEditorComp_included


// Qt includes
#include <QtGui/QFrame>
#include <QtGui/QGroupBox>


// ACF includes
#include "iqtgui/TDesignerGuiObserverCompBase.h"
#include <imeas/INumericValueProvider.h>
#include <istd/TPointerVector.h>

#include "Generated/ui_CScaleCalibrationEditorComp.h"

namespace iqtmeas
{


class CScaleCalibrationEditorComp:
public iqtgui::TDesignerGuiObserverCompBase<
Ui::CScaleCalibrationEditorComp, imeas::INumericValue>
{
	Q_OBJECT
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

	// reimplemented (imod::IModelEditor)
	virtual void UpdateEditor(int changeFlags);

	public
Q_SLOTS:
	void OnValueChanged(double);
	void on_CalibrateButton_clicked();
private:
	I_REF(imeas::INumericValueProvider, m_circleProviderPtr);
};


} // namespace iqtmeas


#endif	/* iqtmeas_CScaleCalibrationEditorComp_included */

