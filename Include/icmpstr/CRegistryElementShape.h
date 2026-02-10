// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#ifndef icmpstr_CRegistryElementShape_included
#define icmpstr_CRegistryElementShape_included


// Qt includes
#include <QtCore/QDir>
#if QT_VERSION >= 0x050000
#include <QtWidgets/QGraphicsItem>
#else
#include <QtGui/QGraphicsItem>
#endif

// ACF includes
#include <imod/TSingleModelObserverBase.h>

// ACF-Solutions includes
#include <icmpstr/TObjectShapeBase.h>
#include <icmpstr/CVisualRegistryElement.h>


namespace icmpstr
{


class CVisualRegistryEditorComp;


/**
	Visualization of geometrical registry elements.
*/
class CRegistryElementShape: public icmpstr::TObjectShapeBase<QGraphicsRectItem, CVisualRegistryElement>
{
	Q_OBJECT

public:
	typedef icmpstr::TObjectShapeBase<QGraphicsRectItem, CVisualRegistryElement> BaseClass;

	CRegistryElementShape(
				CVisualRegistryEditorComp* registryViewPtr,
				const icmpstr::ISceneProvider* providerPtr = NULL);

	QRectF GetViewRect() const;

	/**
		Check consistency, if it is changed do update.
	*/
	void CheckConsistency();

	// reimplemented (QGraphicsItem)
	virtual QRectF boundingRect() const override;
	virtual void paint(QPainter* painterPtr, const QStyleOptionGraphicsItem* stylePtr, QWidget* widgetPtr) override;
	virtual void mouseMoveEvent(QGraphicsSceneMouseEvent* eventPtr) override;
	virtual void mouseDoubleClickEvent(QGraphicsSceneMouseEvent* eventPtr) override;

	// reimplemented (imod::IObserver)
	virtual bool OnModelAttached(imod::IModel* modelPtr, istd::IChangeable::ChangeSet& changeMask) override;

Q_SIGNALS:
	void RectChanged(QRectF rect);
	void SelectionChanged(bool state);

protected:
	enum
	{
		SIDE_OFFSET = 4,
		SHADOW_OFFSET = 10
	};

	void CalcExportedInteraces(const CVisualRegistryElement& element);

	// reimplemented (icmpstr::TObjectShapeBase)
	virtual void UpdateGraphicsItem(const CVisualRegistryElement& element) override;

	// reimplemented (TShapeBase)
	virtual void OnSelectionChanged(bool isSelected) override;

private:
	CVisualRegistryEditorComp& m_registryView;

	QStringList m_exportedInterfacesList;

	QPointF m_lastClickPosition;

	// shadows
	QString m_addressString;
	QIcon m_icon;
	QColor m_backgroundColor;
	QColor m_backgroundSelectedColor;
	bool m_isConsistent;
	int m_componentType;
};


} // namespace icmpstr


#endif // !icmpstr_CRegistryElementShape_included


