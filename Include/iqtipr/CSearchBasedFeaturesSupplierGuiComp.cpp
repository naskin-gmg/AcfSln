#include "iqtipr/CSearchBasedFeaturesSupplierGuiComp.h"


// ACF includes
#include "imath/CVarVector.h"

// ACF-Solutions includes
#include "imeas/INumericValueProvider.h"

#include "iipr/CSearchFeature.h"


namespace iqtipr
{


CSearchBasedFeaturesSupplierGuiComp::CSearchBasedFeaturesSupplierGuiComp()
	:m_lastViewPtr(NULL)
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

QWidget* CSearchBasedFeaturesSupplierGuiComp::GetParamsWidget() const
{
	Q_ASSERT(IsGuiCreated());

	return ParamsFrame;
}


void CSearchBasedFeaturesSupplierGuiComp::OnSupplierParamsChanged()
{
	if (IsGuiCreated() && AutoUpdateButton->isChecked()){
		DoTest();
	}
}


// reimplemented (iqt2d::IViewExtender)

void CSearchBasedFeaturesSupplierGuiComp::AddItemsToScene(iqt2d::IViewProvider* providerPtr, int flags)
{
	Q_ASSERT(providerPtr != NULL);

	iview::IShapeView* viewPtr = providerPtr->GetView();
	if (viewPtr != NULL){
		m_lastViewPtr = viewPtr;

		ConnectShapes(*viewPtr);
	}
	
	BaseClass::AddItemsToScene(providerPtr, flags);
}


void CSearchBasedFeaturesSupplierGuiComp::RemoveItemsFromScene(iqt2d::IViewProvider* providerPtr)
{
	Q_ASSERT(providerPtr != NULL);

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

	Q_ASSERT(IsGuiCreated());

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
		imeas::INumericValueProvider* providerPtr = dynamic_cast<imeas::INumericValueProvider*>(supplierPtr);
		if (providerPtr != NULL){
			int featuresCount = providerPtr->GetValuesCount();

			for (int featureIndex = 0; featureIndex < featuresCount; featureIndex++){
				const iipr::CSearchFeature* searchFeaturePtr = dynamic_cast<const iipr::CSearchFeature*>(&providerPtr->GetNumericValue(featureIndex));
				if (searchFeaturePtr != NULL){
					QTreeWidgetItem* modelItemPtr = new QTreeWidgetItem;
					modelItemPtr->setText(CT_ID, searchFeaturePtr->GetId());
					modelItemPtr->setText(CT_SCORE, QString::number(searchFeaturePtr->GetWeight() * 100, 'f', 2));
					modelItemPtr->setText(CT_POSITION, QString::number(searchFeaturePtr->GetPosition().GetX(), 'f', 2)+","+QString::number(searchFeaturePtr->GetPosition().GetY(), 'f', 2));
					modelItemPtr->setText(CT_ANGLE, QString::number(imath::GetDegreeFromRadian(searchFeaturePtr->GetAngle()), 'f', 2));
					modelItemPtr->setText(CT_SCALE, QString::number(searchFeaturePtr->GetScale().GetX(), 'f', 2)+","+QString::number(searchFeaturePtr->GetScale().GetY(), 'f', 2));

					ResultsList->addTopLevelItem(modelItemPtr);

					VisualObject* visualObject = new VisualObject;

					visualObject->model.SetPtr(new PositionModel);
					visualObject->model->SetPosition(searchFeaturePtr->GetPosition());
					visualObject->model->SetRadius(qMax(5.0, maxScoreRadius * searchFeaturePtr->GetWeight()));

					visualObject->shape.SetPtr(new iview::CCircleShape());
					visualObject->shape->SetEditablePosition(false);
					visualObject->shape->SetEditableRadius(false);

					visualObject->model->AttachObserver(visualObject->shape.GetPtr());

					m_visualPositions.PushBack(visualObject);
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

#if QT_VERSION < 0x050000
	ResultsList->header()->setResizeMode(QHeaderView::ResizeToContents);
#else
	ResultsList->header()->setSectionResizeMode(QHeaderView::ResizeToContents);
#endif
}


void CSearchBasedFeaturesSupplierGuiComp::OnGuiHidden()
{
	AutoUpdateButton->setChecked(false);

	BaseClass::OnGuiHidden();
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
		Q_ASSERT(objectPtr != NULL);

		view.ConnectShape(objectPtr->shape.GetPtr());
	}
}


void CSearchBasedFeaturesSupplierGuiComp::DisconnectShapes(iview::IShapeView& view)
{
	int shapesCount = m_visualPositions.GetCount();
	for (int shapeIndex = 0; shapeIndex < shapesCount; shapeIndex++){
		VisualObject* objectPtr = m_visualPositions.GetAt(shapeIndex);
		Q_ASSERT(objectPtr != NULL);

		view.DisconnectShape(objectPtr->shape.GetPtr());
	}
}


} // namespace iqtipr


