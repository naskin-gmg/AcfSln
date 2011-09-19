#include "iqtfpf/CHotfolderContainerGuiComp.h"


// Qt includes
#include <QVBoxLayout>
#include <QMouseEvent>
#include <QApplication>
#include <QHeaderView>
#include <QItemDelegate>
#include <QBitmap>
#include <QPushButton>
#include <QPainter>
#include <QMenu>


// ACF includes
#include "istd/CString.h"

#include "iser/CMemoryReadArchive.h"
#include "iser/CMemoryWriteArchive.h"

#include "iqt/CSignalBlocker.h"


namespace iqtfpf
{


// protected methods

QPixmap CHotfolderContainerGuiComp::CreateComponentDragPixmap(const istd::CString& /*hotfolderId*/) const
{
	/*
	QPushButton componentLabel(NULL);
	componentLabel.setFlat(true);
	QFont font = componentLabel.font();
	font.setBold(true);
	font.setPointSize(12);
	componentLabel.setFont(font);
	componentLabel.setText(iqt::GetQString(address.ToString()));
	if (m_consistInfoCompPtr.IsValid()){
		componentLabel.setIconSize(QSize(64, 64));
		componentLabel.setIcon(m_consistInfoCompPtr->GetComponentIcon(address));
	}
	componentLabel.adjustSize();

	QPixmap pixmap = QPixmap::grabWidget(&componentLabel);
	QPalette palette = componentLabel.palette();

	pixmap.setMask(pixmap.createMaskFromColor(palette.color(QPalette::Window), Qt::MaskInColor));
	
	QPainter painter(&pixmap);
	painter.setPen(Qt::DashDotLine);
	QRectF mainRect = pixmap.rect();
	mainRect.adjust(0, 0, -1, -1);
	painter.drawRect(mainRect);
	painter.end();

	return pixmap;

	*/

	return QPixmap();
}


// reimplemented (QObject)

bool CHotfolderContainerGuiComp::eventFilter(QObject* sourcePtr, QEvent* eventPtr)
{
	QWidget* sourceWidgetPtr = dynamic_cast<QWidget*>(sourcePtr);
	I_ASSERT(sourceWidgetPtr != NULL);

	switch (eventPtr->type()){
		case QEvent::MouseButtonPress:{
			QMouseEvent* mouseEvent = dynamic_cast<QMouseEvent*>(eventPtr);
			I_ASSERT(mouseEvent != NULL);

			if (mouseEvent->button() == Qt::LeftButton && HotfolderList != NULL){
				QListWidgetItem* selectedItemPtr = NULL;

				QModelIndex componentModelIndex = HotfolderList->indexAt(mouseEvent->pos());
				selectedItemPtr = dynamic_cast<QListWidgetItem*>(reinterpret_cast<QListWidgetItem*>(componentModelIndex.internalPointer()));

				if (selectedItemPtr != NULL){
					QMimeData* mimeData = new QMimeData;

					istd::CString hotfolderId = iqt::GetCString(selectedItemPtr->text());
					iser::CMemoryWriteArchive archive;
					if (archive.Process(hotfolderId)){
						QByteArray byteData = QByteArray((const char*)archive.GetBuffer(), archive.GetBufferSize());

						mimeData->setData("hotfolder", byteData);

						QDrag *drag = new QDrag(sourceWidgetPtr);
						drag->setMimeData(mimeData);
						drag->setPixmap(CreateComponentDragPixmap(hotfolderId));
						drag->setHotSpot(QPoint(drag->pixmap().width()/2, drag->pixmap().height()));
						drag->start(Qt::MoveAction);
					}
				}
			}
		}
		break;

	default:
		break;
	}

	return BaseClass::eventFilter(sourcePtr, eventPtr);
}


// reimplemented (CGuiComponentBase)

void CHotfolderContainerGuiComp::OnGuiCreated()
{
	BaseClass::OnGuiCreated();	

	HotfolderList->viewport()->installEventFilter(this);

	HotfolderList->setContextMenuPolicy( Qt::CustomContextMenu);

	if (m_hotoflderFactoryIdsAttrPtr.IsValid()){
		for (int typeIndex = 0; typeIndex < m_hotoflderFactoryIdsAttrPtr.GetCount(); typeIndex++){
			QString itemText = iqt::GetQString(m_hotoflderFactoryIdsAttrPtr[typeIndex]);

			HotfolderList->addItem(itemText);
		}
	}

}


} // namespace iqtfpf


