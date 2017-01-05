#include <icalibgui/CGeneralCalibrationShape.h>


// Qt includes
#include <QtCore/QtMath>
#include <QtGui/QPainter>

// ACF includes
#include <istd/CChangeNotifier.h>
#include <istd/TArray.h>
#include <i2d/ICalibration2d.h>
#include <iview/IRuler.h>
#include <iview/IViewRulersAccessor.h>
#include <iview/CCalibratedViewBase.h>


namespace icalibgui
{


// reimplemented (imod::IObserver)

void CGeneralCalibrationShape::Invalidate()
{
	BaseClass::Invalidate();

	iview::IDisplay* displayPtr = GetDisplayPtr();
	iview::CViewBase* parentViewPtr = NULL;
	while (displayPtr != NULL){
		iview::CViewBase* viewPtr = dynamic_cast<iview::CViewBase*>(displayPtr);
		if (viewPtr != NULL){
			parentViewPtr = viewPtr;
		}

		displayPtr = displayPtr->GetParentDisplayPtr();
	}

	if (parentViewPtr != NULL){
		static const istd::IChangeable::ChangeSet calibChangeSet(CF_CALIB);
		parentViewPtr->UpdateAllShapes(calibChangeSet);
	}
}


// reimplemented (iview::IVisualizable)

void CGeneralCalibrationShape::Draw(QPainter& drawContext) const
{
	const i2d::ICalibration2d* calibPtr = GetCalibration();
	if (calibPtr == NULL){
		BaseClass::Draw(drawContext);

		return;
	}

	if (IsDisplayConnected()){
		i2d::CRect clientRect = GetClientRect();

		iview::IVisualCalibrationInfo* calibInfoPtr = NULL;
		iview::IDisplay* displayPtr = GetDisplayPtr();
		while (displayPtr != NULL){
			if (calibInfoPtr == NULL){
				calibInfoPtr = dynamic_cast<iview::IVisualCalibrationInfo*>(displayPtr);
			}
			displayPtr = displayPtr->GetParentDisplayPtr();
		}

		if (calibInfoPtr != NULL){
			if (!calibInfoPtr->IsGridInMm()){
				BaseClass::Draw(drawContext);
				return;
			}
		}

		if (calibInfoPtr == NULL){
			return;
		}

		i2d::CLine2d topRulerLine;
		i2d::CLine2d leftRulerLine;

		bool isGridVisible = calibInfoPtr->IsGridVisible();
		if (!isGridVisible){
			return;
		}

		const iview::IColorSchema& colorSchema = GetColorSchema();

		i2d::CVector2d logCorners[4];
		logCorners[0] = GetLogPosition(clientRect.GetLeftTop());
		logCorners[1] = GetLogPosition(clientRect.GetRightTop());
		logCorners[2] = GetLogPosition(clientRect.GetLeftBottom());
		logCorners[3] = GetLogPosition(clientRect.GetRightBottom());

		i2d::CRectangle bounds(-100, -100, 200, 200);
		const i2d::CRectangle* argumentAreaPtr = calibPtr->GetArgumentArea();
		if (argumentAreaPtr != NULL){
			bounds = *argumentAreaPtr;
		}

		i2d::CVector2d viewLeftTop;
		calibPtr->GetInvPositionAt(bounds.GetLeftTop(), viewLeftTop);
		i2d::CVector2d viewLeftBottom;
		calibPtr->GetInvPositionAt(bounds.GetLeftBottom(), viewLeftBottom);
		i2d::CVector2d viewRightTop;
		calibPtr->GetInvPositionAt(bounds.GetRightTop(), viewRightTop);
		i2d::CVector2d viewRightBottom;
		calibPtr->GetInvPositionAt(bounds.GetRightBottom(), viewRightBottom);

		double calibScale = (
					(viewLeftTop.GetDistance(viewRightTop) + viewLeftBottom.GetDistance(viewRightBottom)) / bounds.GetWidth() +
					(viewLeftTop.GetDistance(viewLeftBottom) + viewRightTop.GetDistance(viewRightBottom)) / bounds.GetHeight()) * 0.25;

		double viewScale = GetViewToScreenTransform().GetDeformMatrix().GetApproxScale();

		int levels[2];
		double minGridDistance = 10 * calibInfoPtr->GetMinGridDistance() * calibScale / viewScale;
		double grid = qPow(10.0, qCeil(log10(minGridDistance)));
		if (grid * 0.5 < minGridDistance){
			levels[0] = 5;
			levels[1] = 10;
		}
		else{
			grid *= 0.5;
			levels[0] = 2;
			levels[1] = 20;
		}

		drawContext.save();
		drawContext.setBrush(colorSchema.GetBrush(iview::IColorSchema::SB_TRANSPARENT));
		const QPen& level0Pen = colorSchema.GetPen(iview::IColorSchema::SP_GUIDELINE3);
		const QPen& level1Pen = colorSchema.GetPen(iview::IColorSchema::SP_GUIDELINE2);
		const QPen& level2Pen = colorSchema.GetPen(iview::IColorSchema::SP_GUIDELINE1);

		int firstIndexY = int(qFloor(bounds.GetTop() / grid));
		int lastIndexY = int(qCeil(bounds.GetBottom() / grid));
		int firstIndexX = int(qFloor(bounds.GetLeft() / grid));
		int lastIndexX = int(qCeil(bounds.GetRight() / grid));

		istd::TArray<QPointF, 2> gridPoints;
		gridPoints.SetSizes(istd::CIndex2d(lastIndexX - firstIndexX + 1, lastIndexY - firstIndexY + 1));

		for (int indexY = firstIndexY; indexY <= lastIndexY; ++indexY){
			for (int indexX = firstIndexX; indexX <= lastIndexX; ++indexX){
				i2d::CVector2d logPos(indexX * grid, indexY * grid);
				i2d::CVector2d viewPosition;
				QPointF screenPos(qInf(), qInf());
				if (calibPtr->GetPositionAt(logPos, viewPosition)){
					screenPos = GetScreenPosition(viewPosition);
				}
				gridPoints.SetAt(istd::CIndex2d(indexX - firstIndexX, indexY - firstIndexY), screenPos);
			}
		}
						
		for (int indexY = firstIndexY; indexY <= lastIndexY; ++indexY){
			for (int indexX = firstIndexX; indexX <= lastIndexX; ++indexX){
				QPointF firstPoint = gridPoints.GetAt(istd::CIndex2d(indexX - firstIndexX, indexY - firstIndexY));

				if (indexX < lastIndexX){
					QPointF secondPoint = gridPoints.GetAt(istd::CIndex2d(indexX - firstIndexX + 1, indexY - firstIndexY));

					// draw horizontal line
					if (indexX % levels[1] == 0){
						drawContext.setPen(level2Pen);
					}
					else if (indexX % levels[0] == 0){
						drawContext.setPen(level1Pen);
					}
					else{
						drawContext.setPen(level0Pen);
					}

					if (		(firstPoint.rx() != qInf()) && (firstPoint.ry() != qInf()) &&
								(secondPoint.rx() != qInf()) && (secondPoint.ry() != qInf())){
						drawContext.drawLine(firstPoint, secondPoint);
					}
				}

				if (indexY < lastIndexY){
					QPointF secondPoint = gridPoints.GetAt(istd::CIndex2d(indexX - firstIndexX, indexY - firstIndexY + 1));

					// draw vertical line
					if (indexY % levels[1] == 0){
						drawContext.setPen(level2Pen);
					}
					else if (indexY % levels[0] == 0){
						drawContext.setPen(level1Pen);
					}
					else{
						drawContext.setPen(level0Pen);
					}

					if (		(firstPoint.rx() != qInf()) && (firstPoint.ry() != qInf()) &&
								(secondPoint.rx() != qInf()) && (secondPoint.ry() != qInf())){
						drawContext.drawLine(firstPoint, secondPoint);
					}
				}
			}
		}

		drawContext.restore();
	}
}


} // namespace icalibgui


