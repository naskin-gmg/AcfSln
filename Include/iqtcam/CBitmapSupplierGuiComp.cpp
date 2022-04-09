#include <iqtcam/CBitmapSupplierGuiComp.h>


// Qt includes
#include<QtCore/QtGlobal>
#if QT_VERSION >= 0x050000
#include <QtWidgets/QMessageBox>
#else
#include <QtGui/QMessageBox>
#endif

// ACF includes
#include <iinsp/ISupplier.h>
#include <iview/CImageShape.h>
#include <iview/CViewBase.h>


namespace iqtcam
{


// public methods

CBitmapSupplierGuiComp::CBitmapSupplierGuiComp()
{
	QObject::connect(&m_timer, SIGNAL(timeout()), this, SLOT(OnTimerReady()));
}


// reimplemented (iimg::IBitmapProvider)

const iimg::IBitmap* CBitmapSupplierGuiComp::GetBitmap() const
{
	return m_bitmapPtr.GetPtr();
}


// protected slots

void CBitmapSupplierGuiComp::on_SnapImageButton_clicked()
{
	// snap with UI message by default
	DoSnap();
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

	SnapImageButton->setEnabled(!checked);
}


void CBitmapSupplierGuiComp::on_SaveImageButton_clicked()
{
	Q_ASSERT(m_bitmapPtr.IsValid());

	if (		m_bitmapLoaderCompPtr.IsValid() &&
				m_bitmapLoaderCompPtr->SaveToFile(*m_bitmapPtr, "") == ifile::IFilePersistence::OS_FAILED){
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
	iinsp::ISupplier* supplierPtr = GetObservedObject();
	if (supplierPtr != NULL){
		istd::CChangeGroup changeGroup(supplierPtr);

		supplierPtr->InvalidateSupplier();
		supplierPtr->EnsureWorkInitialized();
		supplierPtr->EnsureWorkFinished();

		if (supplierPtr->GetWorkStatus() == iinsp::ISupplier::WS_FAILED){
			if (noGui){
				SendCriticalMessage(0, QObject::tr("Snap Error"));
			}
			else{
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

	SaveImageButton->setVisible(m_bitmapLoaderCompPtr.IsValid() && m_bitmapPtr.IsValid());
}


void CBitmapSupplierGuiComp::OnGuiHidden()
{
	LiveImageButton->setChecked(false);

	BaseClass::OnGuiHidden();
}

void CBitmapSupplierGuiComp::OnGuiRetranslate()
{
	BaseClass::OnGuiRetranslate();

	if (m_snapButtonTextAttrPtr.IsValid()){
		const QString text = QObject::tr(*m_snapButtonTextAttrPtr);
		if (!text.isEmpty()){
			SnapImageButton->setText(text);
		}
	}

	if (m_snapButtonTooltipAttrPtr.IsValid()){
		const QString tooltip = QObject::tr(*m_snapButtonTooltipAttrPtr);
		if (!tooltip.isEmpty()){
			SnapImageButton->setToolTip(tooltip);
		}
	}
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
	imod::IModel* bitmapModelPtr = dynamic_cast<imod::IModel*>(m_bitmapPtr.GetPtr());
	iview::CImageShape* shapePtr = new iview::CImageShape;

	if ((bitmapModelPtr != NULL) && (shapePtr != NULL)){
		shapePtr->AssignToLayer(iview::IViewLayer::LT_BACKGROUND);

		result.PushBack(shapePtr);

		bitmapModelPtr->AttachObserver(shapePtr);
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


void CBitmapSupplierGuiComp::OnGuiModelDetached()
{
	if (m_bitmapPtr.IsValid()){
		m_bitmapPtr->ResetImage();
	}

	BaseClass::OnGuiModelDetached();
}


void CBitmapSupplierGuiComp::UpdateGui(const istd::IChangeable::ChangeSet& changeSet)
{
	BaseClass::UpdateGui(changeSet);

	Q_ASSERT(IsGuiCreated());

	bool isLive = LiveImageButton->isChecked();

	if (m_bitmapPtr.IsValid()){
		istd::CIndex2d bitmapSize = m_bitmapPtr->GetImageSize();

		double memorySize = bitmapSize.GetY() * m_bitmapPtr->GetLineBytesCount() / 1024.0;
		QString suffix = "KBytes";

		if (memorySize > 1000){
			suffix = "MBytes";

			memorySize /= 1024;
		}

		if (memorySize > 1000){
			suffix = "GBytes";

			memorySize /= 1024;
		}

		SizeLabel->setText(tr("(%1 x %2, %3 %4)").arg(bitmapSize.GetX()).arg(bitmapSize.GetY()).arg(memorySize, 1, 'f', 1).arg(suffix));
		SaveImageButton->setEnabled(!bitmapSize.IsSizeEmpty() && !isLive);
	}

	UpdateAllViews();
}


// reimplemented (imod::IObserver)

void CBitmapSupplierGuiComp::AfterUpdate(imod::IModel* modelPtr, const istd::IChangeable::ChangeSet& changeSet)
{
	const iimg::IBitmap* bitmapPtr = NULL;

	iinsp::ISupplier* supplierPtr = GetObservedObject();
	if (supplierPtr != NULL){
		iimg::IBitmapProvider* providerPtr = dynamic_cast<iimg::IBitmapProvider*>(supplierPtr);
		if (providerPtr != NULL){
			bitmapPtr = providerPtr->GetBitmap();
		}
		else{
			providerPtr = CompCastPtr<iimg::IBitmapProvider>(supplierPtr);
			if (providerPtr != NULL){
				bitmapPtr = providerPtr->GetBitmap();
			}
		}
	}

	if (m_bitmapPtr.IsValid()){
		if ((bitmapPtr == NULL) || !m_bitmapPtr->CopyFrom(*bitmapPtr)){
			m_bitmapPtr->ResetImage();
		}

		istd::CIndex2d imageSize = m_bitmapPtr->GetImageSize();
		i2d::CRectangle imageBox(0, 0, imageSize.GetX(), imageSize.GetY());

		const ShapesMap& shapesMap = GetShapesMap();
		QList<iqt2d::IViewProvider*> keys = shapesMap.keys();

#if QT_VERSION >= 0x600000
		QSet<iqt2d::IViewProvider*> views(keys.begin(), keys.end());

#else
		QSet<iqt2d::IViewProvider*> views = keys.toSet();
#endif

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
	}

	BaseClass::AfterUpdate(modelPtr, changeSet);
}


// reimplemented (icomp::CComponentBase)

void CBitmapSupplierGuiComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	if (m_bitmapFactPtr.IsValid()){
		m_bitmapPtr.SetPtr(m_bitmapFactPtr.CreateInstance());
	}
	else{
		m_bitmapPtr.SetPtr(new BitmapImpl);
	}
}


} // namespace iqtcam


