#ifndef iqtmeas_CNumericValueWidget_included
#define iqtmeas_CNumericValueWidget_included


// Qt includes
#include<QtCore/QtGlobal>
#if QT_VERSION >= 0x050000
#include <QtWidgets/QWidget>
#else
#include <QtGui/QWidget>
#endif

// ACF includes
#include "imath/IUnitInfo.h"

#include "GeneratedFiles/iqtmeas/ui_CNumericValueWidget.h"


namespace iqtmeas
{


class CNumericValueWidget:
			public QWidget,
			protected Ui::CNumericValueWidget
{
	Q_OBJECT

public:
	enum SliderFlags
	{
		SF_NONE = 0,
		SF_SLIDER_ONLY = 1,
		SF_SLIDER_BUTTONS = 2,
		SF_SINGLE_ROW = 8	// can be ORed
	};

	CNumericValueWidget(QWidget* parentPtr = NULL, int sliderFlags = SF_SLIDER_BUTTONS, int inputPolicy = 0, int maxPrecision = 2);

	void SetUnitInfo(const QString& name, const QString& description, const imath::IUnitInfo* unitInfoPtr);

	double GetValue() const;
	void SetValue(double value);

Q_SIGNALS:
	/**
		Will be emitted when user changed value.
	*/
	void ValueChanged();

protected Q_SLOTS:
	void on_ValueSB_valueChanged(double value);
	void on_ValueSlider_valueChanged(int value);
	void on_ValueSlider_sliderReleased();
	void on_MinButton_clicked();
	void on_MaxButton_clicked();

private:
	bool m_ignoreEvents;
	double m_unitMultiplicationFactor;
	double m_sliderScaleFactor;
	int m_maxEditPrecision;
};


} // namespace iqtmeas


#endif // !iqtmeas_CNumericValueWidget_included



