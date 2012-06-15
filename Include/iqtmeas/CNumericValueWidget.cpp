#include "iqtmeas/CNumericValueWidget.h"


// Qt includes
#include <QtCore/qmath.h>


namespace iqtmeas
{


CNumericValueWidget::CNumericValueWidget(
			QWidget* parentPtr,
			bool showSlilder)
:	QWidget(parentPtr),
	m_unitMultiplicationFactor(1),
	m_unitPrecisionFactor(100)
{
	m_ignoreEvents = false;

	setupUi(this);

	ValueSlider->setVisible(showSlilder);
}


void CNumericValueWidget::SetUnitInfo(const QString& description, const imeas::IUnitInfo& unitInfo)
{
	DescriptionLabel->setText(description);
	const QString& unitName = unitInfo.GetUnitName();
	if (!unitName.isEmpty()){
		UnitLabel->setText(unitName);
		UnitLabel->setVisible(true);
	}
	else{
		UnitLabel->setVisible(false);
	}

	m_unitMultiplicationFactor = unitInfo.GetDisplayMultiplicationFactor();
	const imath::IDoubleManip& valueManip = unitInfo.GetValueManip();

	int precision = valueManip.GetPrecision();
	m_unitPrecisionFactor = qPow(10.0, double(qMin(precision, 2)));
	int displayPrecision = qMin(2, qMax(0, precision - int(log10(m_unitMultiplicationFactor) + 0.5)));

	istd::CRange valueRange = unitInfo.GetValueRange();
	if (valueRange.IsEmpty()){
		valueRange = istd::CRange(0, 100);
	}

	ValueSB->setDecimals(displayPrecision);
	ValueSB->setMinimum(valueRange.GetMinValue() * m_unitMultiplicationFactor);
	ValueSB->setMaximum(valueRange.GetMaxValue() * m_unitMultiplicationFactor);
	ValueSlider->setMinimum(valueRange.GetMinValue() * m_unitPrecisionFactor);
	ValueSlider->setMaximum(valueRange.GetMaxValue() * m_unitPrecisionFactor);
}


double CNumericValueWidget::GetValue() const
{
	return ValueSB->value() / m_unitMultiplicationFactor;
}


void CNumericValueWidget::SetValue(double value)
{
	m_ignoreEvents = true;
	ValueSB->setValue(value * m_unitMultiplicationFactor);
	ValueSlider->setValue(value * m_unitPrecisionFactor);
	m_ignoreEvents = false;
}


// protected slots

void CNumericValueWidget::on_ValueSB_valueChanged(double value)
{
	if (m_ignoreEvents){
		return;
	}

	m_ignoreEvents = true;
	SetValue(value / m_unitMultiplicationFactor);
	m_ignoreEvents = false;

	Q_EMIT ValueChanged();
}


void CNumericValueWidget::on_ValueSlider_valueChanged(int value)
{
	if (m_ignoreEvents){
		return;
	}

	m_ignoreEvents = true;
	SetValue(value / m_unitPrecisionFactor);
	m_ignoreEvents = false;

	Q_EMIT ValueChanged();
}


} // namespace iqtmeas


