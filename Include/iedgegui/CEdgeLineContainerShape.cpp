#include "iedgegui/CEdgeLineContainerShape.h"


// Qt includes
#include <QtGui/QPainter>

// ACF includes
#include "imod/IModel.h"
#include "iview/IColorSchema.h"
#include "iview/CScreenTransform.h"
#include "iedge/CEdgeLine.h"
#include "iedge/CEdgeLineContainer.h"


namespace iedgegui
{


// reimplemented (iview::IVisualizable)

void CEdgeLineContainerShape::Draw(QPainter& drawContext) const
{
	const iedge::CEdgeLineContainer* containerPtr = dynamic_cast<const iedge::CEdgeLineContainer*>(GetModelPtr());
	if (IsDisplayConnected() && containerPtr != NULL){
		int numLines = containerPtr->GetItemsCount();

		if (numLines > 0){
			double scale = qSqrt(GetViewToScreenTransform().GetDeformMatrix().GetApproxScale());

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

				int nodesCount = line.GetNodesCount();

				if (nodesCount > 1){
					int pointIndex = line.IsClosed()? nodesCount - 1: 0;

					// first point position
					const iedge::CEdgeNode& firstNode = line.GetNode(pointIndex);
					i2d::CVector2d prevPos = GetScreenPosition(firstNode.GetPosition());
					double prevWeight = firstNode.GetWeight();

					for (pointIndex = (pointIndex + 1) % nodesCount; pointIndex < nodesCount; pointIndex++){
						const iedge::CEdgeNode& node = line.GetNode(pointIndex);
						i2d::CVector2d pos = GetScreenPosition(node.GetPosition());
						double weight = node.GetWeight();

						i2d::CVector2d dif = pos - prevPos;
						dif.Normalize();
						i2d::CVector2d orthogonal = dif.GetOrthogonal();

						i2d::CVector2d thickVector = orthogonal * qMin(4.0, scale * prevWeight);
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
					i2d::CVector2d pos = GetScreenPosition(node.GetPosition());
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
	const iedge::CEdgeLineContainer* containerPtr = dynamic_cast<const iedge::CEdgeLineContainer*>(modelPtr);
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

	const iedge::CEdgeLineContainer* containerPtr = dynamic_cast<const iedge::CEdgeLineContainer*>(GetModelPtr());
	if (containerPtr != NULL){
		for (int lineIndex = 0; lineIndex < containerPtr->GetItemsCount(); lineIndex++){
			const iedge::CEdgeLine& line = containerPtr->GetAt(lineIndex);

			int nodesCount = line.GetNodesCount();

			for (int i = 0; i < nodesCount; i++){
				istd::CIndex2d sp = GetScreenPosition(line.GetNode(i).GetPosition()).ToIndex2d();
				boundingBox.Union(sp);
			}
		}

		boundingBox.Expand(i2d::CRect(-5, -5, 5, 5));
	}

	return boundingBox;
}


QString CEdgeLineContainerShape::GetShapeDescriptionAt(istd::CIndex2d position) const
{
	const iedge::CEdgeLineContainer* containerPtr = dynamic_cast<const iedge::CEdgeLineContainer*>(GetModelPtr());
	if (containerPtr == NULL){
		return "";
	}

	// convert minimum distance of 1.5px to logical units
	const iview::CScreenTransform& transform = GetViewToScreenTransform();
	double scale = qSqrt(transform.GetDeformMatrix().GetApproxScale());

	i2d::CVector2d logPosition = GetLogPosition(position);

	int bestLineIndex = -1;	// index of the nearest found edge line
	double bestLineStrength = 0;	// strength (weight) of the nearest found edge line
	double bestLinePosition = 0;	// position of the nearest found edge line

	for (int lineIndex = 0; lineIndex < containerPtr->GetItemsCount(); lineIndex++){
		const iedge::CEdgeLine& line = containerPtr->GetAt(lineIndex);

		int nodesCount = line.GetNodesCount();
		if (nodesCount > 1){
			i2d::CRectangle lineBoundingBox = line.GetBoundingBox();

			if (lineBoundingBox.IsEmpty() || !lineBoundingBox.GetExpanded(i2d::CRectangle(-4, -4, 4, 4)).Contains(logPosition)){
				continue;
			}

			double segmentPosition = 0;

			int segmentsCount = line.GetSegmentsCount();
			for (int i = 0; i < segmentsCount; i++){
				const iedge::CEdgeNode& node1 = line.GetNode(i);
				const iedge::CEdgeNode& node2 = line.GetNode((i + 1) % nodesCount);

				i2d::CLine2d segmentLine(GetScreenPosition(node1.GetPosition()), GetScreenPosition(node2.GetPosition()));

				double segmentLength = segmentLine.GetLength();

				if (segmentLength > I_BIG_EPSILON){
					QPair<double, double> castDistance = segmentLine.GetAlphaAndCastDistance(position);
					double alpha = castDistance.first;
					double distance = qAbs(castDistance.second);

					if ((alpha >= 0) && (alpha <= 1)){
						double weight1 = node1.GetWeight();
						double weight2 = node2.GetWeight();

						double weight = weight2 * alpha + weight1 * (1 - alpha);

						double minDistance = qMax(1.0, qMin(4.0, scale * weight));
						if (distance <= minDistance){
							if (weight > bestLineStrength){
								bestLineIndex = lineIndex;
								bestLinePosition = segmentPosition + segmentLength * alpha;
								bestLineStrength = weight;
							}
						}
					}
				}

				segmentPosition += segmentLength;
			}
		}
		else if (nodesCount == 1){
			const iedge::CEdgeNode& node = line.GetNode(0);
			i2d::CVector2d screenPoint = GetScreenPosition(node.GetPosition());
			double distance = screenPoint.GetDistance(i2d::CVector2d(position));
			double weight = node.GetWeight();

			double minDistance = qMax(1.0, qMin(4.0, scale * weight));
			if (distance <= minDistance){
				if (weight > bestLineStrength){
					bestLineIndex = lineIndex;
					bestLinePosition = 0;
					bestLineStrength = weight;
				}
			}
		}
	}

	if (bestLineIndex >= 0){
		const iedge::CEdgeLine& line = containerPtr->GetAt(bestLineIndex);

		double lineLength = line.GetTotalLength();

		QString unitName;
		const i2d::ICalibration2d* calibrationPtr = line.GetCalibration();
		if (calibrationPtr != NULL){
			const imath::IUnitInfo* unitInfoPtr = calibrationPtr->GetArgumentUnitInfo();
			if (unitInfoPtr != NULL){
				unitName = unitInfoPtr->GetUnitName();
			}
		}

		return QObject::tr("Line at %1%3 (total %2%3): strength %4%")
					.arg(bestLinePosition, 0, 'f', 2)
					.arg(lineLength, 0, 'f', 2)
					.arg(unitName)
					.arg(int(bestLineStrength * 100 + 0.5));
	}

	return "";
}


iview::ITouchable::TouchState CEdgeLineContainerShape::IsTouched(istd::CIndex2d /*position*/) const
{
	return TS_INACTIVE;
}


} // namespace iedgegui


