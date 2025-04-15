#ifndef icalibgui_CNoneCalibrationShape_included
#define icalibgui_CNoneCalibrationShape_included


// ACF includes
#include <iview/CShapeBase.h>


namespace icalibgui
{


/**
	Shape object for none calibration.
	You need this shape to visualize grid without calibration on the shape view.
*/
class CNoneCalibrationShape: public iview::CShapeBase
{
public:
	typedef iview::CShapeBase BaseClass;

	// reimplemented (iview::IVisualizable)
	virtual void Draw(QPainter& drawContext) const override;

	// reimplemented (iview::IInteractiveShape)
	virtual TouchState IsTouched(istd::CIndex2d position) const override;

	// reimplemented (iview::IMouseActionObserver)
	virtual bool OnMouseButton(istd::CIndex2d position, Qt::MouseButton buttonType, bool downFlag);
	virtual bool OnMouseMove(istd::CIndex2d position);

protected:
	/**
		Get access to calibration.
	*/
	virtual const i2d::ICalibration2d* GetCalibration() const;

	// reimplemented (iview::CInteractiveShapeBase)
	virtual void BeginLogDrag(const i2d::CVector2d& reference);
	virtual void SetLogDragPosition(const i2d::CVector2d& position);

	// reimplemented (iview::CShapeBase)
	virtual i2d::CRect CalcBoundingBox() const override;
};


} // namespace icalibgui


#endif // !icalibgui_CNoneCalibrationShape_included


