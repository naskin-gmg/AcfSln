#include "iqwt/CHistogramViewComp.h"


// Qwt includes
#include "qwt_plot_grid.h"
#include "qwt_scale_draw.h"


// ACF-Solutions includes
#include "imeas/IDataStatistics.h"


namespace iqwt
{


// public methods

// reimplemenented (iqtgui::TGuiObserverWrap)

void CHistogramViewComp::UpdateGui(int /*updateFlags*/)
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
				
				curvePtr->setStyle(QwtPlotCurve::Steps);
			
				curvePtr->attach(m_plotPtr.GetPtr());
				m_channelCurves.PushBack(curvePtr);

				ChannelCombo->addItem(QString("Channel %1").arg(channelIndex + 1));
			}
		}
		
		double maxValue = 0.0;

		for (int channelIndex = 0; channelIndex < channelsCount; channelIndex++){
			int samplesCount = objectPtr->GetSamplesCount();
	
			QVector<double> xData(samplesCount);
			QVector<double> yData(samplesCount);

			for (int sampleIndex = 0; sampleIndex < samplesCount; sampleIndex++){
				double sample = objectPtr->GetSample(sampleIndex, channelIndex);

				xData[sampleIndex] = sampleIndex;
				yData[sampleIndex] = sample;

				if (sample > maxValue){
					maxValue = sample;
				}
			}

			QwtPlotCurve* curvePtr = m_channelCurves.GetAt(channelIndex);
			I_ASSERT(curvePtr != NULL);

			maxValue = ::ceil(maxValue * 100) / 100.0;

			curvePtr->setSamples(xData, yData);

			m_plotPtr->setAxisScale(QwtPlot::xBottom, 0.0, samplesCount, 16); 
		    m_plotPtr->setAxisMaxMinor(QwtPlot::xBottom, 4);

			m_plotPtr->setAxisScale(QwtPlot::yLeft, 0.0, maxValue, maxValue / 10); 
		    m_plotPtr->setAxisMaxMinor(QwtPlot::yLeft, 5);

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

	QwtPlotPicker* plotPickerPtr = new HistogramPlotPicker(*this, QwtPlot::xBottom, QwtPlot::yLeft, m_plotPtr->canvas());
	plotPickerPtr->setTrackerMode(QwtPicker::AlwaysOn);
	plotPickerPtr->setEnabled(true);

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

	if( m_dataStatisticsGuiCompPtr.IsValid() && m_dataStatisticsObserverCompPtr.IsValid()){
		m_dataStatisticsGuiCompPtr->CreateGui(StatisticsFrame);

		m_curentChannelStatisticsModel.AttachObserver(m_dataStatisticsObserverCompPtr.GetPtr());
	}
	else{
		StatisticsFrame->setVisible(false);
	}

	BaseClass::OnGuiCreated();
}


void CHistogramViewComp::OnGuiDestroyed()
{
	ClearPlot();

	m_plotPtr.Reset();

	m_curentChannelStatisticsModel.DetachAllObservers();

	if( m_dataStatisticsGuiCompPtr.IsValid()){
		m_dataStatisticsGuiCompPtr->DestroyGui();
	}

	BaseClass::OnGuiDestroyed();
}


// private slots

void CHistogramViewComp::on_ChannelCombo_currentIndexChanged(int index)
{
	for (int channelIndex = 0; channelIndex < m_channelCurves.GetCount(); channelIndex++){
		m_channelCurves.GetAt(channelIndex)->setVisible(channelIndex == index);
	}

	if (m_dataSequenceStatisticsCompPtr.IsValid() && m_dataStatisticsObserverCompPtr.IsValid()){
		int statisticsChannelCount = m_dataSequenceStatisticsCompPtr->GetChannelsCount();
		if (index >= 0 && index < statisticsChannelCount){
			const imod::IModel* channelStatisticsPtr = dynamic_cast<const imod::IModel*>(m_dataSequenceStatisticsCompPtr->GetChannelStatistics(index));
			if (channelStatisticsPtr != NULL){
				m_curentChannelStatisticsModel.SetModelPtr(const_cast<imod::IModel*>(channelStatisticsPtr));
			}
		}
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
	}
}


// public methods of the embedded class HistogramPlotPicker

CHistogramViewComp::HistogramPlotPicker::HistogramPlotPicker(CHistogramViewComp& parent, int xAxis, int yAxis, QwtPlotCanvas* canvasPtr)
	:BaseClass(xAxis, yAxis, canvasPtr),
	m_parent(parent)
{
}


// reimplemented (QwtPlotPicker)

QwtText CHistogramViewComp::HistogramPlotPicker::trackerText(const QPoint& position) const
{
	imeas::IDataSequence* objectPtr = m_parent.GetObjectPtr();
	if (objectPtr == NULL){
		return QwtText();
	}

	if (objectPtr->IsEmpty()){
		return QwtText();
	}

	I_ASSERT(m_parent.IsGuiCreated());

	int sampleIndex = int(invTransform(position).x() + 0.5);

	if (sampleIndex >= objectPtr->GetSamplesCount() / objectPtr->GetChannelsCount()){
		return QwtText();
	}

	int currentCurveIndex = m_parent.ChannelCombo->currentIndex();

	double sample = objectPtr->GetSample(sampleIndex, currentCurveIndex);

	QString text = QString("Index %1: %2 %").arg(sampleIndex).arg(sample * 100, 3, 'f', 2);

	return text;
}


} // namespace iqwt


