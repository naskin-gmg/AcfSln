#ifndef iedgegui_CEdgeLineContainerShape_included
#define iedgegui_CEdgeLineContainerShape_included


// ACF includes
#include <icmm/IColorTransformation.h>
#include <iview/CShapeBase.h>


namespace iedgegui
{


class CEdgeLineContainerShape: public iview::CShapeBase
{
public:
	typedef iview::CShapeBase BaseClass;

	// reimplemented (iview::IShape)
	virtual void Draw(QPainter& drawContext) const override;

	// reimplemented (imod::IObserver)
	virtual bool OnModelAttached(imod::IModel* modelPtr, istd::IChangeable::ChangeSet& changeMask) override;

	// reimplemented (iview::CShapeBase)
	virtual i2d::CRect CalcBoundingBox() const override;

	virtual QString GetShapeDescriptionAt(istd::CIndex2d position) const override;
	
	virtual iview::ITouchable::TouchState IsTouched(istd::CIndex2d /*position*/) const override;
};


} // namespace iedgegui


#endif // !iedgegui_CEdgeLineContainerShape_included


