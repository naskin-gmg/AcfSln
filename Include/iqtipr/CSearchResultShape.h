#ifndef iqtipr_CSearchResultShape_included
#define iqtipr_CSearchResultShape_included


// Qt includes
#include <QGraphicsItemGroup>

// ACF includes
#include "istd/TPointerVector.h"
#include "imod/TSingleModelObserverBase.h"
#include "i2d/CVector2d.h"

#include "iipr/IFeaturesProvider.h"


namespace iqtipr
{


class CSearchResultShape: 
			public QGraphicsItemGroup, 
			public imod::TSingleModelObserverBase<iipr::IFeaturesProvider>
{
public:
	// reimplemented (imod::IObserver)
	virtual void BeforeUpdate(imod::IModel* modelPtr, int updateFlags, istd::IPolymorphic* updateParamsPtr);
	virtual void AfterUpdate(imod::IModel* modelPtr, int updateFlags, istd::IPolymorphic* updateParamsPtr);

private:
	class CModelPointShape: public QGraphicsEllipseItem
	{
	public:
		typedef QGraphicsEllipseItem BaseClass;

		CModelPointShape(
					const i2d::CVector2d& position,
					const i2d::CVector2d& size,
					QGraphicsItem* parentPtr = NULL,
					QGraphicsScene* scenePtr = NULL,
					double angle = 0);
	
	protected:
		// reimplemented (QGraphicsRectItem)
		virtual void paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget*);

	private:
		double m_angle;
	};

	typedef istd::TPointerVector<QGraphicsItem> Items;

	Items m_items;
};


} // namespace iqtipr


#endif // !iqtipr_CSearchResultShape_included

