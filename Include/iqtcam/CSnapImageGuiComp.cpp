#include <iqtcam/CSnapImageGuiComp.h>


// Qt includes
#include<QtCore/QtGlobal>
#if QT_VERSION >= 0x050000
#include <QtWidgets/QMessageBox>
#else
#include <QtGui/QMessageBox>
#endif

// ACF includes
#include <istd/TDelPtr.h>
#include <iview/CImageShape.h>


namespace iqtcam
{


CSnapImageGuiComp::CSnapImageGuiComp()
:	m_paramsObserver(this)
{
	m_timer.setSingleShot(true);
	QObject::connect(&m_timer, SIGNAL(timeout()), this, SLOT(OnTimerReady()));
}


// reimplemented (iqt2d::IViewExtender)

void CSnapImageGuiComp::AddItemsToScene(iqt2d::IViewProvider* providerPtr, int flags)
{
	BaseClass::AddItemsToScene(providerPtr, flags);

	if (m_paramsSetExtenderCompPtr.IsValid()){
		m_paramsSetExtenderCompPtr->AddItemsToScene(providerPtr, flags);
	}
}


void CSnapImageGuiComp::RemoveItemsFromScene(iqt2d::IViewProvider* providerPtr)
{
	if (m_paramsSetExtenderCompPtr.IsValid()){
		m_paramsSetExtenderCompPtr->RemoveItemsFromScene(providerPtr);
	}

	BaseClass::RemoveItemsFromScene(providerPtr);
}


// protected methods

bool CSnapImageGuiComp::SnapImage()
{
	bool retVal = false;

	if (m_bitmapAcquisitionCompPtr.IsValid() && m_bitmapCompPtr.IsValid()){
		int taskId = m_bitmapAcquisitionCompPtr->BeginTask(m_paramsSetCompPtr.GetPtr(), NULL, m_bitmapCompPtr.GetPtr());
		if (taskId >= 0){
			retVal = m_bitmapAcquisitionCompPtr->WaitTaskFinished(-1, 1) != icam::IBitmapAcquisition::TS_INVALID;
		}
	}

	UpdateButtonsState();

	return retVal;
}


void CSnapImageGuiComp::UpdateButtonsState()
{
	if (m_bitmapCompPtr.IsValid() && m_bitmapLoaderCompPtr.IsValid()){
		LoadImageButton->setEnabled(m_bitmapLoaderCompPtr->IsOperationSupported(m_bitmapCompPtr.GetPtr(), NULL, ifile::IFileTypeInfo::QF_LOAD | ifile::IFileTypeInfo::QF_FILE));
		SaveImageButton->setEnabled(m_bitmapLoaderCompPtr->IsOperationSupported(m_bitmapCompPtr.GetPtr(), NULL, ifile::IFileTypeInfo::QF_SAVE | ifile::IFileTypeInfo::QF_FILE));
	}

	if (m_bitmapCompPtr.IsValid() && m_bitmapAcquisitionCompPtr.IsValid()){
		istd::CIndex2d bitmapSize = m_bitmapCompPtr->GetImageSize();

		if (bitmapSize.IsValid() && !bitmapSize.IsZero()){
			const BaseClass::ShapesMap& shapesMap = BaseClass::GetShapesMap();

			for (BaseClass::ShapesMap::ConstIterator iter = shapesMap.constBegin(); iter != shapesMap.constEnd(); ++iter){
				const iqt2d::IViewProvider* providerPtr = iter.key();
				if (providerPtr != NULL){
					iview::IShapeView* viewPtr = providerPtr->GetView();
					if (viewPtr != NULL){
						viewPtr->SetFitArea(i2d::CRectangle(0, 0, bitmapSize.GetX(), bitmapSize.GetY()));
					}
				}
			}
		}
	}
}


// reimplemented (iqt2d::TViewExtenderCompBase)

void CSnapImageGuiComp::CreateShapes(int /*sceneId*/, Shapes& result)
{
	if (m_bitmapModelCompPtr.IsValid()){
		icmm::IColorTransformation* lookupTablePtr = NULL;
		if (m_lookupTableCompPtr.IsValid()){
			lookupTablePtr = m_lookupTableCompPtr.GetPtr();
		}

		istd::TDelPtr<iview::CImageShape> shapePtr(new iview::CImageShape(lookupTablePtr));
		if (shapePtr != NULL){
			if (m_bitmapModelCompPtr->AttachObserver(shapePtr.GetPtr())){
				shapePtr->AssignToLayer(iview::IViewLayer::LT_BACKGROUND);
			
				result.PushBack(shapePtr.PopPtr());
			}
		}
	}
}


// reimplemented (iqtgui::CGuiComponentBase)

void CSnapImageGuiComp::OnGuiCreated()
{
	bool hasBitmap = m_bitmapCompPtr.IsValid();
	bool hasSnap = m_bitmapAcquisitionCompPtr.IsValid();

	QObject::connect(IntervalSnapButton, SIGNAL(toggled(bool)), this, SLOT(OnIntervalSnap(bool)));
	QObject::connect(OnChangeSnapButton, SIGNAL(toggled(bool)), this, SLOT(OnSnapOnChanges(bool)));

	SnapImageButton->setVisible(hasBitmap && hasSnap);
	LoadImageButton->setVisible(hasBitmap && m_bitmapLoaderCompPtr.IsValid() && *m_allowBitmapLoadAttrPtr);
	SaveImageButton->setVisible(hasBitmap && m_bitmapLoaderCompPtr.IsValid());
	SaveImageButton->setVisible(hasBitmap && m_bitmapLoaderCompPtr.IsValid());
	SaveImageButton->setEnabled(false);
	OnChangeSnapButton->setVisible(m_allowSnapOnChangeAttrPtr.IsValid() && *m_allowSnapOnChangeAttrPtr);

	if (m_paramsSetGuiCompPtr.IsValid()){
		m_paramsSetGuiCompPtr->CreateGui(ParamsFrame);

		if (m_paramsSetModelCompPtr.IsValid() && m_paramsSetObserverCompPtr.IsValid()){
			m_paramsSetModelCompPtr->AttachObserver(m_paramsSetObserverCompPtr.GetPtr());

			m_paramsSetModelCompPtr->AttachObserver(&m_paramsObserver);
		}

		ParamsFrame->setVisible(true);
	}
	else{
		ParamsFrame->setVisible(false);
	}

	ParamsGB->setVisible(m_paramsSetGuiCompPtr.IsValid() || m_paramsLoaderCompPtr.IsValid());
	LoadParamsButton->setVisible(m_paramsLoaderCompPtr.IsValid());
	SaveParamsButton->setVisible(m_paramsLoaderCompPtr.IsValid());
}


void CSnapImageGuiComp::OnGuiDestroyed()
{
	m_timer.stop();

	if (		m_paramsSetModelCompPtr.IsValid() &&
				m_paramsSetObserverCompPtr.IsValid() &&
				m_paramsSetModelCompPtr->IsAttached(m_paramsSetObserverCompPtr.GetPtr())){
		m_paramsSetModelCompPtr->DetachObserver(m_paramsSetObserverCompPtr.GetPtr());
	}

	if (m_paramsSetGuiCompPtr.IsValid() && m_paramsSetGuiCompPtr->IsGuiCreated()){
		m_paramsSetGuiCompPtr->DestroyGui();
	}
}


void CSnapImageGuiComp::OnGuiHidden()
{
	m_timer.stop();

	IntervalSnapButton->setChecked(false);
	OnChangeSnapButton->setChecked(false);

	BaseClass::OnGuiHidden();
}


// reimplemented (icomp::CComponentBase)

void CSnapImageGuiComp::OnComponentCreated()
{
}


// protected slots

void CSnapImageGuiComp::on_SnapImageButton_clicked()
{
	m_timer.stop();
	IntervalSnapButton->setChecked(false);
	OnChangeSnapButton->setChecked(false);

	if (SnapImageButton->isCheckable()){
		SnapImageButton->setChecked(false);
		SnapImageButton->setCheckable(false);
	}
	else{
		SnapImage();
	}
}


void CSnapImageGuiComp::on_LoadImageButton_clicked()
{
	if (m_bitmapLoaderCompPtr.IsValid() && m_bitmapCompPtr.IsValid()){
		if (m_bitmapLoaderCompPtr->LoadFromFile(*m_bitmapCompPtr, "") == ifile::IFilePersistence::OS_FAILED){
			QMessageBox::warning(
						GetQtWidget(),
						QObject::tr("Error"),
						QObject::tr("Cannot load image"));
		}
	}

	UpdateButtonsState();
}


void CSnapImageGuiComp::on_SaveImageButton_clicked()
{
	if (m_bitmapLoaderCompPtr.IsValid() && m_bitmapCompPtr.IsValid()){
		if (m_bitmapLoaderCompPtr->SaveToFile(*m_bitmapCompPtr, "") == ifile::IFilePersistence::OS_FAILED){
			QMessageBox::warning(
						GetQtWidget(),
						QObject::tr("Error"),
						QObject::tr("Cannot save image"));
		}
	}
}


void CSnapImageGuiComp::on_LoadParamsButton_clicked()
{
	if (m_paramsLoaderCompPtr.IsValid() && m_paramsSetCompPtr.IsValid()){
		if (m_paramsLoaderCompPtr->LoadFromFile(*m_paramsSetCompPtr, "") == ifile::IFilePersistence::OS_FAILED){
			QMessageBox::warning(
						GetQtWidget(),
						QObject::tr("Error"),
						QObject::tr("Cannot load parameters"));
		}
	}
}


void CSnapImageGuiComp::on_SaveParamsButton_clicked()
{
	if (m_paramsLoaderCompPtr.IsValid() && m_paramsSetCompPtr.IsValid()){
		if (m_paramsLoaderCompPtr->SaveToFile(*m_paramsSetCompPtr, "") == ifile::IFilePersistence::OS_FAILED){
			QMessageBox::warning(
						GetQtWidget(),
						QObject::tr("Error"),
						QObject::tr("Cannot save parameters"));
		}
	}
}


void CSnapImageGuiComp::OnIntervalSnap(bool checked)
{
	OnChangeSnapButton->setChecked(false);

	if (checked){
		SnapImageButton->setCheckable(true);
		SnapImageButton->setChecked(true);

		m_timer.start();
	}
	else{
		m_timer.stop();

		SnapImageButton->setChecked(false);
		SnapImageButton->setCheckable(false);
	}
}


void CSnapImageGuiComp::OnSnapOnChanges(bool checked)
{
	if (checked){
		m_timer.stop();

		SnapImageButton->setCheckable(true);
		SnapImageButton->setChecked(true);
	}
	else{
		SnapImageButton->setChecked(false);
		SnapImageButton->setCheckable(false);
	}

	IntervalSnapButton->setChecked(false);
}


void CSnapImageGuiComp::OnTimerReady()
{
	SnapImage();
	m_timer.start();
}


// public methods of embedded class ParamsObserver

CSnapImageGuiComp::ParamsObserver::ParamsObserver(CSnapImageGuiComp* parentPtr)
:	m_parent(*parentPtr)
{
	Q_ASSERT(parentPtr != NULL);
}


// protected methods of embedded class ParamsObserver

// reimplemented (imod::CSingleModelObserverBase)

void CSnapImageGuiComp::ParamsObserver::OnUpdate(const istd::IChangeable::ChangeSet& /*changeSet*/)
{
	if (m_parent.OnChangeSnapButton->isChecked()){
		m_parent.SnapImage();
	}
}


} // namespace iqtcam


