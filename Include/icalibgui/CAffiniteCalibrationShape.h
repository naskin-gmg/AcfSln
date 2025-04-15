#ifndef icalibgui_CAffiniteCalibrationShape_included
#define icalibgui_CAffiniteCalibrationShape_included


#include <icalibgui/CNoneCalibrationShape.h>


namespace icalibgui
{


// TODO: Redesign it to ACF transformation concept.

/**
	Shape object for affinite calibration.
	You may use this shape to visualize affine calibration object on display console.
	\sa simple implementation of affine transformation: \c i2d::CAffineTransformation2d.
*/
class CAffiniteCalibrationShape: public icalibgui::CNoneCalibrationShape
{
public:
	typedef icalibgui::CNoneCalibrationShape BaseClass;

	// reimplemented (iview::IVisualizable)
	virtual void Draw(QPainter& drawContext) const override;
};


} // namespace icalibgui


#endif // !icalibgui_CAffiniteCalibrationShape_included


