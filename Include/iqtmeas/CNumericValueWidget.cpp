#include <iqtmeas/CNumericValueWidget.h>


// STL includes
#include <limits>

// ACF includes
#include <iqt/CSignalBlocker.h>

// Qt includes
#include <QtCore/QtMath>


namespace iqtmeas
{


CNumericValueWidget::CNumericValueWidget(
			QWidget* parentPtr,
			int sliderFlags,
			int inputPolicy,
			int maxPrecision,
			bool isPostValidationEnabled)
:	QWidget(parentPtr),
	m_unitMultiplicationFactor(1),
	m_sliderScaleFactor(100),
	m_maxEditPrecision(maxPrecision),
	m_isPostValidationEnabled(isPostValidationEnabled),
	m_valueRange(0, 100)
{
	m_ignoreEvents = false;

	setupUi(this);

	ValueSlider->setTracking(false);

	if (sliderFlags & SF_SINGLE_ROW){
		ValueLayout->addWidget(MinButton);
		ValueLayout->addWidget(ValueSlider);
		ValueLayout->addWidget(MaxButton);
	}

	ValueSlider->setVisible(sliderFlags != SF_NONE);
	MinButton->setVisible(sliderFlags & SF_SLIDER_BUTTONS);
	MaxButton->setVisible(sliderFlags & SF_SLIDER_BUTTONS);

	switch (inputPolicy){
		case 2:
		ValueSB->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
		DescriptionLabel->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
		break;

	case 1:
		ValueSB->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
		break;

	case 0:
		ValueSB->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
		break;
	}
}


void CNumericValueWidget::SetUnitInfo(const QString& name, const QString& description, const imath::IUnitInfo* unitInfoPtr)
{
	iqt::CSignalBlocker blocker1(ValueSB);
	iqt::CSignalBlocker blocker2(ValueSlider);

	DescriptionLabel->setVisible(!name.isEmpty());
	DescriptionLabel->setText(name);

	setToolTip(description);

	QString unitName;
	m_unitMultiplicationFactor = 1;
	int precision = 2;
	
	if (unitInfoPtr != NULL){
		unitName = unitInfoPtr->GetUnitName();
		m_unitMultiplicationFactor = unitInfoPtr->GetDisplayMultiplicationFactor();
		precision = unitInfoPtr->GetValueManip().GetPrecision();

		istd::CRange range = unitInfoPtr->GetValueRange();
		if (range.IsValid()){
			m_valueRange = range;
		}
	}

	UnitLabel->setText(unitName);
	UnitLabel->setVisible(!unitName.isEmpty());

	int multiplicationPrecision = int(log10(m_unitMultiplicationFactor) + 0.5);
	int displayPrecision = qMin(m_maxEditPrecision, qMax(0, precision - multiplicationPrecision));
	m_sliderScaleFactor = qPow(10.0, double(displayPrecision + multiplicationPrecision));

	double minValue = m_valueRange.GetMinValue() * m_unitMultiplicationFactor;
	double maxValue = m_valueRange.GetMaxValue() * m_unitMultiplicationFactor;

	ValueSB->setDecimals(displayPrecision);

	if (!m_isPostValidationEnabled){
		ValueSB->setRange(minValue, maxValue);
	}
	else{
		double initialRangeMin = minValue >= 0 ? 0 : -std::numeric_limits<double>::max();
		double initialRangeMax = maxValue < 0 ? 0 : std::numeric_limits<double>::max();

		ValueSB->setRange(initialRangeMin, initialRangeMax);
	}

	ValueSB->setSingleStep(displayPrecision > 0 ? qPow(0.1, displayPrecision) : 1);
	ValueSB->setToolTip(tr("Range: %1 - %2").arg(minValue).arg(maxValue));

	ValueSlider->setRange(m_valueRange.GetMinValue() * m_sliderScaleFactor, m_valueRange.GetMaxValue() * m_sliderScaleFactor);
	ValueSlider->setPageStep(10 * ValueSlider->singleStep());

	MinButton->setText(QString::number(minValue));
	MaxButton->setText(QString::number(maxValue));
}


double CNumericValueWidget::GetValue() const
{
	return ValueSB->value() / m_unitMultiplicationFactor;
}


void CNumericValueWidget::SetValue(double value)
{
	m_ignoreEvents = true;
	ValueSB->setValue(value * m_unitMultiplicationFactor);
	ValueSlider->setValue(value * m_sliderScaleFactor);
	m_ignoreEvents = false;
}


// protected slots

void CNumericValueWidget::on_ValueSB_valueChanged(double value)
{
	if (m_isPostValidationEnabled){
		return;
	}

	if (m_ignoreEvents){
		return;
	}

	m_ignoreEvents = true;
	ValueSlider->setValue(value * m_sliderScaleFactor / m_unitMultiplicationFactor);
	m_ignoreEvents = false;

	Q_EMIT ValueChanged();
}


void CNumericValueWidget::on_ValueSB_editingFinished()
{
	if (!m_isPostValidationEnabled){
		return;
	}

	if (m_ignoreEvents){
		return;
	}

	double value = ValueSB->value();

	if (!m_valueRange.Contains(value)){
		value = m_valueRange.GetNearestInside(value);

		// Force slider update:
		m_isPostValidationEnabled = false;

		ValueSB->setValue(value);

		m_isPostValidationEnabled = true;

		return;
	}

	m_ignoreEvents = true;

	ValueSlider->setValue(value * m_sliderScaleFactor / m_unitMultiplicationFactor);
	m_ignoreEvents = false;

	Q_EMIT ValueChanged();
}


void CNumericValueWidget::on_ValueSlider_valueChanged(int value)
{
	if (m_ignoreEvents){
		return;
	}

	m_ignoreEvents = true;
	SetValue(value / m_sliderScaleFactor);
	m_ignoreEvents = false;

	Q_EMIT ValueChanged();
}


void CNumericValueWidget::on_ValueSlider_sliderMoved(int value)
{
	if (m_ignoreEvents){
		return;
	}

	m_ignoreEvents = true;
	SetValue(value / m_sliderScaleFactor);
	m_ignoreEvents = false;
}


void CNumericValueWidget::on_ValueSlider_sliderReleased()
{
	Q_EMIT ValueChanged();
}


void CNumericValueWidget::on_MinButton_clicked()
{
	SetValue(m_valueRange.GetMinValue());

	Q_EMIT ValueChanged();
}


void CNumericValueWidget::on_MaxButton_clicked()
{
	SetValue(m_valueRange.GetMaxValue());

	Q_EMIT ValueChanged();
}


} // namespace iqtmeas


