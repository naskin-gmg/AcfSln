#include "iqwt/CProgressHistoryGuiComp.h"


// Qt includes
#include <QCoreApplication>

// Qwt includes
#include "qwt_plot_layout.h"
#include "qwt_plot_grid.h"


namespace iqwt
{


CProgressHistoryGuiComp::CProgressHistoryGuiComp()
:	m_currentId(0),
	m_cancelsCount(0)
{
}


// reimplemented (iproc::IProgressManager)

int CProgressHistoryGuiComp::BeginProgressSession(
			const std::string& /*progressId*/,
			const istd::CString& description,
			bool isCancelable)
{
	if (m_idToSessionMap.empty()){
		m_currentId = 0;
	}

	int sessionId = m_currentId++;

	Session& session = m_idToSessionMap[sessionId];

	// setup current plot curve:
	session.axisY.clear();
	session.curve.setRenderHint(QwtPlotItem::RenderAntialiased);
	session.curve.setTitle(iqt::GetQString(description));
	session.curve.attach(m_plotPtr.GetPtr());
	session.curve.setData(&m_axisXData[0], &m_axisXData[0], 0);
    session.curve.setPen(QPen(Qt::GlobalColor(Qt::cyan + sessionId), 2));
	session.isCancelable = isCancelable;
	session.description = description;

	if (session.isCancelable){
		++m_cancelsCount;
	}

	UpdateState();

	return sessionId;
}


void CProgressHistoryGuiComp::EndProgressSession(int sessionId)
{
	IdToSessionMap::iterator foundIter = m_idToSessionMap.find(sessionId);
	if (foundIter == m_idToSessionMap.end()){
		return;
	}

	Session& session = foundIter->second;

	session.curve.setPen(QPen(Qt::GlobalColor(Qt::cyan + sessionId), 0.5));

	if (session.isCancelable){
		--m_cancelsCount;
		I_ASSERT(m_cancelsCount >= 0);	// number of all cancelable sessions cannot be negative
	}

	session.curve.detach();

	m_idToSessionMap.erase(foundIter);

	UpdateState();
}


void CProgressHistoryGuiComp::OnProgress(int sessionId, double currentProgress)
{
	IdToSessionMap::iterator foundIter = m_idToSessionMap.find(sessionId);
	if (foundIter == m_idToSessionMap.end()){
		return;
	}

	Session& session = foundIter->second;

	session.axisY.insert(session.axisY.begin(), currentProgress * 100);
	while (session.axisY.size() > m_axisXData.size()){
		session.axisY.pop_back();
	}

	I_ASSERT(session.axisY.size() <= m_axisXData.size());
	session.curve.setData(&m_axisXData[0], &session.axisY[0], int(session.axisY.size()));

	if (m_plotPtr.IsValid()){
		emit m_plotPtr->update();

		QCoreApplication::processEvents();
	}
}


bool CProgressHistoryGuiComp::IsCanceled(int /*sessionId*/) const
{
	return IsGuiCreated() && CancelButton->isChecked();
}


// protected methods

void CProgressHistoryGuiComp::UpdateState()
{
	if (!IsGuiCreated()){
		return;
	}

	if (m_cancelsCount > 0){
		CancelButton->setEnabled(true);
	}
	else{
		CancelButton->setEnabled(false);
		CancelButton->setChecked(false);
	}


	int sessionsCount = int(m_idToSessionMap.size());
	if (sessionsCount == 0){
		DescriptionLabel->setText(tr("Finished"));
	}
	else if (sessionsCount == 1){
		DescriptionLabel->setText(iqt::GetQString(m_idToSessionMap.begin()->second.description));
	}
	else{
		DescriptionLabel->setText(tr("%1 Sessions").arg(sessionsCount));
	}
}


// reimplemented (iqtgui::CGuiComponentBase)

void CProgressHistoryGuiComp::OnGuiCreated()
{
	BaseClass::OnGuiCreated();

	QWidget* widgetPtr = GetWidget();
	if (widgetPtr == NULL){
		return;
	}
	m_plotPtr.SetPtr(new QwtPlot(DiagramFrame));
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
	gridPtr->attach(m_plotPtr.GetPtr());

	m_nowMarker.SetPtr(new QwtPlotMarker());
	m_nowMarker->setValue(0.0, 0.0);
	m_nowMarker->setLineStyle(QwtPlotMarker::VLine);
	m_nowMarker->setLabelAlignment(Qt::AlignRight | Qt::AlignBottom);
	m_nowMarker->setLinePen(QPen(Qt::darkMagenta, 2, Qt::DotLine));
	m_nowMarker->attach(m_plotPtr.GetPtr());

	// add plot view to frame layout
	QLayout* layoutPtr = DiagramFrame->layout();
	if (layoutPtr != NULL){
		layoutPtr->addWidget(m_plotPtr.GetPtr());
	}

	UpdateState();
}


void CProgressHistoryGuiComp::OnGuiDestroyed()
{
	m_nowMarker->detach();
	m_nowMarker.Reset();

	m_plotPtr.Reset();

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


