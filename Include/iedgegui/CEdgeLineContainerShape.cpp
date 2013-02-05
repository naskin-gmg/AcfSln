#include "iedgegui/CEdgeLineContainerShape.h"


// Qt includes
#include <QtGui/QPainter>

// ACF includes
#include "imod/IModel.h"
#include "iview/IColorSchema.h"
#include "iview/CScreenTransform.h"
#include "iedge/CEdgeLine.h"


namespace iedgegui
{


// reimplemented (iview::IVisualizable)

void CEdgeLineContainerShape::Draw(QPainter& drawContext) const
{
	const iedge::CEdgeLine::Container* containerPtr = dynamic_cast<const iedge::CEdgeLine::Container*>(GetModelPtr());
	if (IsDisplayConnected() && containerPtr != NULL){
		int numLines = containerPtr->GetItemsCount();

		if (numLines > 0){
			double scale = qSqrt(GetViewToScreenTransform().GetDeformMatrix().GetFrobeniusNorm()) * 2;

			const iview::IColorSchema& colorSchema = GetColorSchema();
			const QPen& defaultPen = colorSchema.GetPen(iview::IColorSchema::SP_NORMAL);

			// prepare draw context
			QBrush brush(Qt::green, Qt::SolidPattern);
			brush.setColor(defaultPen.color());
			drawContext.setPen(Qt::NoPen);
			drawContext.setBrush(brush);

			QPolygonF polygon(4);

			for (int lineIndex = 0; lineIndex < numLines; lineIndex++){
				const iedge::CEdgeLine& line = containerPtr->GetAt(lineIndex);

				const i2d::ICalibration2d* calibrationPtr = line.GetCalibration();

				int nodesCount = line.GetNodesCount();

				if (nodesCount > 1){
					int pointIndex = line.IsClosed()? nodesCount - 1: 0;

					// first point position
					const iedge::CEdgeNode& firstNode = line.GetNode(pointIndex);
					i2d::CVector2d prevPos = GetScreenPosition(firstNode.GetPosition(), calibrationPtr);
					double prevWeight = firstNode.GetWeight();

					for (pointIndex = (pointIndex + 1) % nodesCount; pointIndex < nodesCount; pointIndex++){
						const iedge::CEdgeNode& node = line.GetNode(pointIndex);
						i2d::CVector2d pos = GetScreenPosition(node.GetPosition(), calibrationPtr);
						double weight = node.GetWeight();

						i2d::CVector2d dif = pos - prevPos;
						dif.Normalize();
						i2d::CVector2d orthogonal = dif.GetOrthogonal();

						i2d::CVector2d thickVector = orthogonal * qMin(5.0, scale * prevWeight);
						polygon[0] = QPointF(prevPos - thickVector);
						polygon[1] = QPointF(prevPos + thickVector);

						thickVector = orthogonal * qMin(4.0, scale * weight);
						polygon[2] = QPointF(pos + thickVector);
						polygon[3] = QPointF(pos - thickVector);

						drawContext.drawPolygon(polygon);

						prevPos = pos;
						prevWeight = weight;
					}
				}
				else if (nodesCount > 0){
					// only one node exists
					const iedge::CEdgeNode& node = line.GetNode(0);
					i2d::CVector2d pos = GetScreenPosition(node.GetPosition(), calibrationPtr);
					double radius = qMin(4.0, node.GetWeight() * scale);

					polygon[0] = QPointF(pos.GetX() - radius, pos.GetY() - radius);
					polygon[1] = QPointF(pos.GetX() + radius, pos.GetY() - radius);
					polygon[2] = QPointF(pos.GetX() + radius, pos.GetY() + radius);
					polygon[3] = QPointF(pos.GetX() - radius, pos.GetY() + radius);

					drawContext.drawPolygon(polygon);
				}
			}
		}
	}
}

// reimplemented (imod::IObserver)

bool CEdgeLineContainerShape::OnAttached(imod::IModel* modelPtr)
{
	const iedge::CEdgeLine::Container* containerPtr = dynamic_cast<const iedge::CEdgeLine::Container*>(modelPtr);
	if (containerPtr != NULL){
		return BaseClass::OnAttached(modelPtr);
	}

	return false;
}


// reimplemented (iview::CShapeBase)

i2d::CRect CEdgeLineContainerShape::CalcBoundingBox() const
{
	Q_ASSERT(IsDisplayConnected());

	i2d::CRect boundingBox = i2d::CRect::GetEmpty();

	const iedge::CEdgeLine::Container* containerPtr = dynamic_cast<const iedge::CEdgeLine::Container*>(GetModelPtr());
	if (containerPtr != NULL){
		for (int lineIndex = 0; lineIndex < containerPtr->GetItemsCount(); lineIndex++){
			const iedge::CEdgeLine& line = containerPtr->GetAt(lineIndex);

			const i2d::ICalibration2d* calibrationPtr = line.GetCalibration();
			int nodesCount = line.GetNodesCount();

			for (int i = 0; i < nodesCount; i++){
				istd::CIndex2d sp = GetScreenPosition(line.GetNode(i).GetPosition(), calibrationPtr).ToIndex2d();
				boundingBox.Union(sp);
			}
		}

		boundingBox.Expand(i2d::CRect(-5, -5, 5, 5));
	}

	return boundingBox;
}

namespace
{


/** 
	Function to calculate distance between P and a line segment connecting A and B.
	Will return a distance to a point, if it is closer than the segment.
 
	\return distance and weight between points A, B
 */
QPair<double, double> pointToLineDistance(i2d::CVector2d A, i2d::CVector2d B, i2d::CVector2d P)
{
	QPair<double, double> result;

	i2d::CVector2d ap = P - A;
	i2d::CVector2d ab = B - A;
	i2d::CVector2d n = ab.GetNormalized(1);
	double dp = ap.GetDotProduct(n);

	double weight = dp / ab.GetLength();

	if (weight < 0){ // point is before A
		result.first = ap.GetLength();
		weight = 0;
	}
	else if (weight > 1){ // point is after B
		i2d::CVector2d bp = P - B;
		result.first = bp.GetLength();
		weight = 1;
	}
	else{
		i2d::CVector2d directional = ap - dp * n;
		result.first = directional.GetLength();
	}

	result.second = weight;
	return result;
}

} // namespace


QString CEdgeLineContainerShape::GetShapeDescriptionAt(istd::CIndex2d position) const
{
	const iedge::CEdgeLine::Container* containerPtr = dynamic_cast<const iedge::CEdgeLine::Container*>(GetModelPtr());
	if (containerPtr == NULL){
		return "";
	}

	// convert minimum distance of 1.5px to logical units
	const iview::CScreenTransform& transform = GetLogToScreenTransform();
	const double minDistance = 1.5 / transform.GetDeformMatrix().GetApproxScale();
	double contourStrength = 0;

	i2d::CVector2d P = transform.GetClientPosition(position);

	for (int lineIndex = 0; lineIndex < containerPtr->GetItemsCount(); lineIndex++){
		const iedge::CEdgeLine& line = containerPtr->GetAt(lineIndex);

		for (int i = 0; i < line.GetNodesCount() - 1; i++){
			i2d::CVector2d A = line.GetNode(i).GetPosition();
			i2d::CVector2d B = line.GetNode(i + 1).GetPosition();
			QPair<double, double> distance = pointToLineDistance(A, B, P);
			double d = distance.first;
			double weight = distance.second;

			if (d <= minDistance){
				const iedge::CEdgeNode& node1 = line.GetNode(i);
				const iedge::CEdgeNode& node2 = line.GetNode(i + 1);
				double u = node1.GetWeight();
				double v = node2.GetWeight();
				double approxWeight = u + (v - u) * weight;
				double newContourStrength = ceil(approxWeight * 100 - 0.5);
				if (newContourStrength > contourStrength){
					contourStrength = newContourStrength;
				}
			}
		}
	}

	if (contourStrength > 0){
		return "Contour strength " + QString::number(contourStrength) + "%";
	}

	return "";
}


iview::ITouchable::TouchState CEdgeLineContainerShape::IsTouched(istd::CIndex2d /*position*/) const
{
	return TS_INACTIVE;
}

} // namespace iedgegui


