#include "iqtcam/CMultiBitmapSupplierGuiComp.h"


// Qt includes
#include <QtCore/QDateTime>
// Qt includes
#include<QtCore/QtGlobal>
#if QT_VERSION >= 0x050000
#include <QtWidgets/QMessageBox>
#else
#include <QtGui/QMessageBox>
#endif

// ACF includes
#include "iview/CImageShape.h"

namespace iqtcam
{


// protected slots

CMultiBitmapSupplierGuiComp::CMultiBitmapSupplierGuiComp()
{
	m_timer.setInterval(40);
	QObject::connect(&m_timer, SIGNAL(timeout()), this, SLOT(OnTimerReady()));
}


void CMultiBitmapSupplierGuiComp::on_SnapImageButton_clicked()
{
	iproc::ISupplier* supplierPtr = GetObjectPtr();
	if (supplierPtr != NULL){
		supplierPtr->InvalidateSupplier();
		supplierPtr->EnsureWorkInitialized();
		supplierPtr->EnsureWorkFinished();

		if (supplierPtr->GetWorkStatus() >= iproc::ISupplier::WS_ERROR){
			SizeLabel->setText("Snap Error");
		}
	}
}


void CMultiBitmapSupplierGuiComp::on_LiveImageButton_toggled(bool checked)
{
	if (checked){
		m_timer.start();
	}
	else{
		m_timer.stop();
	}
}


void CMultiBitmapSupplierGuiComp::on_SaveImageButton_clicked()
{
	if (m_bitmapLoaderCompPtr.IsValid()){
		QString newFilePath;

		if (m_filePathFormatAttrPtr.IsValid()){
			// prepare a new filename
			QDateTime currentDateTime = QDateTime::currentDateTime();
			QString dateText = currentDateTime.toString("yyyyMMdd");
			QString timeText = currentDateTime.toString("hhmmss");
			QString channelndexText = QString::number(IconsView->currentRow());

			istd::CIndex2d bitmapSize = m_bitmap.GetImageSize();
			QString resolutionText = QString("(%1x%2)").arg(bitmapSize.GetX()).arg(bitmapSize.GetY());

			newFilePath = (*m_filePathFormatAttrPtr).arg(dateText).arg(timeText).arg(channelndexText).arg(resolutionText);
		}

		if (m_bitmapLoaderCompPtr->SaveToFile(m_bitmap, newFilePath) == ifile::IFilePersistence::OS_FAILED){
			QMessageBox::warning(
						GetQtWidget(),
						QObject::tr("Error"),
						QObject::tr("Cannot save image"));
		}
		else{
			SaveImageButton->setEnabled(false);
		}
	}
}


void CMultiBitmapSupplierGuiComp::on_LoadParamsButton_clicked()
{
	LoadParams();
}


void CMultiBitmapSupplierGuiComp::on_SaveParamsButton_clicked()
{
	SaveParams();
}


void CMultiBitmapSupplierGuiComp::on_IconsView_currentItemChanged(QListWidgetItem* current, QListWidgetItem* /*previous*/)
{
	QVariant bitmapIndex = current->data(Qt::UserRole);
	if (!bitmapIndex.isNull()){
		SelectBitmap(bitmapIndex.toInt());
		SaveImageButton->setEnabled(true);

		// display image size
		istd::CIndex2d bitmapSize = m_bitmap.GetImageSize();
		SizeLabel->setText(tr("(%1 x %2)").arg(bitmapSize.GetX()).arg(bitmapSize.GetY()));
	}
}


void CMultiBitmapSupplierGuiComp::OnTimerReady()
{
	on_SnapImageButton_clicked();
}


// protected methods

// reimplemented (iqtgui::CGuiComponentBase)

void CMultiBitmapSupplierGuiComp::OnGuiCreated()
{
	BaseClass::OnGuiCreated();

	SaveImageButton->setVisible(m_bitmapLoaderCompPtr.IsValid());
	IconsView->setIconSize(QSize(*m_iconSizeAttrPtr, *m_iconSizeAttrPtr));
}


void CMultiBitmapSupplierGuiComp::OnGuiHidden()
{
	LiveImageButton->setChecked(false);

	BaseClass::OnGuiHidden();
}


// reimplemented (iqtinsp::TSupplierGuiCompBase)

QWidget* CMultiBitmapSupplierGuiComp::GetParamsWidget() const
{
	Q_ASSERT(IsGuiCreated());

	return ParamsFrame;
}


// reimplemented (iqt2d::TViewExtenderCompBase)

void CMultiBitmapSupplierGuiComp::CreateShapes(int /*sceneId*/, Shapes& result)
{
	iview::CImageShape* shapePtr = new iview::CImageShape;
	if (shapePtr != NULL){
		shapePtr->AssignToLayer(iview::IViewLayer::LT_BACKGROUND);

		result.PushBack(shapePtr);

		m_bitmap.AttachObserver(shapePtr);
	}
}


// reimplemented (iqtgui::TGuiObserverWrap)

void CMultiBitmapSupplierGuiComp::OnGuiModelAttached()
{
	BaseClass::OnGuiModelAttached();

	ParamsGB->setVisible(AreParamsEditable() || IsLoadParamsSupported());

	LoadParamsButton->setVisible(IsLoadParamsSupported());
	SaveParamsButton->setVisible(IsSaveParamsSupported());
}


void CMultiBitmapSupplierGuiComp::UpdateGui(int updateFlags)
{
	BaseClass::UpdateGui(updateFlags);

	Q_ASSERT(IsGuiCreated());

	IconsView->reset();
	IconsView->clear();
	m_icons.clear();

	iimg::IMultiBitmapProvider* providerPtr = dynamic_cast<iimg::IMultiBitmapProvider*>(GetObjectPtr());
	if (providerPtr == NULL){
		return;
	}

	const iprm::IOptionsList* selectionConstraintsPtr = providerPtr->GetBitmapListInfo();

	int bitmapsCount = providerPtr->GetBitmapsCount();

	// create bitmap thumbnails
	for (int bitmapIndex = 0; bitmapIndex < bitmapsCount; bitmapIndex++){
		const iimg::CBitmap* bitmapPtr = dynamic_cast<const iimg::CBitmap*> (providerPtr->GetBitmap(bitmapIndex));
		if (bitmapPtr != NULL){
			const QImage& image = bitmapPtr->GetQImage();
			QPixmap iconPixmap;

#if QT_VERSION >= 0x040700
			iconPixmap.convertFromImage(image);
#else
			iconPixmap.fromImage(image);
#endif
			m_icons.push_back(QIcon(iconPixmap.scaled(*m_iconSizeAttrPtr, *m_iconSizeAttrPtr, Qt::KeepAspectRatio)));

			QString iconText = QObject::tr("Channel %1").arg(bitmapIndex + 1);
			if ((selectionConstraintsPtr != NULL) && (bitmapIndex < selectionConstraintsPtr->GetOptionsCount())){
				iconText = selectionConstraintsPtr->GetOptionName(bitmapIndex);
			}

			QListWidgetItem* newItem = new QListWidgetItem(m_icons.back(), iconText, IconsView, 0);
			newItem->setData(Qt::UserRole, bitmapIndex); // store a number to be used for bitmap selection
			IconsView->addItem(newItem);
		}
	}

	istd::CIndex2d bitmapSize = m_bitmap.GetImageSize();
	if (bitmapsCount > 0){
		if (IconsView->selectedItems().empty()){
			SizeLabel->setText("");
		}
		else{
			SizeLabel->setText(tr("(%1 x %2)").arg(bitmapSize.GetX()).arg(bitmapSize.GetY()));
		}
	}
	else{
		SizeLabel->setText(tr("No image"));
	}

	SaveImageButton->setEnabled(!bitmapSize.IsSizeEmpty());

	UpdateAllViews();
}


void CMultiBitmapSupplierGuiComp::SelectBitmap(int bitmapIndex)
{
	const iimg::IBitmap* bitmapPtr = NULL;

	iimg::IMultiBitmapProvider* providerPtr = dynamic_cast<iimg::IMultiBitmapProvider*>(GetObjectPtr());
	if (providerPtr != NULL){
		bitmapPtr = providerPtr->GetBitmap(bitmapIndex);
	}

	if ((bitmapPtr == NULL) || !m_bitmap.CopyFrom(*bitmapPtr)){
		m_bitmap.ResetImage();
	}

	istd::CIndex2d imageSize = m_bitmap.GetImageSize();
	i2d::CRectangle imageBox(0, 0, imageSize.GetX(), imageSize.GetY());

	const ShapesMap& shapesMap = GetShapesMap();
	QSet<iqt2d::IViewProvider*> views = shapesMap.keys().toSet();
	for (QSet<iqt2d::IViewProvider*>::ConstIterator viewIter = views.begin();
			viewIter != views.end();
			++viewIter){
		iqt2d::IViewProvider* viewProviderPtr = *viewIter;
		Q_ASSERT(viewProviderPtr != NULL);

		iview::CViewBase* viewPtr = dynamic_cast<iview::CViewBase*> (viewProviderPtr->GetView());
		if (viewPtr != NULL){
			viewPtr->SetFitArea(imageBox);
		}
	}
}


// reimplemented (imod::IObserver)

} // namespace iqtcam


