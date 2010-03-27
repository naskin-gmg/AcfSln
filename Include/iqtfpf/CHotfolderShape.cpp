#include "iqtfpf/CHotfolderShape.h"


// Qt includes
#include <QPainter>
#include <QFont>
#include <QFontMetrics>
#include <QStyleOptionGraphicsItem>
#include <QGraphicsScene>
#include <QApplication>
#include <QDir>


// ACF includes
#include "iqtfpf/CHotfolderWorkflowScenographerComp.h"
#include "iqtfpf/CHotfolderShape.h"


namespace icmpstr
{


CHotfolderShape::CHotfolderShape(
			const iqt2d::ISceneProvider* providerPtr)
:	BaseClass(true, providerPtr)
{
}


QRectF CHotfolderShape::GetViewRect() const
{
	QRectF retVal = rect();
	retVal.moveCenter(pos());

	return retVal;
}


// reimplemented (QGraphicsItem)

QRectF CHotfolderShape::boundingRect() const
{
	QRectF retVal = rect();

	retVal.adjust(0, 0, SHADOW_OFFSET, SHADOW_OFFSET);

	return retVal;
}


void CHotfolderShape::paint(QPainter* painterPtr, const QStyleOptionGraphicsItem* /*stylePtr*/, QWidget* /*widgetPtr*/)
{

}


void CHotfolderShape::mouseMoveEvent(QGraphicsSceneMouseEvent* eventPtr)
{
	BaseClass::mouseMoveEvent(eventPtr);

	Q_EMIT RectChanged(GetViewRect());
}


void CHotfolderShape::mouseDoubleClickEvent(QGraphicsSceneMouseEvent* eventPtr)
{
	BaseClass::mouseDoubleClickEvent(eventPtr);
}


// reimplemented (imod::IObserver)

bool CHotfolderShape::OnAttached(imod::IModel* modelPtr)
{
	const iprm::IParamsSet* objectPtr = dynamic_cast<const iprm::IParamsSet*>(modelPtr);
	if (objectPtr == NULL){
		return false;
	}

	return BaseClass::OnAttached(modelPtr);
}


// protected methods

// reimplemented (iqt2d::TObjectShapeBase)

void CHotfolderShape::UpdateGraphicsItem(const iprm::IParamsSet& element)
{
	
}


// reimplemented (TShapeBase)

void CHotfolderShape::OnSelectionChanged(bool isSelected)
{
	BaseClass::OnSelectionChanged(isSelected);

	Q_EMIT SelectionChanged(isSelected);
}


} // namespace icmpstr


