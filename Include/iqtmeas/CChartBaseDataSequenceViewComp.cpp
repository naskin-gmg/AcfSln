#include <iqtmeas/CChartBaseDataSequenceViewComp.h>


// Qt includes
#if QT_VERSION >= 0x050800
#include <QtCharts/QChartView>
#include <QtCharts/QDateTimeAxis>
#include <QtCharts/QValueAxis>
#endif

// ACF includes
#include <imeas/CSamplesInfo.h>


namespace iqtmeas
{


// public methods

CChartBaseDataSequenceViewComp::CChartBaseDataSequenceViewComp()
{
}


// protected methods

// reimplemenented (iqtgui::TGuiObserverWrap)

void CChartBaseDataSequenceViewComp::UpdateGui(const istd::IChangeable::ChangeSet& /*changeSet*/)
{
#if QT_VERSION >= 0x050800
	Q_ASSERT(IsGuiCreated());

	m_chart.removeAllSeries();

	imeas::IDataSequence* objectPtr = GetObservedObject();
	if (objectPtr != NULL){
		int samplesCount = objectPtr->GetSamplesCount();

		istd::CRange timeRange;
		double timeInterval = 0.0;
		const imeas::CSamplesInfo* infoPtr = dynamic_cast<const imeas::CSamplesInfo*>(objectPtr->GetSequenceInfo());
		if (infoPtr != NULL){
			timeRange = infoPtr->GetLogicalSamplesRange();

			if (samplesCount > 0){
				timeInterval = timeRange.GetLength() / double(samplesCount);
			}
		}

		int channelsCount = objectPtr->GetChannelsCount();
		for (int channelIndex = 0; channelIndex < channelsCount; ++channelIndex){
			QtCharts::QLineSeries* series = new QtCharts::QLineSeries();
			quint64 timeOffset = 0;
			// Prepare conversion of the sample index to the absolute time:
			if (timeRange.IsValid() && timeInterval > 0){
				Q_ASSERT(infoPtr != NULL);

				QDateTime startTime = infoPtr->GetSamplingStartTime();
				if (startTime.isValid()){
					timeOffset = startTime.toMSecsSinceEpoch();
				}
			}

			for (int sampleIndex = 0; sampleIndex < samplesCount; ++sampleIndex){
				quint64 xValue = (timeInterval > 0) ? timeOffset + timeInterval * sampleIndex * 1000 : sampleIndex;

				series->append(xValue, objectPtr->GetSample(sampleIndex, channelIndex));
			}

			m_chart.addSeries(series);

			if (m_chart.axisX() == NULL){
				QtCharts::QAbstractAxis* axisXPtr = NULL;
				if (infoPtr != NULL){
					QtCharts::QDateTimeAxis* axisImplPtr = new QtCharts::QDateTimeAxis;
					axisImplPtr->setTickCount(10);
					axisImplPtr->setTitleText(tr("Time"));

					axisXPtr = axisImplPtr;
				}
				else{
					QtCharts::QValueAxis* axisImplPtr = new QtCharts::QValueAxis;
					axisImplPtr->setTickCount(10);
					axisImplPtr->setTitleText(tr("Index"));

					axisXPtr = axisImplPtr;
				}

				m_chart.setAxisX(axisXPtr, series);

				QtCharts::QValueAxis* axisYPtr = new QtCharts::QValueAxis;
				axisYPtr->setTickCount(10);
				axisYPtr->setTitleText(tr("Value"));
				m_chart.setAxisY(axisYPtr, series);
			}
		}
	}

#endif // #if QT_VERSION >= 0x050800
}


void CChartBaseDataSequenceViewComp::OnGuiModelAttached()
{
	BaseClass::OnGuiModelAttached();
}


// reimplemented (iqtgui::CGuiComponentBase)

void CChartBaseDataSequenceViewComp::OnGuiCreated()
{
	BaseClass::OnGuiCreated();

#if QT_VERSION >= 0x050800
	QLayout* chartLayoutPtr = ChartFrame->layout();
	Q_ASSERT(chartLayoutPtr != NULL);

	QtCharts::QChartView* chartViewPtr = new QtCharts::QChartView(ChartFrame);
	chartViewPtr->setRenderHints(QPainter::Antialiasing);

	chartViewPtr->setChart(&m_chart);
	m_chart.setTheme(QtCharts::QChart::ChartThemeDark);

	chartLayoutPtr->addWidget(chartViewPtr);
#endif
}


void CChartBaseDataSequenceViewComp::OnGuiDestroyed()
{
#if QT_VERSION >= 0x050800
	QLayout* chartLayoutPtr = ChartFrame->layout();
	Q_ASSERT(chartLayoutPtr != NULL);

	QWidgetItem* itemPtr = dynamic_cast<QWidgetItem*>(chartLayoutPtr->takeAt(0));
	Q_ASSERT(itemPtr != NULL);

	delete itemPtr;
#endif

	BaseClass::OnGuiDestroyed();
}


} // namespace iqtmeas


