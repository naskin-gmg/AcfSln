#include "iqtipr/CSearchBasedFeaturesSupplierGuiComp.h"


// Qt includes
#include <QtGui/QMessageBox>
#include <QElapsedTimer>

// ACF includes
#include "imath/CVarVector.h"

// ACF-Solutions includes
#include "iipr/IFeaturesProvider.h"
#include "iipr/CSearchFeature.h"


namespace iqtipr
{


CSearchBasedFeaturesSupplierGuiComp::CSearchBasedFeaturesSupplierGuiComp()
:	m_paramsObserver(this),
	m_lastViewPtr(NULL)
{
}


// protected slots

void CSearchBasedFeaturesSupplierGuiComp::on_TestButton_clicked()
{
	DoTest();
}


void CSearchBasedFeaturesSupplierGuiComp::on_LoadParamsButton_clicked()
{
	LoadParams();
}


void CSearchBasedFeaturesSupplierGuiComp::on_SaveParamsButton_clicked()
{
	SaveParams();
}


// protected methods

// reimplemented (iqtinsp::TSupplierGuiCompBase)


bool CSearchBasedFeaturesSupplierGuiComp::DoTest()
{
	QElapsedTimer timer;
	timer.start();

	bool retVal = BaseClass::DoTest();

	quint64 time = timer.nsecsElapsed() / 1000000.0;
	TimeLabel->setText(QString("%1 ms").arg(time));

	return retVal;
}


QWidget* CSearchBasedFeaturesSupplierGuiComp::GetParamsWidget() const
{
	I_ASSERT(IsGuiCreated());

	return ParamsFrame;
}


// reimplemented (iqt2d::IViewExtender)

void CSearchBasedFeaturesSupplierGuiComp::AddItemsToScene(iqt2d::IViewProvider* providerPtr, int flags)
{
	I_ASSERT(providerPtr != NULL);

	iview::IShapeView* viewPtr = providerPtr->GetView();
	if (viewPtr != NULL){
		m_lastViewPtr = viewPtr;

		ConnectShapes(*viewPtr);
	}
	
	BaseClass::AddItemsToScene(providerPtr, flags);
}


void CSearchBasedFeaturesSupplierGuiComp::RemoveItemsFromScene(iqt2d::IViewProvider* providerPtr)
{
	I_ASSERT(providerPtr != NULL);

	iview::IShapeView* viewPtr = providerPtr->GetView();
	if (viewPtr != NULL){
		DisconnectShapes(*viewPtr);
	}

	m_lastViewPtr = NULL;

	BaseClass::RemoveItemsFromScene(providerPtr);
}


// reimplemented (iqtgui::TGuiObserverWrap)

void CSearchBasedFeaturesSupplierGuiComp::OnGuiModelAttached()
{
	BaseClass::OnGuiModelAttached();

	ParamsGB->setVisible(AreParamsEditable() || IsLoadParamsSupported());
	LoadParamsButton->setVisible(IsLoadParamsSupported());
	SaveParamsButton->setVisible(IsSaveParamsSupported());
}


void CSearchBasedFeaturesSupplierGuiComp::UpdateGui(int updateFlags)
{
	BaseClass::UpdateGui(updateFlags);

	I_ASSERT(IsGuiCreated());

	if ((updateFlags & iproc::ISupplier::CF_SUPPLIER_RESULTS) == 0){
		return;
	}

	ResultsList->clear();

	if (m_lastViewPtr != NULL){
		DisconnectShapes(*m_lastViewPtr);
	}

	m_visualPositions.Reset();

	double maxScoreRadius = 50;

	iproc::ISupplier* supplierPtr = GetObjectPtr();
	if (supplierPtr != NULL){
		int workStatus = supplierPtr->GetWorkStatus();
		if (workStatus == iproc::ISupplier::WS_OK){
			iipr::IFeaturesProvider* providerPtr = dynamic_cast<iipr::IFeaturesProvider*>(supplierPtr);
			if (providerPtr != NULL){
				iipr::IFeaturesProvider::Features foundFeatures = providerPtr->GetFeatures();
				int featuresCount = foundFeatures.size();

				for (int featureIndex = 0; featureIndex < featuresCount; featureIndex++){
					const iipr::CSearchFeature* searchFeaturePtr = dynamic_cast<const iipr::CSearchFeature*>(foundFeatures[featureIndex]);
					if (searchFeaturePtr != NULL){
						QTreeWidgetItem* modelItemPtr = new QTreeWidgetItem;
						modelItemPtr->setText(CT_ID, searchFeaturePtr->GetId());
						modelItemPtr->setText(CT_SCORE, QString::number(searchFeaturePtr->GetWeight() * 100, 'f', 2));
						modelItemPtr->setText(CT_X, QString::number(searchFeaturePtr->GetPosition().GetX(), 'f', 2));
						modelItemPtr->setText(CT_Y, QString::number(searchFeaturePtr->GetPosition().GetY(), 'f', 2));
						modelItemPtr->setText(CT_ANGLE, QString::number(imath::GetDegreeFromRadian(searchFeaturePtr->GetAngle()), 'f', 2));
						modelItemPtr->setText(CT_X_SCALE, QString::number(searchFeaturePtr->GetScale().GetX(), 'f', 2));
						modelItemPtr->setText(CT_Y_SCALE, QString::number(searchFeaturePtr->GetScale().GetY(), 'f', 2));

						ResultsList->addTopLevelItem(modelItemPtr);

						VisualObject* visualObject = new VisualObject;

						visualObject->model.SetPtr(new PositionModel);
						visualObject->model->SetPosition(searchFeaturePtr->GetPosition());
						visualObject->model->SetRadius(qMax(5.0, maxScoreRadius * searchFeaturePtr->GetWeight()));

						visualObject->shape.SetPtr(new iview::CInteractiveCircleShape());
						visualObject->shape->SetEditablePosition(false);
						visualObject->shape->SetEditableRadius(false);

						visualObject->model->AttachObserver(visualObject->shape.GetPtr());

						m_visualPositions.PushBack(visualObject);
					}
				}
			}
		}

		imod::IModel* paramsModelPtr = dynamic_cast<imod::IModel*>(supplierPtr->GetModelParametersSet());
		if (paramsModelPtr != NULL){
			if (!paramsModelPtr->IsAttached(&m_paramsObserver)){
				m_paramsObserver.EnsureModelDetached();

				paramsModelPtr->AttachObserver(&m_paramsObserver);
			}
		}
	}

	if (m_lastViewPtr != NULL){
		ConnectShapes(*m_lastViewPtr);
	}
}


// reimplemented (iqtgui::IGuiObject)

void CSearchBasedFeaturesSupplierGuiComp::OnGuiCreated()
{
	BaseClass::OnGuiCreated();

	ResultsList->header()->setResizeMode(QHeaderView::ResizeToContents);
}


void CSearchBasedFeaturesSupplierGuiComp::OnGuiDestroyed()
{	
	BaseClass::OnGuiDestroyed();
}


// reimplemented (icomp::IComponentBase)

void CSearchBasedFeaturesSupplierGuiComp::OnComponentDestroyed()
{
	m_paramsObserver.EnsureModelDetached();

	BaseClass::OnComponentDestroyed();
}


// private methods

void CSearchBasedFeaturesSupplierGuiComp::ConnectShapes(iview::IShapeView& view)
{
	int shapesCount = m_visualPositions.GetCount();
	for (int shapeIndex = 0; shapeIndex < shapesCount; shapeIndex++){
		VisualObject* objectPtr = m_visualPositions.GetAt(shapeIndex);
		I_ASSERT(objectPtr != NULL);

		view.ConnectShape(objectPtr->shape.GetPtr());
	}
}


void CSearchBasedFeaturesSupplierGuiComp::DisconnectShapes(iview::IShapeView& view)
{
	int shapesCount = m_visualPositions.GetCount();
	for (int shapeIndex = 0; shapeIndex < shapesCount; shapeIndex++){
		VisualObject* objectPtr = m_visualPositions.GetAt(shapeIndex);
		I_ASSERT(objectPtr != NULL);

		view.DisconnectShape(objectPtr->shape.GetPtr());
	}
}


// public methods of embedded class ParamsObserver

CSearchBasedFeaturesSupplierGuiComp::ParamsObserver::ParamsObserver(CSearchBasedFeaturesSupplierGuiComp* parentPtr)
:	m_parent(*parentPtr)
{
	I_ASSERT(parentPtr != NULL);
}


// reimplemented (imod::CSingleModelObserverBase)

void CSearchBasedFeaturesSupplierGuiComp::ParamsObserver::OnUpdate(int updateFlags, istd::IPolymorphic* /*updateParamsPtr*/)
{
	if (		((updateFlags & istd::IChangeable::CF_MODEL) != 0) &&
				m_parent.IsGuiCreated() &&
				m_parent.AutoUpdateButton->isChecked()){
		m_parent.DoTest();
	}
}


} // namespace iqtipr


