#ifndef iqtcam_CScaleCalibrationEditorComp_included
#define	iqtcam_CScaleCalibrationEditorComp_included


// Qt includes
#include<QtCore/QtGlobal>
#if QT_VERSION >= 0x050000
#include <QtWidgets/QFrame>
#include <QtWidgets/QGroupBox>
#else
#include <QtGui/QFrame>
#include <QtGui/QGroupBox>
#endif

// ACF includes
#include "iqtgui/TDesignerGuiObserverCompBase.h"
#include "istd/TPointerVector.h"

// ACF-Solutions includes
#include "imeas/INumericValueProvider.h"

#include "GeneratedFiles/iqtcam/ui_CScaleCalibrationEditorComp.h"


namespace iqtcam
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
		I_ASSIGN(m_circleProviderPtr, "CircleProvider", "External circle provider providing radius used for scale calibration\nThe provider must provide a numeric value of type VTI_RADIUS", false, "CircleProvider");
		I_REGISTER_INTERFACE(imod::IModelEditor);
	I_END_COMPONENT;

protected:
	// reimplemented (imod::IModelEditor)
	virtual void UpdateModel() const;

	// reimplemented (iqtgui::CGuiComponentBase)
	virtual void OnGuiCreated();

	// reimplemented (iqtgui::TGuiObserverWrap)
	virtual void UpdateGui(const istd::IChangeable::ChangeSet& changeSet);

private Q_SLOTS:
	void OnValueChanged(double);
	void on_CalibrateButton_clicked();
	void on_NominalRadiusSpinBox_valueChanged(double d);

private:
	I_REF(imeas::INumericValueProvider, m_circleProviderPtr);
};


} // namespace iqtcam


#endif	// !iqtcam_CScaleCalibrationEditorComp_included


