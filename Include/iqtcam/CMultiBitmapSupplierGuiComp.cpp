#include <iqtcam/CMultiBitmapSupplierGuiComp.h>


// Qt includes
#include <QtCore/QDateTime>
#include<QtCore/QtGlobal>
#if QT_VERSION >= 0x050000
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QFileDialog>
#else
#include <QtGui/QMessageBox>
#include <QtGui/QFileDialog>
#endif

// ACF includes
#include <iview/CImageShape.h>


namespace iqtcam
{


// public methods

CMultiBitmapSupplierGuiComp::CMultiBitmapSupplierGuiComp()
{
	m_timer.setInterval(40);

	QObject::connect(&m_timer, SIGNAL(timeout()), this, SLOT(OnTimerReady()));
}


void CMultiBitmapSupplierGuiComp::on_SnapImageButton_clicked()
{
	iinsp::ISupplier* supplierPtr = GetObservedObject();
	if (supplierPtr != NULL){
		istd::CChangeGroup changeGroup(supplierPtr);

		supplierPtr->InvalidateSupplier();
		supplierPtr->EnsureWorkInitialized();
		supplierPtr->EnsureWorkFinished();

		if (supplierPtr->GetWorkStatus() == iinsp::ISupplier::WS_FAILED){
			SizeLabel->setText("Snap Error");
		}

		iimg::IMultiBitmapProvider* providerPtr = dynamic_cast<iimg::IMultiBitmapProvider*>(GetObservedObject());
		if (providerPtr != NULL){
			m_bitmapDocument.CopyFrom(*providerPtr);
		}

		if (ContiniousSaveSelectedImageButton->isChecked()){
			ExportSelectedBitmap();
		}

		if (ContiniousSaveBitmapDocumentButton->isChecked()){
			ExportBitmapDocument();
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


void CMultiBitmapSupplierGuiComp::on_LoadParamsButton_clicked()
{
	LoadParams();
}


void CMultiBitmapSupplierGuiComp::on_SaveParamsButton_clicked()
{
	SaveParams();
}


void CMultiBitmapSupplierGuiComp::on_BitmapPreview_currentItemChanged(QListWidgetItem* current, QListWidgetItem* /*previous*/)
{
	QVariant bitmapIndex = current->data(Qt::UserRole);
	if (!bitmapIndex.isNull()){
		SelectBitmap(bitmapIndex.toInt());

		// display image size
		istd::CIndex2d bitmapSize = m_bitmap.GetImageSize();
		SizeLabel->setText(tr("(%1 x %2)").arg(bitmapSize.GetX()).arg(bitmapSize.GetY()));
	}
}


void CMultiBitmapSupplierGuiComp::OnTimerReady()
{
	on_SnapImageButton_clicked();
}


void CMultiBitmapSupplierGuiComp::on_ExportSelectedImageButton_clicked()
{
	Q_ASSERT(m_singleBitmapPersistenceCompPtr.IsValid());

	QString fileFilter = CreateFileFilterForPersistence(*m_singleBitmapPersistenceCompPtr.GetPtr());

	QString filePath = QFileDialog::getSaveFileName(GetWidget(), tr("Save selected image..."), "", fileFilter);
	if (!filePath.isEmpty()){
		ExportSelectedBitmap(filePath);
	}
}


void CMultiBitmapSupplierGuiComp::on_ExportBitmapDocumentButton_clicked()
{
	Q_ASSERT(m_multiBitmapPersistenceCompPtr.IsValid());

	QString fileFilter = CreateFileFilterForPersistence(*m_multiBitmapPersistenceCompPtr.GetPtr());

	QString filePath = QFileDialog::getSaveFileName(GetWidget(), tr("Save selected image..."), "", fileFilter);
	if (!filePath.isEmpty()){
		ExportBitmapDocument(filePath);
	}
}


// protected methods

// reimplemented (iqtgui::CGuiComponentBase)

void CMultiBitmapSupplierGuiComp::OnGuiCreated()
{
	BaseClass::OnGuiCreated();

	ContiniousSaveSelectedImageButton->setVisible(m_singleBitmapPersistenceCompPtr.IsValid());
	ExportSelectedImageButton->setVisible(m_singleBitmapPersistenceCompPtr.IsValid());

	BitmapPreview->setIconSize(QSize(*m_iconSizeAttrPtr, *m_iconSizeAttrPtr));

	if (*m_iconSizeAttrPtr == 0){
		BitmapPreview->setVisible(false);
	}
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


void CMultiBitmapSupplierGuiComp::UpdateGui(const istd::IChangeable::ChangeSet& changeSet)
{
	BaseClass::UpdateGui(changeSet);

	Q_ASSERT(IsGuiCreated());

	BitmapPreview->reset();
	BitmapPreview->clear();
	m_icons.clear();

	iimg::IMultiBitmapProvider* providerPtr = dynamic_cast<iimg::IMultiBitmapProvider*>(GetObservedObject());
	if (providerPtr == NULL){
		return;
	}

	const iprm::IOptionsList* selectionConstraintsPtr = providerPtr->GetBitmapListInfo();

	int bitmapsCount = providerPtr->GetBitmapsCount();

// 	BitmapPreview->setGridSize(QSize(1, bitmapsCount));

	// create bitmap thumbnails
	for (int bitmapIndex = 0; bitmapIndex < bitmapsCount; bitmapIndex++){
		const iimg::IBitmap* bitmapPtr = providerPtr->GetBitmap(bitmapIndex);
		if (bitmapPtr != NULL){
			QImage previewImage;
			const iimg::CBitmap* qtBitmapPtr = dynamic_cast<const iimg::CBitmap*>(bitmapPtr);
			if (qtBitmapPtr != NULL){
				previewImage = qtBitmapPtr->GetQImage();
			}
			else{
				iimg::CBitmap bitmap;

				bitmap.CopyFrom(*bitmapPtr, istd::IChangeable::CM_CONVERT);

				previewImage = bitmap.GetQImage();
			}
			
			QPixmap iconPixmap;

#if QT_VERSION >= 0x040700
			iconPixmap.convertFromImage(previewImage);
#else
			iconPixmap.fromImage(image);
#endif
			m_icons.push_back(QIcon(iconPixmap.scaled(*m_iconSizeAttrPtr, *m_iconSizeAttrPtr, Qt::KeepAspectRatio)));

			QString iconText = QObject::tr("Channel %1").arg(bitmapIndex + 1);
			if ((selectionConstraintsPtr != NULL) && (bitmapIndex < selectionConstraintsPtr->GetOptionsCount())){
				iconText = selectionConstraintsPtr->GetOptionName(bitmapIndex);
			}

			QListWidgetItem* newItem = new QListWidgetItem(m_icons.back(), iconText, BitmapPreview, 0);
			newItem->setData(Qt::UserRole, bitmapIndex); // store a number to be used for bitmap selection
			BitmapPreview->addItem(newItem);
		}
	}

	QList<QListWidgetItem*> selectedItems = BitmapPreview->selectedItems();
	if (!selectedItems.isEmpty()){
		SelectBitmap(selectedItems.indexOf(0));
	}
	else if (bitmapsCount > 0){
		SelectBitmap(0);
	}

	istd::CIndex2d bitmapSize = m_bitmap.GetImageSize();
	if (!bitmapSize.IsSizeEmpty()){
		SizeLabel->setText(tr("(%1 x %2)").arg(bitmapSize.GetX()).arg(bitmapSize.GetY()));
	}
	else{
		SizeLabel->setText(tr("No image"));
	}

	UpdateAllViews();

	UpdateCommands();
}


void CMultiBitmapSupplierGuiComp::SelectBitmap(int bitmapIndex)
{
	const iimg::IBitmap* bitmapPtr = NULL;

	iimg::IMultiBitmapProvider* providerPtr = dynamic_cast<iimg::IMultiBitmapProvider*>(GetObservedObject());
	if (providerPtr != NULL){
		bitmapPtr = providerPtr->GetBitmap(bitmapIndex);
	}

	if ((bitmapPtr == NULL) || !m_bitmap.CopyFrom(*bitmapPtr, istd::IChangeable::CM_CONVERT)){
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

	UpdateCommands();
}


void CMultiBitmapSupplierGuiComp::ExportSelectedBitmap(const QString& filePath)
{
	if (m_singleBitmapPersistenceCompPtr.IsValid()){
		QString newFilePath = filePath;

		if (newFilePath.isEmpty()){
			// Prepare a new filename
			QDateTime currentDateTime = QDateTime::currentDateTime();
			QString dateText = currentDateTime.toString("yyyyMMdd");
			QString timeText = currentDateTime.toString("hhmmsszzz");

			int imageIndex = BitmapPreview->currentRow();
			QString channelndexText;
			if (imageIndex >= 0){
				channelndexText = QString::number(BitmapPreview->currentRow());
			}

			istd::CIndex2d bitmapSize = m_bitmap.GetImageSize();
			QString resolutionText = QString("(%1x%2)").arg(bitmapSize.GetX()).arg(bitmapSize.GetY());

			newFilePath = (*m_filePathFormatAttrPtr).arg(dateText).arg(timeText).arg(channelndexText).arg(resolutionText);
		}

		m_singleBitmapPersistenceCompPtr->SaveToFile(m_bitmap, newFilePath);
	}
}


void CMultiBitmapSupplierGuiComp::ExportBitmapDocument(const QString& filePath)
{
	if (m_multiBitmapPersistenceCompPtr.IsValid()){
		QString newFilePath = filePath;

		if (newFilePath.isEmpty()){
			// Prepare a new filename
			QDateTime currentDateTime = QDateTime::currentDateTime();
			QString dateText = currentDateTime.toString("dd.MM.yyyy");
			QString timeText = currentDateTime.toString("hh_mm_ss_zzz");

			newFilePath = dateText + timeText;
		}

		m_multiBitmapPersistenceCompPtr->SaveToFile(m_bitmapDocument, newFilePath);
	}
}


void CMultiBitmapSupplierGuiComp::UpdateCommands()
{
	ContiniousSaveSelectedImageButton->setEnabled(!m_bitmap.IsEmpty());
	ExportSelectedImageButton->setEnabled(!m_bitmap.IsEmpty());
}


// private static methods

QString CMultiBitmapSupplierGuiComp::CreateFileFilterForPersistence(const ifile::IFilePersistence& persistence)
{
	QStringList extensions;
	persistence.GetFileExtensions(extensions);

	QString fileFilter = "Image files(";
	for (int i = 0; i < extensions.count(); ++i){
		fileFilter += "*." + extensions[i];

		if (i != extensions.count() - 1){
			fileFilter += ";";
		}

	}

	fileFilter += ")";

	return fileFilter;
}


} // namespace iqtcam


