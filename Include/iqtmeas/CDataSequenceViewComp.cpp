// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#include <iqtmeas/CDataSequenceViewComp.h>


// Qt includes
#include <QtGui/QPainter>

// ACF includes
#include <iqt/CSignalBlocker.h>

#include <imeas/CSamplesInfo.h>


namespace iqtmeas
{


// public methods

CDataSequenceViewComp::CDataSequenceViewComp()
:	m_lastChannelsCount(0),
	m_channelsSelectorModel(1, 1)
{
	m_channelsSelectorModel.setItem(0, 0, new QStandardItem(tr("All channels")));
}


// reimplemenented (iqtgui::TGuiObserverWrap)

void CDataSequenceViewComp::UpdateGui(const istd::IChangeable::ChangeSet& /*changeSet*/)
{
	Q_ASSERT(IsGuiCreated());

	int channelsCount = 0;

	imeas::IDataSequence* objectPtr = GetObservedObject();
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

void CDataSequenceViewComp::OnGuiCreated()
{
	BaseClass::OnGuiCreated();

	DiagramWidget* newDiagramPtr = new DiagramWidget(MainFrame, this);
	QLayout* layoutPtr = MainFrame->layout();
	if (layoutPtr != NULL){
		layoutPtr->addWidget(newDiagramPtr);
	}

	ScaleFrame->setVisible(*m_showScalePanelAttrPtr);
	ScaleControlFrame->setVisible(*m_showScalePanelAttrPtr);
	ChannelsFrame->setVisible(*m_showChannelPanelAttrPtr);
	TimeSpanFrame->setVisible(*m_showTimeSpanPanelAttrPtr);
	BottomFrame->setVisible(*m_showScalePanelAttrPtr || *m_showChannelPanelAttrPtr || *m_showTimeSpanPanelAttrPtr);
	ValueMinSB->setValue(*m_minValueAttrPtr);
	ValueMaxSB->setValue(*m_maxValueAttrPtr);
	ScaleCB->setChecked(!*m_isScaledValueAttrPtr);
	TimeBeginSB->setValue(*m_minTimeSpanAttrPtr);
	TimeEndSB->setValue(*m_maxTimeSpanAttrPtr);
	TimeSpanCB->setChecked(!*m_isScaledTimeSpanAttrPtr);

	ChannelSelectorCB->setModel(&m_channelsSelectorModel);
}


// protected signals

void CDataSequenceViewComp::on_TimeSpanCB_toggled(bool /*state*/)
{
	UpdateGui(istd::IChangeable::GetAllChanges());
}


void CDataSequenceViewComp::on_ScaleCB_toggled(bool /*state*/)
{
	UpdateGui(istd::IChangeable::GetAllChanges());
}


void CDataSequenceViewComp::on_TimeBeginSB_valueChanged(double /*value*/)
{
	UpdateGui(istd::IChangeable::GetAllChanges());
}


void CDataSequenceViewComp::on_TimeEndSB_valueChanged(double /*value*/)
{
	UpdateGui(istd::IChangeable::GetAllChanges());
}


void CDataSequenceViewComp::on_ValueMinSB_valueChanged(double /*value*/)
{
	UpdateGui(istd::IChangeable::GetAllChanges());
}


void CDataSequenceViewComp::on_ValueMaxSB_valueChanged(double /*value*/)
{
	UpdateGui(istd::IChangeable::GetAllChanges());
}


void CDataSequenceViewComp::on_ChannelSelectorCB_currentIndexChanged(int /*index*/)
{
	UpdateGui(istd::IChangeable::GetAllChanges());
}


void CDataSequenceViewComp::on_ZoomInButton_clicked()
{
	const imeas::IDataSequence* samplesPtr = GetObservedObject();
	if (samplesPtr == NULL){
		return;
	}

	iqt::CSignalBlocker blocker1(TimeBeginSB);
	iqt::CSignalBlocker blocker2(TimeEndSB);

	double center = (TimeBeginSB->value() + TimeEndSB->value()) * 0.5;
	TimeBeginSB->setValue((TimeBeginSB->value() + center) * 0.5);
	TimeEndSB->setValue((TimeEndSB->value() + center) * 0.5);

	UpdateGui(istd::IChangeable::GetAllChanges());
}


void CDataSequenceViewComp::on_ZoomOutButton_clicked()
{
	const imeas::IDataSequence* samplesPtr = GetObservedObject();
	if (samplesPtr == NULL){
		return;
	}

	double maxTimeSpan = samplesPtr->GetSamplesCount();
	const imeas::CSamplesInfo* infoPtr = dynamic_cast<const imeas::CSamplesInfo*>(samplesPtr->GetSequenceInfo());
	if (infoPtr != NULL){
		const istd::CRange& logicalRange = infoPtr->GetLogicalSamplesRange();
		if (logicalRange.IsValid()){
			maxTimeSpan = logicalRange.GetLength();
		}
	}

	iqt::CSignalBlocker blocker1(TimeBeginSB);
	iqt::CSignalBlocker blocker2(TimeEndSB);

	double center = (TimeBeginSB->value() + TimeEndSB->value()) * 0.5;
	TimeBeginSB->setValue(qMax(0.0, TimeBeginSB->value() * 2 - center));
	TimeEndSB->setValue(qMin(TimeEndSB->value() * 2 - center, maxTimeSpan));

	UpdateGui(istd::IChangeable::GetAllChanges());
}


void CDataSequenceViewComp::on_PrevButton_clicked()
{
	const imeas::IDataSequence* samplesPtr = GetObservedObject();
	if (samplesPtr == NULL){
		return;
	}

	iqt::CSignalBlocker blocker1(TimeBeginSB);
	iqt::CSignalBlocker blocker2(TimeEndSB);

	double page = TimeEndSB->value() - TimeBeginSB->value();

	double shift = qMin(qMin(TimeBeginSB->value(), TimeEndSB->value()), page * 0.5);

	if (shift < I_BIG_EPSILON){
		return;
	}

	TimeBeginSB->setValue(TimeBeginSB->value() - shift);
	TimeEndSB->setValue(TimeEndSB->value() - shift);

	UpdateGui(istd::IChangeable::GetAllChanges());
}


void CDataSequenceViewComp::on_NextButton_clicked()
{
	const imeas::IDataSequence* samplesPtr = GetObservedObject();
	if (samplesPtr == NULL){
		return;
	}

	double maxTimeSpan = samplesPtr->GetSamplesCount();
	const imeas::CSamplesInfo* infoPtr = dynamic_cast<const imeas::CSamplesInfo*>(samplesPtr->GetSequenceInfo());
	if (infoPtr != NULL){
		const istd::CRange& logicalRange = infoPtr->GetLogicalSamplesRange();
		if (logicalRange.IsValid()){
			maxTimeSpan = logicalRange.GetLength();
		}
	}

	iqt::CSignalBlocker blocker1(TimeBeginSB);
	iqt::CSignalBlocker blocker2(TimeEndSB);

	double page = TimeEndSB->value() - TimeBeginSB->value();

	double shift = qMin(maxTimeSpan - qMax(TimeBeginSB->value(), TimeEndSB->value()), page * 0.5);

	if (shift < I_BIG_EPSILON){
		return;
	}

	TimeBeginSB->setValue(TimeBeginSB->value() + shift);
	TimeEndSB->setValue(TimeEndSB->value() + shift);

	UpdateGui(istd::IChangeable::GetAllChanges());
}


// public methods of embedded class DiagramWidget

CDataSequenceViewComp::DiagramWidget::DiagramWidget(QWidget* parentWidgetPtr, CDataSequenceViewComp* parentPtr)
:	QWidget(parentWidgetPtr), m_parent(*parentPtr)
{
	Q_ASSERT(parentPtr != NULL);
}


// protected methods of embedded class DiagramWidget

// reimplemented (QWidget)

void CDataSequenceViewComp::DiagramWidget::paintEvent(QPaintEvent* /*event*/)
{
	Q_ASSERT(m_parent.IsGuiCreated());

	const imeas::IDataSequence* samplesPtr = m_parent.GetObservedObject();
	if (samplesPtr != NULL){
		int samplesCount = samplesPtr->GetSamplesCount();

		double maxTimeSpan = samplesPtr->GetSamplesCount();
		const imeas::CSamplesInfo* infoPtr = dynamic_cast<const imeas::CSamplesInfo*>(samplesPtr->GetSequenceInfo());
		if (infoPtr != NULL){
			const istd::CRange& logicalRange = infoPtr->GetLogicalSamplesRange();
			if (logicalRange.IsValid()){
				maxTimeSpan = logicalRange.GetLength();
			}
		}

		if ((samplesCount <= 0) || (maxTimeSpan < I_BIG_EPSILON)){
			return;
		}

		istd::CRange timeRange = m_parent.TimeSpanCB->isChecked()?
					istd::CRange(m_parent.TimeBeginSB->value(), m_parent.TimeEndSB->value()):
					istd::CRange(0, maxTimeSpan);

		int firstChannel = 0;
		int lastChannel = samplesPtr->GetChannelsCount() - 1;

		int selectedChannel = m_parent.ChannelSelectorCB->currentIndex() - 1;
		if (selectedChannel >= 0){
			Q_ASSERT(selectedChannel < samplesPtr->GetChannelsCount());

			firstChannel = selectedChannel;
			lastChannel = selectedChannel;
		}

		istd::CRange valueRange;
		if (m_parent.ScaleCB->isChecked()){
			valueRange = istd::CRange(m_parent.ValueMinSB->value(), m_parent.ValueMaxSB->value());
		}
		else{
			int firstSample = qMax(0, qFloor(timeRange.GetMinValue() * samplesCount / maxTimeSpan));
			int lastSample = qMin(qCeil(timeRange.GetMaxValue() * samplesCount / maxTimeSpan), samplesCount - 1);
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
				int sampleEndIndex = qMin(qFloor(time * samplesCount / maxTimeSpan), samplesCount - 1);

				istd::CRange sampleRange = istd::CRange::GetInvalid();

				for (int sampleIndex = lastSampleEnd; sampleIndex <= sampleEndIndex; ++sampleIndex){
					double sample = samplesPtr->GetSample(sampleIndex, channelIndex);
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


