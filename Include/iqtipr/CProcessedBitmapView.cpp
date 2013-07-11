#include "iqtipr/CProcessedBitmapView.h"


// Qt includes
#include <QtGui/QFileDialog>
#include <QtGui/QToolBar>

// ACF includes
#include "istd/TChangeNotifier.h"
#include "iqtgui/CCommandTools.h"


namespace iqtipr
{


CProcessedBitmapView::CProcessedBitmapView(QWidget* parent): QWidget(parent)
{
	m_ui.setupUi(this);

	m_ui.ImageView->SetZoomsVisible(true);

	m_sourceImageModel.AttachObserver(&m_sourceImageShape);
	m_sourceImageShape.AssignToLayer(iview::IViewLayer::LT_BACKGROUND);

	iview::CViewport& view = m_ui.ImageView->GetViewRef();
	view.ConnectShape(&m_sourceImageShape);

	view.SetViewDraggable(true);
	view.SetGridVisible(true);

	const ibase::IHierarchicalCommand* commandsPtr = m_ui.ImageView->GetCommands();
	if (commandsPtr != NULL){
		QToolBar* toolBarPtr = new QToolBar(m_ui.ToolBarFrame);
		m_ui.ToolBarFrame->layout()->addWidget(toolBarPtr);

		m_toolBarCommands.JoinLinkFrom(commandsPtr);
		iqtgui::CCommandTools::SetupToolbar(m_toolBarCommands, *toolBarPtr);
	}
}


void CProcessedBitmapView::SetBitmap(const iimg::IBitmap* bitmapPtr)
{
	if (bitmapPtr == NULL || bitmapPtr->IsEmpty()){
		m_ui.SaveButton->setEnabled(false);

		m_ui.WidthLabel->setText("-");
		m_ui.HeightLabel->setText("-");

		m_sourceImageModel.ResetImage();
	} else {
		m_ui.SaveButton->setEnabled(true);

		m_ui.WidthLabel->setText(QString::number(bitmapPtr->GetImageSize().GetX()));
		m_ui.HeightLabel->setText(QString::number(bitmapPtr->GetImageSize().GetY()));

		m_sourceImageModel.CopyFrom(*bitmapPtr);
	}

	istd::TChangeNotifier<iimg::CBitmap> updatePtr(&m_sourceImageModel);

	iview::CViewport& view = m_ui.ImageView->GetViewRef();
	view.UpdateAllShapes(iview::CF_TRANSFORM);
}


void CProcessedBitmapView::SetTitle(const QString& title)
{
	m_ui.GroupBox->setTitle(title);
}


// protected slots

void CProcessedBitmapView::on_SaveButton_clicked()
{
	QString imageFileName = QFileDialog::getSaveFileName(0, 
		tr("Save Image As..."), m_lastImagePath, "*.bmp");

	if (imageFileName.isEmpty() || m_sourceImageModel.GetQImageRef().save(imageFileName) == false){
		return;
	}

	m_lastImagePath = imageFileName;
}


} // namespace iqtipr
