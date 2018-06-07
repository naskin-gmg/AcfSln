/********************************************************************************
**
**	Copyright (C) 2007-2017 Witold Gantzke & Kirill Lepskiy
**
**	This file is part of the ACF-Solutions Toolkit.
**
**	This file may be used under the terms of the GNU Lesser
**	General Public License version 2.1 as published by the Free Software
**	Foundation and appearing in the file LicenseLGPL.txt included in the
**	packaging of this file.  Please review the following information to
**	ensure the GNU Lesser General Public License version 2.1 requirements
**	will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
**	If you are unsure which license is appropriate for your use, please
**	contact us at info@imagingtools.de.
**
** 	See http://www.ilena.org or write info@imagingtools.de for further
** 	information about the ACF.
**
********************************************************************************/


#include <iqtcam/CSnapImageParamsEditorComp.h>


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


CSnapImageParamsEditorComp::CSnapImageParamsEditorComp()
:	m_paramsObserver(this)
{
	m_timer.setInterval(40);
	QObject::connect(&m_timer, SIGNAL(timeout()), this, SLOT(OnTimerReady()));
}


// reimplemented (iqt2d::IViewExtender)

void CSnapImageParamsEditorComp::AddItemsToScene(iqt2d::IViewProvider* providerPtr, int flags)
{
	BaseClass::AddItemsToScene(providerPtr, flags);

	if (m_paramsSetExtenderCompPtr.IsValid()){
		m_paramsSetExtenderCompPtr->AddItemsToScene(providerPtr, flags);
	}
}


void CSnapImageParamsEditorComp::RemoveItemsFromScene(iqt2d::IViewProvider* providerPtr)
{
	if (m_paramsSetExtenderCompPtr.IsValid()){
		m_paramsSetExtenderCompPtr->RemoveItemsFromScene(providerPtr);
	}

	BaseClass::RemoveItemsFromScene(providerPtr);
}


// protected methods

bool CSnapImageParamsEditorComp::SnapImage()
{
	bool retVal = false;

	if (m_bitmapAcquisitionCompPtr.IsValid() && m_bitmapCompPtr.IsValid()){
		int taskId = m_bitmapAcquisitionCompPtr->BeginTask(GetObservedObject(), NULL, m_bitmapCompPtr.GetPtr());
		if (taskId >= 0){
			retVal = m_bitmapAcquisitionCompPtr->WaitTaskFinished(-1, 1) != icam::IBitmapAcquisition::TS_INVALID;
		}
	}

	UpdateButtonsState();

	return retVal;
}


void CSnapImageParamsEditorComp::UpdateButtonsState()
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

void CSnapImageParamsEditorComp::CreateShapes(int /*sceneId*/, Shapes& result)
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

void CSnapImageParamsEditorComp::OnGuiCreated()
{
	BaseClass::OnGuiCreated();

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

	ParamsGB->setVisible(m_paramsSetGuiCompPtr.IsValid() || m_paramsLoaderCompPtr.IsValid());
	LoadParamsButton->setVisible(m_paramsLoaderCompPtr.IsValid());
	SaveParamsButton->setVisible(m_paramsLoaderCompPtr.IsValid());
}


void CSnapImageParamsEditorComp::OnGuiDestroyed()
{
	m_timer.stop();

	if (m_paramsSetGuiCompPtr.IsValid() && m_paramsSetGuiCompPtr->IsGuiCreated()){
		m_paramsSetGuiCompPtr->DestroyGui();
	}

	BaseClass::OnGuiDestroyed();
}


void CSnapImageParamsEditorComp::OnGuiHidden()
{
	m_timer.stop();

	IntervalSnapButton->setChecked(false);
	OnChangeSnapButton->setChecked(false);

	BaseClass::OnGuiHidden();
}


// reimplemented (iqtgui::TGuiObserverWrap)

void CSnapImageParamsEditorComp::OnGuiModelAttached()
{
	BaseClass::OnGuiModelAttached();

	if (m_paramsSetGuiCompPtr.IsValid()){
		m_paramsSetGuiCompPtr->CreateGui(ParamsFrame);

		imod::IModel* observedModelPtr = GetObservedModel();
		if ((observedModelPtr != NULL) && m_paramsSetObserverCompPtr.IsValid()){
			observedModelPtr->AttachObserver(m_paramsSetObserverCompPtr.GetPtr());

			observedModelPtr->AttachObserver(&m_paramsObserver);
		}

		ParamsFrame->setVisible(true);
	}
	else{
		ParamsFrame->setVisible(false);
	}
}


void CSnapImageParamsEditorComp::OnGuiModelDetached()
{
	imod::IModel* observedModelPtr = GetObservedModel();
	if (		(observedModelPtr != NULL) &&
				m_paramsSetObserverCompPtr.IsValid() &&
				observedModelPtr->IsAttached(m_paramsSetObserverCompPtr.GetPtr())){
		observedModelPtr->DetachObserver(m_paramsSetObserverCompPtr.GetPtr());
	}

	m_paramsObserver.EnsureModelDetached();

	BaseClass::OnGuiModelDetached();
}


// reimplemented (icomp::CComponentBase)

void CSnapImageParamsEditorComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	m_timer.setInterval(int(*m_liveIntervalAttrPtr * 1000));
}


// protected slots

void CSnapImageParamsEditorComp::on_SnapImageButton_clicked()
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


void CSnapImageParamsEditorComp::on_LoadImageButton_clicked()
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


void CSnapImageParamsEditorComp::on_SaveImageButton_clicked()
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


void CSnapImageParamsEditorComp::on_LoadParamsButton_clicked()
{
	iprm::IParamsSet* objectPtr = GetObservedObject();

	if (m_paramsLoaderCompPtr.IsValid() && (objectPtr != NULL)){
		if (m_paramsLoaderCompPtr->LoadFromFile(*objectPtr, "") == ifile::IFilePersistence::OS_FAILED){
			QMessageBox::warning(
						GetQtWidget(),
						QObject::tr("Error"),
						QObject::tr("Cannot load parameters"));
		}
	}
}


void CSnapImageParamsEditorComp::on_SaveParamsButton_clicked()
{
	iprm::IParamsSet* objectPtr = GetObservedObject();

	if (m_paramsLoaderCompPtr.IsValid() && (objectPtr != NULL)){
		if (m_paramsLoaderCompPtr->SaveToFile(*objectPtr, "") == ifile::IFilePersistence::OS_FAILED){
			QMessageBox::warning(
						GetQtWidget(),
						QObject::tr("Error"),
						QObject::tr("Cannot save parameters"));
		}
	}
}


void CSnapImageParamsEditorComp::OnIntervalSnap(bool checked)
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


void CSnapImageParamsEditorComp::OnSnapOnChanges(bool checked)
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


void CSnapImageParamsEditorComp::OnTimerReady()
{
	SnapImage();
}


// public methods of embedded class ParamsObserver

CSnapImageParamsEditorComp::ParamsObserver::ParamsObserver(CSnapImageParamsEditorComp* parentPtr)
:	m_parent(*parentPtr)
{
	Q_ASSERT(parentPtr != NULL);
}


// protected methods of embedded class ParamsObserver

// reimplemented (imod::CSingleModelObserverBase)

void CSnapImageParamsEditorComp::ParamsObserver::OnUpdate(const istd::IChangeable::ChangeSet& /*changeSet*/)
{
	if (m_parent.OnChangeSnapButton->isChecked()){
		m_parent.SnapImage();
	}
}


} // namespace iqtcam


