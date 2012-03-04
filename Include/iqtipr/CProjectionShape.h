#ifndef iqtipr_CProjectionShape_included
#define iqtipr_CProjectionShape_included


// ACF includes
#include "iview/CShapeBase.h"

// ACF-Solutions includes
#include "imeas/IDataSequence.h"

#include "iqtipr/iqtipr.h"


namespace iqtipr
{


class CProjectionShape: public iview::CShapeBase
{
public:
	typedef iview::CShapeBase BaseClass;

	CProjectionShape();

	// reimplemented (iview::IVisualizable)
	virtual void Draw(QPainter& drawContext) const;

	// reimplemented (iview::TShapeBase)
	virtual void CalcBoundingBox(i2d::CRect& result) const;

	// reimplemented (imod::IObserver)
	virtual void AfterUpdate(imod::IModel* modelPtr, int updateFlags, istd::IPolymorphic* updateParamsPtr);

protected:
	void UpdateFigure() const;

private:
	i2d::CLine2d m_projectionLine;

	mutable QPainterPath m_path;
	mutable QTransform m_transform;
};


} // namespace iqtipr


#endif // !iqtipr_CProjectionShape_included


