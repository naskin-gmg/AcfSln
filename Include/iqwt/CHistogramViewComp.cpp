#include "iqwt/CHistogramViewComp.h"


namespace iqwt
{


// reimplemented (imod::IModelEditor)

void CHistogramViewComp::UpdateModel() const
{
	I_ASSERT(IsGuiCreated() && (GetObjectPtr() != NULL));
}


void CHistogramViewComp::UpdateEditor(int /*updateFlags*/)
{
	I_ASSERT(IsGuiCreated());

	int channelsCount = 0;

	imeas::IDataSequence* objectPtr = GetObjectPtr();
	if (objectPtr != NULL){
		channelsCount = objectPtr->GetChannelsCount();

		if (objectPtr->GetChannelsCount() != m_channelCurves.GetCount()){
			ClearPlot();

			ChannelCombo->clear();

			for (int channelIndex = 0; channelIndex < channelsCount; channelIndex++){
				QwtPlotCurve* curvePtr = new QwtPlotCurve();
				curvePtr->setPen(QColor(Qt::darkBlue));

				QLinearGradient brushGradient(QPointF(0.0, 0.0), QPoint(0.0, 1.0));
				brushGradient.setColorAt(0, Qt::cyan);
				brushGradient.setColorAt(1, Qt::blue);
				brushGradient.setCoordinateMode(QGradient::StretchToDeviceMode);

				curvePtr->setBrush(brushGradient);
			
				curvePtr->attach(m_plotPtr.GetPtr());
				curvePtr->setRenderHint(QwtPlotItem::RenderAntialiased);
				m_channelCurves.PushBack(curvePtr);

				ChannelCombo->addItem(QString("Channel %1").arg(channelIndex + 1));
			}
		}
		
		for (int channelIndex = 0; channelIndex < channelsCount; channelIndex++){
			int samplesCount = objectPtr->GetSamplesCount() / channelsCount;

			QVector<double> xData(samplesCount);
			QVector<double> yData(samplesCount);

			for (int sampleIndex = 0; sampleIndex < samplesCount; sampleIndex++){
				xData[sampleIndex] = sampleIndex;
				yData[sampleIndex] = objectPtr->GetSample(sampleIndex, channelIndex);
			}

			QwtPlotCurve* curvePtr = m_channelCurves.GetAt(channelIndex);
			I_ASSERT(curvePtr != NULL);

			curvePtr->setData(xData, yData);
			m_plotPtr->setAxisScale(QwtPlot::xBottom, 0, samplesCount, samplesCount / 5); 
		    m_plotPtr->setAxisMaxMinor(QwtPlot::xBottom, samplesCount / 10);

			if (channelIndex != ChannelCombo->currentIndex()){
				curvePtr->setVisible(false);
			}
		}
	}
	else{
		ClearPlot();
		
		ChannelCombo->clear();
	}

	if (IsGuiCreated()){
		ChannelCombo->setVisible(channelsCount > 1);
	}
}


// protected methods

// reimplemented (iqtgui::CGuiComponentBase)

void CHistogramViewComp::OnGuiCreated()
{
	m_plotPtr.SetPtr(new QwtPlot(PlotFrame));
	m_plotPtr->setCanvasLineWidth(0);
	m_plotPtr->setAxisFont(QwtPlot::xBottom, qApp->font());
	m_plotPtr->setAxisFont(QwtPlot::yLeft, qApp->font());
	m_plotPtr->setAutoReplot(true);

	QwtPlotGrid* plotGridPtr = new QwtPlotGrid;
	plotGridPtr->enableXMin(true);
	plotGridPtr->enableYMin(true);
	plotGridPtr->setMajPen(QPen(Qt::black, 0, Qt::DotLine));
	plotGridPtr->setMinPen(QPen(Qt::gray, 0 , Qt::DotLine));
	plotGridPtr->attach(m_plotPtr.GetPtr());

	QLayout* layoutPtr = PlotFrame->layout();
	if (layoutPtr == NULL){
		layoutPtr = new QVBoxLayout(PlotFrame);
		layoutPtr->setMargin(0);
	}

	layoutPtr->addWidget(m_plotPtr.GetPtr());

	BaseClass::OnGuiCreated();
}


void CHistogramViewComp::OnGuiDestroyed()
{
	ClearPlot();

	m_plotPtr.Reset();

	BaseClass::OnGuiDestroyed();
}


// private slots

void CHistogramViewComp::on_ChannelCombo_currentIndexChanged(int index)
{
	for (int channelIndex = 0; channelIndex < m_channelCurves.GetCount(); channelIndex++){
		m_channelCurves.GetAt(channelIndex)->setVisible(channelIndex == index);
	}
}


// private methods

void CHistogramViewComp::ClearPlot()
{
	if (IsGuiCreated()){
		for (int curveIndex = 0; curveIndex < m_channelCurves.GetCount(); curveIndex++){
			m_channelCurves.GetAt(curveIndex)->detach();
		}
		
		m_channelCurves.Reset();
		m_plotPtr->clear();
	}
}


} // namespace iqwt


