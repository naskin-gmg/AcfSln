#include "iedgegui/CEdgeLineContainerShape.h"


// Qt includes
#include <QtGui/QPainter>

// ACF includes
#include "imod/IModel.h"
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
			i2d::CAffine2d destTransform = GetLogToScreenTransform();

			double scale = qMin(4.0, qSqrt(destTransform.GetDeformMatrix().GetFrobeniusNorm()));

			// prepare draw context
			QPen pen;
			pen.setStyle(Qt::NoPen);
			QBrush brush(Qt::green, Qt::SolidPattern);
			drawContext.setPen(pen);
			drawContext.setBrush(brush);

			QPolygonF polygon(4);

			for(int lineIndex = 0; lineIndex < numLines; lineIndex++){
				const iedge::CEdgeLine& line = containerPtr->GetAt(lineIndex);
				// first point position
				i2d::CVector2d prevPos = destTransform.GetApply(line.GetNode(0).GetPosition());
				double prevWeight = line.GetNode(0).GetWeight();
				// ignore case when a polyline consists of only a single point
				for(int pointIndex = 1; pointIndex < line.GetNodesCount(); pointIndex++){
					const iedge::CEdgeNode& node = line.GetNode(pointIndex);
					i2d::CVector2d pos = destTransform.GetApply(node.GetPosition());
					double weight = node.GetWeight();

					i2d::CVector2d dif = pos - prevPos;
					dif.Normalize(scale);
					i2d::CVector2d orthogonal = dif.GetOrthogonal();

					i2d::CVector2d thickVector = orthogonal * prevWeight;
					polygon[0] = QPointF(prevPos - thickVector);
					polygon[1] = QPointF(prevPos + thickVector);

					thickVector = orthogonal * weight;
					polygon[2] = QPointF(pos + thickVector);
					polygon[3] = QPointF(pos - thickVector);

					drawContext.drawPolygon(polygon);

					prevPos = pos;
					prevWeight = weight;
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

	const iedge::CEdgeLine::Container* containerPtr = dynamic_cast<const iedge::CEdgeLine::Container*>(GetModelPtr());
	i2d::CRect boundingBox = i2d::CRect::GetEmpty();
	if (containerPtr != NULL){
		for (int lineIndex = 0; lineIndex < containerPtr->GetItemsCount(); lineIndex++){
			const iview::CScreenTransform& transform = GetLogToScreenTransform();
			const iedge::CEdgeLine& line = containerPtr->GetAt(lineIndex);
			int nodesCount = line.GetNodesCount();
			if (nodesCount > 0){
				istd::CIndex2d sp = transform.GetScreenPosition(line.GetNode(0).GetPosition());
				if (boundingBox.IsEmpty()){
					boundingBox = i2d::CRect(sp, sp);
				}
				else{
					boundingBox.Union(sp);
				}

				for (int i = 1; i < nodesCount; i++){
					sp = transform.GetScreenPosition(line.GetNode(i).GetPosition());
					boundingBox.Union(sp);
				}
			}
		}
		boundingBox.Expand(i2d::CRect(-4, -4, 4, 4));
	}

	return boundingBox;
}


} // namespace iedgegui


