#include "iqtcam/CBitmapSupplierGuiComp.h"


// Qt includes
#include <QtGui/QMessageBox>

// ACF includes
#include "iproc/ISupplier.h"
#include "iview/CImageShape.h"
#include "iview/CViewBase.h"
#include "icam/ISnapControl.h"


namespace iqtcam
{


// protected slots

CBitmapSupplierGuiComp::CBitmapSupplierGuiComp()
{
	QObject::connect(&m_timer, SIGNAL(timeout()), this, SLOT(OnTimerReady()));
}


// reimplemented (iimg::IBitmapProvider)

const iimg::IBitmap* CBitmapSupplierGuiComp::GetBitmap() const
{
	return &m_bitmap;
}


// protected slots

void CBitmapSupplierGuiComp::on_SnapImageButton_clicked()
{
	icam::ISnapControl* snapControlPtr = dynamic_cast<icam::ISnapControl*>(GetObjectPtr());
	if (snapControlPtr != NULL){
		snapControlPtr->SetSnapDirection(icam::ISnapControl::SD_FORWARD);
	}

	// snap with UI message by default
	DoSnap();
}


void CBitmapSupplierGuiComp::on_SnapBackImageButton_clicked()
{
	icam::ISnapControl* snapControlPtr = dynamic_cast<icam::ISnapControl*>(GetObjectPtr());
	if (snapControlPtr != NULL){
		snapControlPtr->SetSnapDirection(icam::ISnapControl::SD_BACK);

		DoSnap();
	}
}


void CBitmapSupplierGuiComp::on_LiveImageButton_toggled(bool checked)
{
	if (checked){
		m_timer.setInterval(*m_snapIntervalAttrPtr);

		m_timer.start();
	}
	else{
		m_timer.stop();
	}
}


void CBitmapSupplierGuiComp::on_SaveImageButton_clicked()
{
	if (		m_bitmapLoaderCompPtr.IsValid() &&
				m_bitmapLoaderCompPtr->SaveToFile(m_bitmap, "") == ifile::IFilePersistence::OS_FAILED){
		QMessageBox::warning(
					GetQtWidget(),
					QObject::tr("Error"),
					QObject::tr("Cannot save image"));
	}
}


void CBitmapSupplierGuiComp::on_LoadParamsButton_clicked()
{
	LoadParams();
}


void CBitmapSupplierGuiComp::on_SaveParamsButton_clicked()
{
	SaveParams();
}


void CBitmapSupplierGuiComp::OnTimerReady()
{
	DoSnap(true);
}


// protected methods

void CBitmapSupplierGuiComp::DoSnap(bool noGui)
{
	iproc::ISupplier* supplierPtr = GetObjectPtr();
	if (supplierPtr != NULL){
		supplierPtr->InvalidateSupplier();
		supplierPtr->EnsureWorkInitialized();
		supplierPtr->EnsureWorkFinished();

		if (supplierPtr->GetWorkStatus() >= iproc::ISupplier::WS_ERROR){
			if (noGui){
				SendCriticalMessage(0, QObject::tr("Snap Error"));
			} else {
				QMessageBox::warning(
					GetQtWidget(),
					QObject::tr("Error"),
					QObject::tr("Snap Error"));
			}
		}
	}
}


// reimplemented (iqtgui::CGuiComponentBase)

void CBitmapSupplierGuiComp::OnGuiCreated()
{
	BaseClass::OnGuiCreated();

	SaveImageButton->setVisible(m_bitmapLoaderCompPtr.IsValid());

	icam::ISnapControl* snapControlPtr = dynamic_cast<icam::ISnapControl*>(GetObjectPtr());
	SnapBackImageButton->setVisible(snapControlPtr != NULL);
}


void CBitmapSupplierGuiComp::OnGuiHidden()
{
	LiveImageButton->setChecked(false);

	BaseClass::OnGuiHidden();
}


// reimplemented (iqtinsp::TSupplierGuiCompBase)

QWidget* CBitmapSupplierGuiComp::GetParamsWidget() const
{
	Q_ASSERT(IsGuiCreated());

	return ParamsFrame;
}


// reimplemented (iqt2d::TViewExtenderCompBase)

void CBitmapSupplierGuiComp::CreateShapes(int /*sceneId*/, Shapes& result)
{
	iview::CImageShape* shapePtr = new iview::CImageShape;
	if (shapePtr != NULL){
		shapePtr->AssignToLayer(iview::IViewLayer::LT_BACKGROUND);

		result.PushBack(shapePtr);

		m_bitmap.AttachObserver(shapePtr);
	}
}


// reimplemented (iqtgui::TGuiObserverWrap)

void CBitmapSupplierGuiComp::OnGuiModelAttached()
{
	BaseClass::OnGuiModelAttached();

	ParamsFrame->setVisible(AreParamsEditable());

	LoadParamsButton->setVisible(IsLoadParamsSupported());
	SaveParamsButton->setVisible(IsSaveParamsSupported());
}


void CBitmapSupplierGuiComp::UpdateGui(int updateFlags)
{
	BaseClass::UpdateGui(updateFlags);

	Q_ASSERT(IsGuiCreated());

	istd::CIndex2d bitmapSize = m_bitmap.GetImageSize();

	SizeLabel->setText(tr("(%1 x %2)").arg(bitmapSize.GetX()).arg(bitmapSize.GetY()));
	SaveImageButton->setEnabled(!bitmapSize.IsSizeEmpty());

	UpdateAllViews();
}


// reimplemented (imod::IObserver)

void CBitmapSupplierGuiComp::AfterUpdate(imod::IModel* modelPtr, int updateFlags, istd::IPolymorphic* updateParamsPtr)
{
	const iimg::IBitmap* bitmapPtr = NULL;

	iimg::IBitmapProvider* providerPtr = dynamic_cast<iimg::IBitmapProvider*>(GetObjectPtr());
	if (providerPtr != NULL){
		bitmapPtr = providerPtr->GetBitmap();
	}

	if ((bitmapPtr == NULL) || !m_bitmap.CopyFrom(*bitmapPtr)){
		m_bitmap.ResetImage();
	}

	istd::CIndex2d imageSize = m_bitmap.GetImageSize();
	i2d::CRectangle imageBox(0, 0, imageSize.GetX(), imageSize.GetY());

	const ShapesMap& shapesMap = GetShapesMap();
	QSet<iqt2d::IViewProvider*> views = shapesMap.keys().toSet();
	for (		QSet<iqt2d::IViewProvider*>::ConstIterator viewIter = views.begin();
				viewIter != views.end();
				++viewIter){
		iqt2d::IViewProvider* viewProviderPtr = *viewIter;
		Q_ASSERT(viewProviderPtr != NULL);

		iview::CViewBase* viewPtr = dynamic_cast<iview::CViewBase*>(viewProviderPtr->GetView());
		if (viewPtr != NULL){
			viewPtr->SetFitArea(imageBox);
		}
	}

	BaseClass::AfterUpdate(modelPtr, updateFlags, updateParamsPtr);
}


} // namespace iqtcam


