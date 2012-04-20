#include "iqtipr/CProjectionShape.h"


// Qt includes
#include <QtGui/QStyleOptionGraphicsItem>
#include <QtGui/QPainter>

// ACF-Solutions includes
#include "imeas/IDataSequence.h"


namespace iqtipr
{


// public methods

CProjectionShape::CProjectionShape()
{
}


// reimplemented (iview::IVisualizable)

void CProjectionShape::Draw(QPainter& drawContext) const
{
	const imeas::IDataSequence* projectionPtr = dynamic_cast<const imeas::IDataSequence*>(GetModelPtr());
	if (projectionPtr != NULL){

		drawContext.save();

		drawContext.setPen(QPen(QBrush(Qt::blue), 0));

		UpdateFigure();

		drawContext.setTransform(m_transform);
		drawContext.drawPath(m_path);

		drawContext.restore();
	}
}


// reimplemented (iview::CShapeBase)

i2d::CRect CProjectionShape::CalcBoundingBox() const
{
	return iqt::GetCRectangle(m_path.boundingRect());
}

	
// reimplemented (imod::IObserver)

void CProjectionShape::AfterUpdate(imod::IModel* modelPtr, int updateFlags, istd::IPolymorphic* updateParamsPtr)
{
	const i2d::CLine2d* linePtr = dynamic_cast<const i2d::CLine2d*>(modelPtr);
	if (linePtr != NULL){
		m_projectionLine = *linePtr;

		return;
	}

	UpdateFigure();

	BaseClass::AfterUpdate(modelPtr, updateFlags, updateParamsPtr);
}


// protected methods

void CProjectionShape::UpdateFigure() const
{
	const imeas::IDataSequence* projectionPtr = dynamic_cast<const imeas::IDataSequence*>(GetModelPtr());
	if (projectionPtr == NULL){
		m_transform.reset();
		return;
	}

	const i2d::CVector2d center = m_projectionLine.GetCenter();

	int samplesCount = projectionPtr->GetSamplesCount();
	if (		(samplesCount > 0) &&
				(projectionPtr->GetChannelsCount() > 0) &&
				(m_projectionLine.GetDiffVector().GetLength() > 0)){
		
		m_transform.translate(m_projectionLine.GetPoint1().GetX(), m_projectionLine.GetPoint1().GetY());
		m_transform.rotate(imath::GetDegreeFromRadian(m_projectionLine.GetDiffVector().GetAngle()));
		
		double scaleFactor = m_projectionLine.GetDiffVector().GetLength() / samplesCount;
		
		m_transform.scale(scaleFactor, scaleFactor);
		m_transform.translate(0, -128);

		double prevSample = projectionPtr->GetSample(0);

		for(int x = 1; x < samplesCount - 1; x++){
			double sample = projectionPtr->GetSample(x);
			
			m_path.moveTo(x - 1, prevSample * 100);
			m_path.lineTo(x, 255 - sample * 100);

			prevSample = sample;
		}
	}
}


} // namespace iqtipr


