#include "iqwt/CProgressHistoryGuiComp.h"


// Qt includes
#include <QCoreApplication>
#include <QVBoxLayout>

// Qwt includes
#include "qwt_plot_layout.h"
#include "qwt_plot_grid.h"


namespace iqwt
{


CProgressHistoryGuiComp::CProgressHistoryGuiComp()
:	m_plotPtr(NULL),
	m_nowMarker(NULL),
	m_activeCurveColor(Qt::blue),
	m_inactiveCurveColor(Qt::gray),
	m_currentCurve(*new QwtPlotCurve),
	m_isTrainActive(false)
{
}


// reimplemented (sprw::IActivityConsumer)

int CProgressHistoryGuiComp::BeginProgressSession(
			const std::string& /*progressId*/,
			const istd::CString& description,
			bool isCancelable)
{
	if (m_isTrainActive){
		return -1;
	}

	m_isTrainActive = true;

	m_axisYData.clear();
	m_currentCurve.setData(&m_axisXData[0], &m_axisXData[0], 0);
    m_currentCurve.setPen(QPen(m_activeCurveColor, 2));

	if (IsGuiCreated()){
		CancelButton->setVisible(isCancelable);
		CancelButton->setChecked(false);
		CancelButton->setEnabled(true);

		if (!description.IsEmpty()){
			DescriptionLabel->setText(iqt::GetQString(description));
			DescriptionLabel->setVisible(true);
			DescriptionLabel->setEnabled(true);
		}
		else{
			DescriptionLabel->setVisible(false);
		}
	}

	return 0;
}


void CProgressHistoryGuiComp::EndProgressSession(int /*sessionId*/)
{
    m_currentCurve.setPen(QPen(m_inactiveCurveColor, 1));

	CancelButton->setEnabled(false);
	CancelButton->setChecked(false);
	DescriptionLabel->setEnabled(false);

	m_isTrainActive = false;
}


void CProgressHistoryGuiComp::OnProgress(int /*sessionId*/, double currentProgress)
{
	m_axisYData.insert(m_axisYData.begin(), currentProgress * 100);
	while (m_axisYData.size() > m_axisXData.size()){
		m_axisYData.pop_back();
	}

	I_ASSERT(m_axisYData.size() <= m_axisXData.size());
	m_currentCurve.setData(&m_axisXData[0], &m_axisYData[0], int(m_axisYData.size()));

	if (m_plotPtr != NULL){
		emit m_plotPtr->update();
		QCoreApplication::processEvents();
	}
}


bool CProgressHistoryGuiComp::IsCanceled(int /*sessionId*/) const
{
	return IsGuiCreated() && CancelButton->isChecked();
}


// protected methods

// reimplemented (iqtgui::CGuiComponentBase)

void CProgressHistoryGuiComp::OnGuiCreated()
{
	BaseClass::OnGuiCreated();

	QWidget* widgetPtr = GetWidget();
	if (widgetPtr == NULL){
		return;
	}
	m_plotPtr = new QwtPlot(DiagramFrame);
	m_plotPtr->setCanvasLineWidth(1);
	m_plotPtr->setAxisScale(QwtPlot::yLeft, 0, 100);
	m_plotPtr->setAxisScale(QwtPlot::xBottom, *m_historyStepsCountAttrPtr, 0);
	m_plotPtr->plotLayout()->setAlignCanvasToScales(true);
	m_plotPtr->setAutoReplot(true);
	m_plotPtr->setEnabled(false);

	// setup the plot grid:
	QwtPlotGrid* gridPtr = new QwtPlotGrid();
	QPen gridPen(QBrush(Qt::gray), 0, Qt::DashLine);
	gridPtr->setMajPen(gridPen);
	gridPtr->attach(m_plotPtr);

	m_nowMarker = new QwtPlotMarker();
	m_nowMarker->setValue(0.0, 0.0);
	m_nowMarker->setLineStyle(QwtPlotMarker::VLine);
	m_nowMarker->setLabelAlignment(Qt::AlignRight | Qt::AlignBottom);
	m_nowMarker->setLinePen(QPen(Qt::darkMagenta, 2, Qt::DotLine));
	m_nowMarker->attach(m_plotPtr);

	// setup current plot curve:
    m_currentCurve.setPen(QPen(m_inactiveCurveColor, 1));
	m_currentCurve.setRenderHint(QwtPlotItem::RenderAntialiased);
	m_currentCurve.setTitle(tr("Neuron activities"));
	m_currentCurve.attach(m_plotPtr);

	// add plot view to frame layout
	QLayout* layoutPtr = DiagramFrame->layout();
	if (layoutPtr != NULL){
		layoutPtr->addWidget(m_plotPtr);
	}

	CancelButton->setEnabled(false);
}


void CProgressHistoryGuiComp::OnGuiDestroyed()
{
	m_nowMarker = NULL;

	BaseClass::OnGuiDestroyed();
}


// reimplemented (icomp::IComponent)

void CProgressHistoryGuiComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	m_axisXData.clear();

	int stepsCount = *m_historyStepsCountAttrPtr;
	for (int i = 0; i <= stepsCount; ++i){
		m_axisXData.push_back(i);
	}
}


} // namespace iqwt


