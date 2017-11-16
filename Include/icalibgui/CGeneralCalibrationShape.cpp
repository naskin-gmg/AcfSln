#include <icalibgui/CGeneralCalibrationShape.h>


// Qt includes
#include <QtCore/QtGlobal>
#if QT_VERSION >= 0x050000
#include <QtCore/QtMath>
#else
#include <QtCore/qmath.h>
#endif
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

		const iview::CScreenTransform&  viewToScreenTransform = GetViewToScreenTransform();

		// calculate optimal argument boundaries
		i2d::CRect clientRect = GetClientRect();

		i2d::CRectangle clientProjectedBounds = i2d::CRectangle::GetInvalid();
		for (int y = 0; y <= 10; ++y){
			for (int x = 0; x <= 10; ++x){
				i2d::CVector2d screenPos(clientRect.GetLeft() + clientRect.GetWidth() * x / 10.0, clientRect.GetTop() + clientRect.GetHeight() * y / 10.0);

				i2d::CVector2d logPos;
				if (calibPtr->GetInvPositionAt(viewToScreenTransform.GetInvertedApply(screenPos), logPos)){
					if (clientProjectedBounds.IsValid()){
						clientProjectedBounds.Unite(logPos);
					}
					else{
						clientProjectedBounds = i2d::CRectangle(logPos, logPos);
					}
				}
			}
		}

		i2d::CRectangle bounds;

		const i2d::CRectangle* argumentAreaPtr = calibPtr->GetArgumentArea();
		if (argumentAreaPtr != NULL){
			bounds = *argumentAreaPtr;

			if (!clientProjectedBounds.IsEmpty()){
				bounds.Intersect(clientProjectedBounds);
			}
		}
		else if (!clientProjectedBounds.IsEmpty()){
			bounds = clientProjectedBounds;		}
		else{
			bounds = i2d::CRectangle(-1000, -1000, 2000, 2000);
		}

		i2d::CRectangle smallBounds(bounds.GetLeftTop() * 0.25 + bounds.GetRightBottom() * 0.75, bounds.GetLeftTop() * 0.75 + bounds.GetRightBottom() * 0.25);

		i2d::CVector2d viewLeftCenter;
		calibPtr->GetPositionAt(smallBounds.GetLeftCenter(), viewLeftCenter);
		i2d::CVector2d viewRightCenter;
		calibPtr->GetPositionAt(smallBounds.GetRightCenter(), viewRightCenter);
		i2d::CVector2d viewTopCenter;
		calibPtr->GetPositionAt(smallBounds.GetTopCenter(), viewTopCenter);
		i2d::CVector2d viewBottomCenter;
		calibPtr->GetPositionAt(smallBounds.GetBottomCenter(), viewBottomCenter);

		double calibScale = qAbs(qMax(viewLeftCenter.GetDistance(viewRightCenter) / smallBounds.GetWidth(), viewTopCenter.GetDistance(viewBottomCenter) / smallBounds.GetHeight()));
		double viewScale = qAbs(viewToScreenTransform.GetDeformMatrix().GetApproxScale());

		int levels[2];
		double minGridDistance = calibInfoPtr->GetMinGridDistance() / (viewScale * calibScale);
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

		int firstIndexX = int(qFloor(bounds.GetLeft() / grid));
		int lastIndexX = int(qCeil(bounds.GetRight() / grid));
		int firstIndexY = int(qFloor(bounds.GetTop() / grid));
		int lastIndexY = int(qCeil(bounds.GetBottom() / grid));

		if ((firstIndexX >= lastIndexX) || (firstIndexY >= lastIndexY)){
			return;
		}

		drawContext.save();
		drawContext.setBrush(colorSchema.GetBrush(iview::IColorSchema::SB_TRANSPARENT));
		const QPen& level0Pen = colorSchema.GetPen(iview::IColorSchema::SP_GUIDELINE3);
		const QPen& level1Pen = colorSchema.GetPen(iview::IColorSchema::SP_GUIDELINE2);
		const QPen& level2Pen = colorSchema.GetPen(iview::IColorSchema::SP_GUIDELINE1);

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

				if ((firstPoint.rx() != qInf()) && (firstPoint.ry() != qInf())){
					if (indexX < lastIndexX){
						QPointF secondPoint = gridPoints.GetAt(istd::CIndex2d(indexX - firstIndexX + 1, indexY - firstIndexY));

						if ((secondPoint.rx() != qInf()) && (secondPoint.ry() != qInf())){
							// draw horizontal line
							if (indexY % levels[1] == 0){
								drawContext.setPen(level2Pen);
							}
							else if (indexY % levels[0] == 0){
								drawContext.setPen(level1Pen);
							}
							else{
								drawContext.setPen(level0Pen);
							}

							drawContext.drawLine(firstPoint, secondPoint);
						}
					}

					if (indexY < lastIndexY){
						QPointF secondPoint = gridPoints.GetAt(istd::CIndex2d(indexX - firstIndexX, indexY - firstIndexY + 1));

						if ((secondPoint.rx() != qInf()) && (secondPoint.ry() != qInf())){
							// draw vertical line
							if (indexX % levels[1] == 0){
								drawContext.setPen(level2Pen);
							}
							else if (indexX % levels[0] == 0){
								drawContext.setPen(level1Pen);
							}
							else{
								drawContext.setPen(level0Pen);
							}

							drawContext.drawLine(firstPoint, secondPoint);
						}
					}
				}
			}
		}

		drawContext.restore();
	}
}


} // namespace icalibgui


