#include <iqtsig/CSampleContainerView.h>


// Qt includes
#include <QtGui/QPainter>


namespace iqtsig
{


CSampleContainerView::CSampleContainerView(QWidget* parentPtr)
:	BaseClass(parentPtr),
	m_displayedRange(0, 100)
{
}


void CSampleContainerView::SetDisplayedRange(const istd::CRange& range)
{
	m_displayedRange = range;
}


// reimplemented (imod::CSingleModelObserverBase)

void CSampleContainerView::OnUpdate(const istd::IChangeable::ChangeSet& changeSet)
{
	BaseClass2::OnUpdate(changeSet);

	emit repaint();
}


// reimplemented (QWidget)

void CSampleContainerView::paintEvent(QPaintEvent* /*event*/)
{
	const imeas::IDataSequence* samplesPtr = GetObservedObject();
	if (samplesPtr != NULL){
		QPainter painter(this);

		painter.setRenderHint(QPainter::Antialiasing);

		int rectWidth = width();
		int rectHeight = height();

		int samplesCount = samplesPtr->GetSamplesCount();
		QPolygonF polyline(samplesCount);

		for (int i = 0; i < samplesCount; ++i){
			polyline[i].setX(double(i * rectWidth) / samplesCount);
			double sample = samplesPtr->GetSample(i);
			polyline[i].setY((1 - m_displayedRange.GetAlphaFromValue(sample)) * rectHeight);
		}

		painter.drawPolyline(polyline);
	}
}


} // namespace iqtsig


