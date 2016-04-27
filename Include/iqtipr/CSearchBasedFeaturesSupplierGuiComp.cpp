#include "iqtipr/CSearchBasedFeaturesSupplierGuiComp.h"

// Qt includes
#include <QtCore/QTimer>

// ACF includes
#include "imath/CVarVector.h"

// ACF-Solutions includes
#include "imeas/INumericValueProvider.h"
#include "iipr/CSearchFeature.h"


namespace iqtipr
{


CSearchBasedFeaturesSupplierGuiComp::CSearchBasedFeaturesSupplierGuiComp()
:	m_lastViewPtr(NULL)
{
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
	if (IsGuiCreated() && AutoTestButton->isChecked()){
		QTimer::singleShot(1, this, SLOT(on_TestButton_clicked()));
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


void CSearchBasedFeaturesSupplierGuiComp::UpdateGui(const istd::IChangeable::ChangeSet& changeSet)
{
	BaseClass::UpdateGui(changeSet);

	Q_ASSERT(IsGuiCreated());

	if (!changeSet.Contains(iinsp::ISupplier::CF_SUPPLIER_RESULTS)){
		return;
	}

	ResultsList->clear();

	if (m_lastViewPtr != NULL){
		DisconnectShapes(*m_lastViewPtr);
	}

	m_visualPositions.Reset();

	double maxScoreRadius = 50;

	iinsp::ISupplier* supplierPtr = GetObservedObject();
	if (supplierPtr != NULL){
		imeas::INumericValueProvider* providerPtr = dynamic_cast<imeas::INumericValueProvider*>(supplierPtr);
		if (providerPtr != NULL){
			int featuresCount = providerPtr->GetValuesCount();

			for (int featureIndex = 0; featureIndex < featuresCount; featureIndex++){
				const iipr::CObjectFeature* objectFeaturePtr = dynamic_cast<const iipr::CObjectFeature*>(&providerPtr->GetNumericValue(featureIndex));
				if (objectFeaturePtr != NULL){
					QTreeWidgetItem* modelItemPtr = new QTreeWidgetItem;
					modelItemPtr->setText(CT_ID, objectFeaturePtr->GetObjectId());
					modelItemPtr->setText(CT_SCORE, QString::number(objectFeaturePtr->GetWeight() * 100, 'f', 2));
					modelItemPtr->setText(CT_POSITION, QString::number(objectFeaturePtr->GetPosition().GetX(), 'f', 2)+","+QString::number(objectFeaturePtr->GetPosition().GetY(), 'f', 2));
					modelItemPtr->setText(CT_ANGLE, QString::number(imath::GetDegreeFromRadian(objectFeaturePtr->GetAngle()), 'f', 2));
					modelItemPtr->setText(CT_SCALE, QString::number(objectFeaturePtr->GetScale().GetX(), 'f', 2) + "," + QString::number(objectFeaturePtr->GetScale().GetY(), 'f', 2));

					const iipr::CSearchFeature* searchFeaturePtr = dynamic_cast<const iipr::CSearchFeature*>(&providerPtr->GetNumericValue(featureIndex));
					if (searchFeaturePtr != NULL){
						modelItemPtr->setText(CT_ID, QString("%1 (%2)").arg(QString(objectFeaturePtr->GetObjectId())).arg(searchFeaturePtr->GetIndex()));
					
						if (searchFeaturePtr->IsNegativeModelEnabled()){
							modelItemPtr->setBackground(CT_ID, QBrush(Qt::red));
							modelItemPtr->setBackground(CT_SCORE, QBrush(Qt::red));
							modelItemPtr->setBackground(CT_POSITION, QBrush(Qt::red));
							modelItemPtr->setBackground(CT_ANGLE, QBrush(Qt::red));
							modelItemPtr->setBackground(CT_SCALE, QBrush(Qt::red));
						}
					}

					ResultsList->addTopLevelItem(modelItemPtr);

					// add shape
					if (*m_showResultShapesAttrPtr){
						VisualObject* visualObject = new VisualObject(false);
						visualObject->model->SetPosition(objectFeaturePtr->GetPosition());
						visualObject->model->SetRadius(qMax(5.0, maxScoreRadius * objectFeaturePtr->GetWeight()));
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

#if QT_VERSION < 0x050000
	ResultsList->header()->setResizeMode(QHeaderView::ResizeToContents);
#else
	ResultsList->header()->setSectionResizeMode(QHeaderView::ResizeToContents);
#endif

	if (m_intermediateResultsGuiCompPtr.IsValid()){
		m_intermediateResultsGuiCompPtr->CreateGui(IntermediateWidget);
	}
}


void CSearchBasedFeaturesSupplierGuiComp::OnGuiHidden()
{
	AutoTestButton->setChecked(false);

	if (m_intermediateResultsGuiCompPtr.IsValid()){
		m_intermediateResultsGuiCompPtr->DestroyGui();
	}

	BaseClass::OnGuiHidden();
}


// reimplemented (icomp::IComponentBase)

void CSearchBasedFeaturesSupplierGuiComp::OnComponentDestroyed()
{
	m_paramsObserver.EnsureModelDetached();

	BaseClass::OnComponentDestroyed();
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


