#include "iqtcam/CBitmapSupplierGuiComp.h"


// Qt includes
#include <QMessageBox>

// ACF includes
#include "iproc/ISupplier.h"
#include "iqt2d/CImageShape.h"


namespace iqtcam
{


// protected slots

void CBitmapSupplierGuiComp::on_SnapImageButton_clicked()
{
	iproc::ISupplier* supplierPtr = GetObjectPtr();
	if (supplierPtr != NULL){
		supplierPtr->InvalidateSupplier();
		supplierPtr->EnsureWorkFinished();

		if (supplierPtr->GetWorkStatus() >= iproc::ISupplier::WS_ERROR){
			QMessageBox::information(
						NULL,
						QObject::tr("Error"),
						QObject::tr("Snap Error"));
		}
	}
}


void CBitmapSupplierGuiComp::on_SaveImageButton_clicked()
{
	if (		m_bitmapLoaderCompPtr.IsValid() &&
				m_bitmapLoaderCompPtr->SaveToFile(m_bitmap, "") == iser::IFileLoader::StateFailed){
		QMessageBox::information(
					NULL,
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


// protected methods

// reimplemented (iqtgui::CGuiComponentBase)

void CBitmapSupplierGuiComp::OnGuiCreated()
{
	BaseClass::OnGuiCreated();

	SaveImageButton->setVisible(m_bitmapLoaderCompPtr.IsValid());
}


// reimplemented (iqtinsp::TSupplierGuiCompBase)

QWidget* CBitmapSupplierGuiComp::GetParamsWidget() const
{
	I_ASSERT(IsGuiCreated());

	return ParamsFrame;
}


// reimplemented (iqt2d::TSceneExtenderCompBase)

void CBitmapSupplierGuiComp::CreateShapes(int /*sceneId*/, bool /*inactiveOnly*/, Shapes& result)
{
	iqt2d::CImageShape* shapePtr = new iqt2d::CImageShape;
	if (shapePtr != NULL){
		result.PushBack(shapePtr);

		m_bitmap.AttachObserver(shapePtr);
	}
}


// reimplemented (iqtgui::TGuiObserverWrap)

void CBitmapSupplierGuiComp::OnGuiModelAttached()
{
	BaseClass::OnGuiModelAttached();

	ParamsGB->setVisible(AreParamsEditable() || IsLoadParamsSupported());

	LoadParamsButton->setVisible(IsLoadParamsSupported());
	SaveParamsButton->setVisible(IsSaveParamsSupported());
}


void CBitmapSupplierGuiComp::UpdateGui(int /*updateFlags*/)
{
	I_ASSERT(IsGuiCreated());

	istd::CIndex2d bitmapSize = m_bitmap.GetImageSize();

	SizeLabel->setText(tr("(%1 x %2)").arg(bitmapSize.GetX()).arg(bitmapSize.GetY()));
	SaveImageButton->setEnabled(!bitmapSize.IsSizeEmpty());
}



// reimplemented (imod::IObserver)

void CBitmapSupplierGuiComp::AfterUpdate(imod::IModel* modelPtr, int updateFlags, istd::IPolymorphic* updateParamsPtr)
{
	iproc::ISupplier* supplierPtr = GetObjectPtr();
	if (supplierPtr != NULL){
		const iimg::IBitmap* bitmapPtr = NULL;

		int workStatus = supplierPtr->GetWorkStatus();
		if (workStatus == iproc::ISupplier::WS_OK){
			iipr::IBitmapProvider* providerPtr = dynamic_cast<iipr::IBitmapProvider*>(supplierPtr);
			if (providerPtr != NULL){
				bitmapPtr = providerPtr->GetBitmap();
			}
		}

		if ((bitmapPtr == NULL) || !m_bitmap.CopyFrom(*bitmapPtr)){
			m_bitmap.ResetImage();
		}
	}
	else{
		m_bitmap.ResetImage();
	}

	BaseClass::AfterUpdate(modelPtr, updateFlags, updateParamsPtr);
}


} // namespace iqtcam


