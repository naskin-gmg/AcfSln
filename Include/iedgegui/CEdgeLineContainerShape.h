#ifndef iedgegui_CEdgeLineContainerShape_included
#define iedgegui_CEdgeLineContainerShape_included


// ACF includes
#include "icmm/IColorTransformation.h"
#include "iview/CShapeBase.h"


namespace iedgegui
{


class CEdgeLineContainerShape: public iview::CShapeBase
{
public:
	typedef iview::CShapeBase BaseClass;

	// reimplemented (iview::IShape)
	virtual void Draw(QPainter& drawContext) const;

	// reimplemented (imod::IObserver)
	virtual bool OnAttached(imod::IModel* modelPtr);

	// reimplemented (iview::CShapeBase)
	virtual i2d::CRect CalcBoundingBox() const;

	virtual QString GetShapeDescriptionAt(istd::CIndex2d position) const;
	
	iview::ITouchable::TouchState IsTouched(istd::CIndex2d /*position*/) const;
};


} // namespace iedgegui


#endif // !iedgegui_CEdgeLineContainerShape_included


