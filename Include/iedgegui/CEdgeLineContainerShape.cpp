#include "iedgegui/CEdgeLineContainerShape.h"


// Qt includes
#include <QtGui/QPainter>

// ACF includes
#include "imod/IModel.h"
#include "iview/IColorShema.h"
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

			const iview::IColorShema& colorShema = GetColorShema();
			const QPen& defaultPen = colorShema.GetPen(iview::IColorShema::SP_NORMAL);

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
	I_ASSERT(IsDisplayConnected());

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


} // namespace iedgegui


