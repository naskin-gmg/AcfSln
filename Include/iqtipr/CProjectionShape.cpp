#include "iqtipr/CProjectionShape.h"


// Qt includes
#include <QStyleOptionGraphicsItem>
#include <QPainter>

// ACF-Solutions includes
#include "imeas/IDataSequence.h"


namespace iqtipr
{


// public methods

CProjectionShape::CProjectionShape()
{
	setPen(QPen(QBrush(Qt::blue), 0));
	setZValue(1);
}


// reimplemented (imod::IObserver)

void CProjectionShape::AfterUpdate(imod::IModel* modelPtr, int /*updateFlags*/, istd::IPolymorphic* /*updateParamsPtr*/)
{
	const i2d::CLine2d* linePtr = dynamic_cast<const i2d::CLine2d*>(modelPtr);
	if (linePtr != NULL){
		m_projectionLine = *linePtr;

		return;
	}

	resetTransform();

	const imeas::IDataSequence* projectionPtr = dynamic_cast<const imeas::IDataSequence*>(modelPtr);
	if (projectionPtr != NULL){
		QPainterPath path;

		QTransform transform;

		const i2d::CVector2d center = m_projectionLine.GetCenter();

		int samplesCount = projectionPtr->GetSamplesCount();
		if (		(samplesCount > 0) &&
					(projectionPtr->GetChannelsCount() > 0) &&
					(m_projectionLine.GetDiffVector().GetLength() > 0)){
			transform.translate(m_projectionLine.GetPoint1().GetX(), m_projectionLine.GetPoint1().GetY());
			transform.rotate(imath::GetDegreeFromRadian(m_projectionLine.GetDiffVector().GetAngle()));
			double scaleFactor = m_projectionLine.GetDiffVector().GetLength() / samplesCount;
			transform.scale(scaleFactor, scaleFactor);
			transform.translate(0, -128);

			setTransform(transform);

			double prevSample = projectionPtr->GetSample(0);
			for(int x = 1; x < samplesCount - 1; x++){
				double sample = projectionPtr->GetSample(x);

				path.moveTo(x - 1, prevSample * 100);
				path.lineTo(x, 255 - sample * 100);

				prevSample = sample;
			}
		}

		setPath(path);

		return;
	}
}


} // namespace iqtipr


