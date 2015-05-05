#include "iqtmeas/CNumericValueWidget.h"


// Qt includes
#include <QtCore/qmath.h>


namespace iqtmeas
{


CNumericValueWidget::CNumericValueWidget(
			QWidget* parentPtr,
			bool showSlilder,
			bool showButtons,
			int inputPolicy)
:	QWidget(parentPtr),
	m_unitMultiplicationFactor(1),
	m_unitPrecisionFactor(100)
{
	m_ignoreEvents = false;

	setupUi(this);

	ValueSlider->setVisible(showSlilder);
	MinButton->setVisible(showSlilder && showButtons);
	MaxButton->setVisible(showSlilder && showButtons);

	switch (inputPolicy){
		case 2:
			ValueSB->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
			DescriptionLabel->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
			break;

		case 1:
			ValueSB->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
			break;

		default:
			ValueSB->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
			break;
	}
}


void CNumericValueWidget::SetUnitInfo(const QString& description, const imath::IUnitInfo& unitInfo)
{
	DescriptionLabel->setVisible(!description.isEmpty());
	DescriptionLabel->setText(description);

	const QString& unitName = unitInfo.GetUnitName();
	if (!unitName.isEmpty()){
		UnitLabel->setText(unitName);
	}
	else{
		UnitLabel->setFixedWidth(0);
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

	double minValue = valueRange.GetMinValue() * m_unitMultiplicationFactor;
	double maxValue = valueRange.GetMaxValue() * m_unitMultiplicationFactor;

	ValueSB->setDecimals(displayPrecision);
	ValueSB->setRange(minValue, maxValue);
	ValueSlider->setRange(valueRange.GetMinValue() * m_unitPrecisionFactor, valueRange.GetMaxValue() * m_unitPrecisionFactor);
	MinButton->setText(QString::number(minValue));
	MaxButton->setText(QString::number(maxValue));
	ValueSB->setToolTip(tr("Range: %1 - %2").arg(minValue).arg(maxValue));
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


void CNumericValueWidget::SetupValueEditor(int editPrecision, double stepValue)
{
	if (editPrecision >= 0){
		ValueSB->setDecimals(editPrecision);
	}

	ValueSB->setSingleStep(stepValue);
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


void CNumericValueWidget::on_MinButton_clicked()
{
	ValueSB->setValue(ValueSB->minimum());
	ValueSlider->setValue(ValueSlider->minimum());
}


void CNumericValueWidget::on_MaxButton_clicked()
{
	ValueSB->setValue(ValueSB->maximum());
	ValueSlider->setValue(ValueSlider->maximum());
}



} // namespace iqtmeas


