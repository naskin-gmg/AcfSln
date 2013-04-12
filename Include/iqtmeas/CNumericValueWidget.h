#ifndef iqtmeas_CNumericValueWidget_included
#define iqtmeas_CNumericValueWidget_included


// Qt includes
#include <QtGui/QWidget>


#include "imath/IUnitInfo.h"

#include "Generated/ui_CNumericValueWidget.h"


namespace iqtmeas
{


class CNumericValueWidget:
			public QWidget,
			protected Ui::CNumericValueWidget
{
	Q_OBJECT

public:
	CNumericValueWidget(QWidget* parentPtr = NULL, bool showSlilder = true, int inputPolicy = 0);

	void SetUnitInfo(const QString& description, const imath::IUnitInfo& unitInfo);

	double GetValue() const;
	void SetValue(double value);

Q_SIGNALS:
	/**
		Will be emited when user changed value.
	*/
	void ValueChanged();

protected Q_SLOTS:
	void on_ValueSB_valueChanged(double value);
	void on_ValueSlider_valueChanged(int value);

private:
	bool m_ignoreEvents;
	double m_unitMultiplicationFactor;
	double m_unitPrecisionFactor;
};


} // namespace iqtmeas


#endif // !iqtmeas_CNumericValueWidget_included



