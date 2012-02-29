#include "iqtfpf/CHotfolderShape.h"


// Qt includes
#include <QtGui/QPainter>
#include <QtGui/QFont>
#include <QtGui/QFontMetrics>
#include <QtGui/QStyleOptionGraphicsItem>
#include <QtGui/QGraphicsScene>
#include <QtGui/QApplication>
#include <QtCore/QDir>
#include <QtGui/QGraphicsProxyWidget>
#include <QtGui/QLabel>
#include <QtGui/QGraphicsTextItem>

// ACF includes
#include "iqtfpf/CHotfolderShape.h"


namespace iqtfpf
{


CHotfolderShape::CHotfolderShape(const iqt2d::ISceneProvider* providerPtr)
:	BaseClass(true, providerPtr)
{
	m_hotfolderName = new QGraphicsTextItem(this);

	m_hotfolderName->setPlainText("SUPER TEXT");
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
	const ifpf::CVisualHotfolderWorkflowItem* objectPtr = GetObjectPtr();
	if (objectPtr == NULL){
		return;
	}

	painterPtr->setRenderHints(QPainter::Antialiasing, true);

	QRectF mainRect = rect();

	QRectF shadowRect = mainRect;
	shadowRect.adjust(SHADOW_OFFSET, SHADOW_OFFSET, SHADOW_OFFSET, SHADOW_OFFSET);

	if (isSelected()){
		painterPtr->setBrush(QColor(10, 242, 126, 50));
		painterPtr->setPen(Qt::NoPen);
		painterPtr->drawRoundedRect(shadowRect, 5, 5);

		painterPtr->setBrush( QColor(10, 242, 126, 255));
		painterPtr->drawRoundedRect(mainRect, 5, 5);
	}
	else{
		painterPtr->setBrush(QColor(0, 0, 0, 30));
		painterPtr->setPen(Qt::NoPen);
		painterPtr->drawRoundedRect(shadowRect, 5, 5);

		painterPtr->setBrush(Qt::white);
		painterPtr->drawRoundedRect(mainRect, 5, 5);
	}

	painterPtr->setPen(Qt::black);
	painterPtr->setBrush(Qt::NoBrush);

	painterPtr->drawRoundedRect(mainRect, 5, 5);

	mainRect.adjust(SIDE_OFFSET, SIDE_OFFSET, -SIDE_OFFSET, -SIDE_OFFSET);

	QString hotfolderName = objectPtr->GetName();

	// draw component name:
	QFont nameFont;
	nameFont.setPointSize(12);
	nameFont.setBold(true);
	painterPtr->setFont(nameFont);
	painterPtr->drawText(mainRect, hotfolderName, Qt::AlignTop | Qt::AlignLeft);

	QString hotfolderId = objectPtr->GetHotfolderId();
	QFont detailFont = QApplication::font();
	painterPtr->setFont(detailFont);
	painterPtr->drawText(
				mainRect,
				hotfolderId,
				Qt::AlignBottom | Qt::AlignLeft);
}


// reimplemented (imod::IObserver)

bool CHotfolderShape::OnAttached(imod::IModel* modelPtr)
{
	const ifpf::CVisualHotfolderWorkflowItem* objectPtr = dynamic_cast<const ifpf::CVisualHotfolderWorkflowItem*>(modelPtr);
	if (objectPtr == NULL){
		return false;
	}

	return BaseClass::OnAttached(modelPtr);
}


// protected methods

// reimplemented (iqt2d::TObjectShapeBase)

void CHotfolderShape::UpdateGraphicsItem(const ifpf::CVisualHotfolderWorkflowItem& element)
{
	setPos(iqt::GetQPointF(element.GetCenter()));

	QFont nameFont;
	nameFont.setPointSize(12);
	nameFont.setBold(true);
	QFont detailFont = QApplication::font();
	QFontMetrics nameFontInfo(nameFont);
	QFontMetrics detailFontInfo(detailFont);

	QStringList inputDirectories = (element.GetInputDirectories());
	QString outputDirectory = element.GetOutputDirectory();

	QString hotfolderName = element.GetName();

	int titleWidth = nameFontInfo.width(hotfolderName);
	int height = nameFontInfo.height() + detailFontInfo.height();

	int width = istd::Max(titleWidth, detailFontInfo.width(element.GetHotfolderId())) + SIDE_OFFSET * 2;
 
	width += SIDE_OFFSET * 2;
	height += SIDE_OFFSET * 2;

	double gridSize;
	const iqt2d::ISceneProvider* providerPtr = GetSceneProvider();
	if ((providerPtr != NULL) && providerPtr->GetSceneAlignment(gridSize)){
		gridSize *= 2;
		width = ::ceil(width / gridSize) * gridSize;
		height = ::ceil(height / gridSize) * gridSize;
	}

	setRect(QRectF(-width * 0.5, -height * 0.5, width, height));

	m_hotfolderName->setPos(QPointF(-width * 0.5, -height * 0.5));	
}


// reimplemented (TShapeBase)

void CHotfolderShape::OnSelectionChanged(bool isSelected)
{
	BaseClass::OnSelectionChanged(isSelected);

	Q_EMIT SelectionChanged(isSelected);
}


} // namespace iqtfpf


