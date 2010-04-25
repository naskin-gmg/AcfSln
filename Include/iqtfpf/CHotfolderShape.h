#ifndef iqtfpf_CHotfolderShape_included
#define iqtfpf_CHotfolderShape_included


// Qt includes
#include <QGraphicsRectItem>



// ACF includes
#include "imod/TSingleModelObserverBase.h"

#include "ifpf/CVisualHotfolderWorkflowItem.h"

#include "iqt2d/TObjectShapeBase.h"



namespace iqtfpf
{


class CHotfolderShape: public iqt2d::TObjectShapeBase<QGraphicsRectItem, ifpf::CVisualHotfolderWorkflowItem>
{
	Q_OBJECT

public:
	typedef iqt2d::TObjectShapeBase<QGraphicsRectItem, ifpf::CVisualHotfolderWorkflowItem> BaseClass;

	CHotfolderShape(const iqt2d::ISceneProvider* providerPtr = NULL);

	QRectF GetViewRect() const;

	// reimplemented (QGraphicsItem)
	virtual QRectF boundingRect() const;
	virtual void paint(QPainter* painterPtr, const QStyleOptionGraphicsItem* stylePtr, QWidget* widgetPtr);

	// reimplemented (imod::IObserver)
	virtual bool OnAttached(imod::IModel* modelPtr);

Q_SIGNALS:
	void SelectionChanged(bool state);

protected:
	enum
	{
		SIDE_OFFSET = 4,
		SHADOW_OFFSET = 10
	};

	// reimplemented (iqt2d::TObjectShapeBase)
	virtual void UpdateGraphicsItem(const ifpf::CVisualHotfolderWorkflowItem& element);

	// reimplemented (TShapeBase)
	virtual void OnSelectionChanged(bool isSelected);

private:
	QGraphicsTextItem* m_hotfolderName;

};


} // namespace iqtfpf


#endif // !iqtfpf_CHotfolderShape_included


