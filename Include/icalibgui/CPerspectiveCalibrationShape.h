#ifndef icalibgui_CPerspectiveCalibrationShape_included
#define icalibgui_CPerspectiveCalibrationShape_included


#include <icalibgui/CNoneCalibrationShape.h>


namespace icalibgui
{


/**
	Shape object for perspective calibration.
	You need this shape to visualize calibration of type icalibgui::CPerspectiveCalibration on the shape view.
*/
class CPerspectiveCalibrationShape: public CNoneCalibrationShape
{
public:
	typedef CNoneCalibrationShape BaseClass;

	// reimplemented (imod::IObserver)
	virtual void Invalidate();

	// reimplemented (iview::IVisualizable)
	virtual void Draw(QPainter& drawContext) const;

protected:
	bool GetLineScreenPosition(const i2d::ICalibration2d& calib, const i2d::CLine2d& logLine, int gridSize, QPointF& point1, QPointF& point2) const;
};


} // namespace icalibgui


#endif // !icalibgui_CPerspectiveCalibrationShape_included


