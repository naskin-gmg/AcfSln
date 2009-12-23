#include "iqtmeas/CSamplesSequenceViewComp.h"


// Qt includes
#include <QPainter>

// ACF includes
#include "iqt/CSignalBlocker.h"


namespace iqtmeas
{


CSamplesSequenceViewComp::CSamplesSequenceViewComp()
:	m_lastChannelsCount(0),
	m_channelsSelectorModel(1, 1)
{
	m_channelsSelectorModel.setItem(0, 0, new QStandardItem(tr("All channels")));
}


// reimplemented (imod::IModelEditor)

void CSamplesSequenceViewComp::UpdateModel() const
{
}


void CSamplesSequenceViewComp::UpdateEditor(int /*updateFlags*/)
{
	int channelsCount = 0;

	imeas::ISamplesSequence* objectPtr = GetObjectPtr();
	if (objectPtr != NULL){
		channelsCount = objectPtr->GetChannelsCount();
		if (channelsCount != m_lastChannelsCount){
			m_channelsSelectorModel.setRowCount(channelsCount + 1);
			for (int i = m_lastChannelsCount; i < channelsCount; ++i){
				m_channelsSelectorModel.setItem(i + 1, 0, new QStandardItem(tr("Channel %1").arg(i + 1)));
			}

			m_lastChannelsCount = channelsCount;
		}
	}

	if (IsGuiCreated()){
		ChannelsFrame->setVisible(channelsCount > 1);

		emit MainFrame->repaint();
	}
}


// protected methods

// reimplemented (iqtgui::CGuiComponentBase)

void CSamplesSequenceViewComp::OnGuiCreated()
{
	BaseClass::OnGuiCreated();

	DiagramWidget* newDiagramPtr = new DiagramWidget(MainFrame, this);
	QLayout* layoutPtr = MainFrame->layout();
	if (layoutPtr != NULL){
		layoutPtr->addWidget(newDiagramPtr);
	}

	ScaleFrame->setVisible(*m_showScalePanelAttrPtr);
	ScaleControlFrame->setVisible(*m_showScalePanelAttrPtr);
	TimeSpanFrame->setVisible(*m_showTimeSpanPanelAttrPtr);
	TimeSpanCB->setVisible(*m_showTimeSpanPanelAttrPtr);
	ValueMinSB->setValue(*m_defaultMinValueAttrPtr);
	ValueMaxSB->setValue(*m_defaultMaxValueAttrPtr);
	ScaleCB->setChecked(!*m_defaultScaledValueAttrPtr);

	ChannelSelectorCB->setModel(&m_channelsSelectorModel);
}


// protected signals

void CSamplesSequenceViewComp::on_TimeSpanCB_toggled(bool /*state*/)
{
	UpdateEditor();
}


void CSamplesSequenceViewComp::on_ScaleCB_toggled(bool /*state*/)
{
	UpdateEditor();
}


void CSamplesSequenceViewComp::on_TimeBeginSB_valueChanged(double /*value*/)
{
	UpdateEditor();
}


void CSamplesSequenceViewComp::on_TimeEndSB_valueChanged(double /*value*/)
{
	UpdateEditor();
}


void CSamplesSequenceViewComp::on_ValueMinSB_valueChanged(double /*value*/)
{
	UpdateEditor();
}


void CSamplesSequenceViewComp::on_ValueMaxSB_valueChanged(double /*value*/)
{
	UpdateEditor();
}


void CSamplesSequenceViewComp::on_ChannelSelectorCB_currentIndexChanged(int /*index*/)
{
	UpdateEditor();
}


void CSamplesSequenceViewComp::on_ZoomInButton_clicked()
{
	const imeas::ISamplesSequence* samplesPtr = GetObjectPtr();
	if (samplesPtr == NULL){
		return;
	}

	iqt::CSignalBlocker blocker1(TimeBeginSB);
	iqt::CSignalBlocker blocker2(TimeEndSB);

	double center = (TimeBeginSB->value() + TimeEndSB->value()) * 0.5;
	TimeBeginSB->setValue((TimeBeginSB->value() + center) * 0.5);
	TimeEndSB->setValue((TimeEndSB->value() + center) * 0.5);

	UpdateEditor();
}


void CSamplesSequenceViewComp::on_ZoomOutButton_clicked()
{
	const imeas::ISamplesSequence* samplesPtr = GetObjectPtr();
	if (samplesPtr == NULL){
		return;
	}

	double maxTimeSpan = samplesPtr->GetTimeSamplesCount() * samplesPtr->GetSamplingPeriod();

	iqt::CSignalBlocker blocker1(TimeBeginSB);
	iqt::CSignalBlocker blocker2(TimeEndSB);

	double center = (TimeBeginSB->value() + TimeEndSB->value()) * 0.5;
	TimeBeginSB->setValue(istd::Max(0.0, TimeBeginSB->value() * 2 - center));
	TimeEndSB->setValue(istd::Min(TimeEndSB->value() * 2 - center, maxTimeSpan));

	UpdateEditor();
}


void CSamplesSequenceViewComp::on_PrevButton_clicked()
{
	const imeas::ISamplesSequence* samplesPtr = GetObjectPtr();
	if (samplesPtr == NULL){
		return;
	}

	iqt::CSignalBlocker blocker1(TimeBeginSB);
	iqt::CSignalBlocker blocker2(TimeEndSB);

	double page = TimeEndSB->value() - TimeBeginSB->value();

	double shift = istd::Min(istd::Min(TimeBeginSB->value(), TimeEndSB->value()), page * 0.5);

	if (shift < I_BIG_EPSILON){
		return;
	}

	TimeBeginSB->setValue(TimeBeginSB->value() - shift);
	TimeEndSB->setValue(TimeEndSB->value() - shift);

	UpdateEditor();
}


void CSamplesSequenceViewComp::on_NextButton_clicked()
{
	const imeas::ISamplesSequence* samplesPtr = GetObjectPtr();
	if (samplesPtr == NULL){
		return;
	}

	double maxTimeSpan = samplesPtr->GetTimeSamplesCount() * samplesPtr->GetSamplingPeriod();

	iqt::CSignalBlocker blocker1(TimeBeginSB);
	iqt::CSignalBlocker blocker2(TimeEndSB);

	double page = TimeEndSB->value() - TimeBeginSB->value();

	double shift = istd::Min(maxTimeSpan - istd::Max(TimeBeginSB->value(), TimeEndSB->value()), page * 0.5);

	if (shift < I_BIG_EPSILON){
		return;
	}

	TimeBeginSB->setValue(TimeBeginSB->value() + shift);
	TimeEndSB->setValue(TimeEndSB->value() + shift);

	UpdateEditor();
}


// public methods of embedded class DiagramWidget

CSamplesSequenceViewComp::DiagramWidget::DiagramWidget(QWidget* parentWidgetPtr, CSamplesSequenceViewComp* parentPtr)
:	QWidget(parentWidgetPtr), m_parent(*parentPtr)
{
	I_ASSERT(parentPtr != NULL);
}


// protected methods of embedded class DiagramWidget

// reimplemented (QWidget)

void CSamplesSequenceViewComp::DiagramWidget::paintEvent(QPaintEvent* /*event*/)
{
	I_ASSERT(m_parent.IsGuiCreated());

	const imeas::ISamplesSequence* samplesPtr = m_parent.GetObjectPtr();
	if (samplesPtr != NULL){
		int samplesCount = samplesPtr->GetTimeSamplesCount();
		double samplingPeriod = samplesPtr->GetSamplingPeriod();
		if ((samplesCount <= 0) || (samplingPeriod < I_BIG_EPSILON)){
			return;
		}

		istd::CRange timeRange = m_parent.TimeSpanCB->isChecked()?
					istd::CRange(m_parent.TimeBeginSB->value(), m_parent.TimeEndSB->value()):
					istd::CRange(0, samplingPeriod * samplesCount);

		int firstChannel = 0;
		int lastChannel = samplesPtr->GetChannelsCount() - 1;

		int selectedChannel = m_parent.ChannelSelectorCB->currentIndex() - 1;
		if (selectedChannel >= 0){
			I_ASSERT(selectedChannel < samplesPtr->GetChannelsCount());

			firstChannel = selectedChannel;
			lastChannel = selectedChannel;
		}

		istd::CRange valueRange;
		if (m_parent.ScaleCB->isChecked()){
			valueRange = istd::CRange(m_parent.ValueMinSB->value(), m_parent.ValueMaxSB->value());
		}
		else{
			int firstSample = istd::Max(0, int(floor(timeRange.GetMinValue() / samplingPeriod)));
			int lastSample = istd::Min(int(ceil(timeRange.GetMaxValue() / samplingPeriod)), samplesCount - 1);
			for (int channelIndex = firstChannel; channelIndex <= lastChannel; ++channelIndex){
				for (int i = firstSample; i <= lastSample; ++i){
					double sample = samplesPtr->GetSample(i, channelIndex);

					valueRange.Unite(sample);
				}
			}
		}

		if (!timeRange.IsValid() || !valueRange.IsValid() || (lastChannel < firstChannel)){
			return;
		}

		QPainter painter(this);

		painter.setRenderHint(QPainter::Antialiasing);

		int rectWidth = width();
		int rectHeight = height();

		QPolygonF polygon(rectWidth * 2);

		int index2End = rectWidth * 2 - 1;

		painter.setPen(Qt::NoPen);

		for (int channelIndex = lastChannel; channelIndex >= firstChannel; --channelIndex){
			int lastSampleEnd = 0;
			for (int x = 0; x < rectWidth; ++x){
				double time = timeRange.GetValueFromAlpha(double(x) / rectWidth);
				int sampleEndIndex = istd::Min(int(floor(time / samplingPeriod)), samplesCount - 1);

				istd::CRange sampleRange = istd::CRange::GetInvalid();

				for (int sampleIndex = lastSampleEnd; sampleIndex <= sampleEndIndex; ++sampleIndex){
					double sample = samplesPtr->GetSample(sampleIndex);
					sampleRange.Unite(sample);
				}

				lastSampleEnd = sampleEndIndex;

				double minValue = (1 - valueRange.GetAlphaFromValue(sampleRange.GetMinValue())) * rectHeight + 0.5;
				double maxValue = (1 - valueRange.GetAlphaFromValue(sampleRange.GetMaxValue())) * rectHeight - 0.5;

				polygon[x].setX(x);
				polygon[x].setY(maxValue);

				polygon[index2End - x].setX(x);
				polygon[index2End - x].setY(minValue);
			}

			painter.setBrush(QBrush(Qt::GlobalColor(Qt::cyan + channelIndex)));
			painter.drawPolygon(polygon);
		}
	}
}


} // namespace iqtmeas


