#ifndef icmpstr_CGraphicsConnectorItem_included
#define icmpstr_CGraphicsConnectorItem_included


// Qt includes
#include <QtCore/QObject>
#if QT_VERSION >= 0x050000
#include <QtWidgets/QGraphicsItem>
#else
#include <QtGui/QGraphicsItem>
#endif

// ACF-Solutions includes
#include <icmpstr/ISceneProvider.h>


namespace icmpstr
{


class CComponentSceneItem;
class CRegistryElementShape;


class CGraphicsConnectorItem:
			public QObject,
			public QGraphicsItem
{
	Q_OBJECT

public:
	typedef QGraphicsItem BaseClass;

	enum ConnectFlags
	{
		CF_FACTORY = 0x0001,
		CF_EMBEDDED  =0x0002
	};

	enum GeometricParams
	{
		GP_RADIUS = 4,
		GP_RADIUS2 = 8,
		GP_OFFSET = 12
	};

	CGraphicsConnectorItem(
				const icmpstr::ISceneProvider& sceneProvider,
				int connectFlags = 0,
				QGraphicsItem* parent = NULL);

	void InitEnds(CRegistryElementShape* sourceShapePtr, CRegistryElementShape* destShapePtr);

	// reimplemented (QGraphicsItem)
	virtual int type() const override;
	virtual QPainterPath shape() const override;
	virtual bool contains(const QPointF& point) const override;

protected:
	void Adjust();

	// reimplemented (QGraphicsItem)
	virtual QRectF boundingRect() const override;
	virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;

protected Q_SLOTS:
	void OnSourceRectMoved(const QRectF& rect);
	void OnDestRectMoved(const QRectF& rect);
	void OnSourceSelected(bool state);
	void OnDestSelected(bool state);

private:
	const icmpstr::ISceneProvider& m_sceneProvider;

	int m_connectFlags;

	QPolygonF m_connectionLine;
	QPointF m_touchPoint;

	QRectF m_sourceRect;
	QRectF m_destRect;
	bool m_isSourceSelected;
	bool m_isDestSelected;
};


} // namespace icmpstr


#endif // !icmpstr_CGraphicsConnectorItem_included


