#ifndef icmpstr_CHotfolderShape_included
#define icmpstr_CHotfolderShape_included


// Qt includes
#include <QGraphicsItem>



// ACF includes
#include "imod/TSingleModelObserverBase.h"

#include "ifpf/CHotfolderVisualisation.h"

#include "iqt2d/TObjectShapeBase.h"



namespace icmpstr
{


class CHotfolderShape: public iqt2d::TObjectShapeBase<QGraphicsRectItem, ifpf::CHotfolderVisualisation>
{
	Q_OBJECT

public:
	typedef iqt2d::TObjectShapeBase<QGraphicsRectItem, ifpf::CHotfolderVisualisation> BaseClass;

	CHotfolderShape(const iqt2d::ISceneProvider* providerPtr = NULL);

	QRectF GetViewRect() const;

	// reimplemented (QGraphicsItem)
	virtual QRectF boundingRect() const;
	virtual void paint(QPainter* painterPtr, const QStyleOptionGraphicsItem* stylePtr, QWidget* widgetPtr);
	virtual void mouseMoveEvent(QGraphicsSceneMouseEvent* eventPtr);
	virtual void mouseDoubleClickEvent(QGraphicsSceneMouseEvent* eventPtr);

	// reimplemented (imod::IObserver)
	virtual bool OnAttached(imod::IModel* modelPtr);

signals:
	void RectChanged(QRectF rect);
	void SelectionChanged(bool state);

protected:
	enum
	{
		SIDE_OFFSET = 4,
		SHADOW_OFFSET = 10
	};

	// reimplemented (iqt2d::TObjectShapeBase)
	void UpdateGraphicsItem(const iprm::IParamsSet& element);

	// reimplemented (TShapeBase)
	virtual void OnSelectionChanged(bool isSelected);

private:
	QStringList m_exportedInterfacesList;

	// shadows
	QString m_addressString;
	QIcon m_icon;
	bool m_isConsistent;
};


} // namespace icmpstr


#endif // !icmpstr_CHotfolderShape_included


