#ifndef icalibgui_CGeneralCalibrationShape_included
#define icalibgui_CGeneralCalibrationShape_included


#include <icalibgui/CNoneCalibrationShape.h>


namespace icalibgui
{


/**
	Shape object for perspective calibration.
	You need this shape to visualize calibration of type icalibgui::CPerspectiveCalibration on the shape view.
*/
class CGeneralCalibrationShape: public CNoneCalibrationShape
{
public:
	typedef CNoneCalibrationShape BaseClass;

	// reimplemented (imod::IObserver)
	virtual void Invalidate() override;

	// reimplemented (iview::IVisualizable)
	virtual void Draw(QPainter& drawContext) const override;
};


} // namespace icalibgui


#endif // !icalibgui_CGeneralCalibrationShape_included


