#include "iqtipr/CSearchBasedFeaturesSupplierGuiComp.h"


// Qt includes
#include <QtGui/QMessageBox>

// ACF includes
#include "imath/CVarVector.h"
#include "iview/CInteractiveCircleShape.h"
#include "iview/CInteractivePinShape.h"

// ACF-Solutions includes
#include "iipr/IFeaturesProvider.h"
#include "iipr/CSearchFeature.h"


namespace iqtipr
{


CSearchBasedFeaturesSupplierGuiComp::CSearchBasedFeaturesSupplierGuiComp()
:	m_paramsObserver(this)
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
	I_ASSERT(IsGuiCreated());

	return ParamsFrame;
}


// reimplemented (iqt2d::TSceneExtenderCompBase)

void CSearchBasedFeaturesSupplierGuiComp::CreateShapes(int /*sceneId*/, Shapes& result)
{
	int foundPositionsCount = m_foundPositions.GetCount();
	
	for (int positionIndex = 0; positionIndex < foundPositionsCount; positionIndex++){
		iview::CInteractiveCircleShape* circleShapePtr = new iview::CInteractiveCircleShape();

		m_foundPositions.GetAt(positionIndex)->AttachObserver(circleShapePtr);

		result.PushBack(circleShapePtr);
	}
}


// reimplemented (iqtgui::TGuiObserverWrap)

void CSearchBasedFeaturesSupplierGuiComp::OnGuiModelAttached()
{
	BaseClass::OnGuiModelAttached();

	ParamsGB->setVisible(AreParamsEditable() || IsLoadParamsSupported());
	LoadParamsButton->setVisible(IsLoadParamsSupported());
	SaveParamsButton->setVisible(IsSaveParamsSupported());
}


void CSearchBasedFeaturesSupplierGuiComp::UpdateGui(int /*updateFlags*/)
{
	I_ASSERT(IsGuiCreated());

	ResultsList->clear();
	m_foundPositions.Reset();

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
						modelItemPtr->setText(CT_SCORE, QString::number(searchFeaturePtr->GetWeight()));
						modelItemPtr->setText(CT_X, QString::number(searchFeaturePtr->GetPosition().GetX()));
						modelItemPtr->setText(CT_Y, QString::number(searchFeaturePtr->GetPosition().GetY()));
						modelItemPtr->setText(CT_ANGLE, QString::number(searchFeaturePtr->GetAngle()));
						modelItemPtr->setText(CT_X_SCALE, QString::number(searchFeaturePtr->GetScale().GetX()));
						modelItemPtr->setText(CT_Y_SCALE, QString::number(searchFeaturePtr->GetScale().GetY()));

						ResultsList->addTopLevelItem(modelItemPtr);

						PositionModel* searchShapeModelPtr = new PositionModel;
						searchShapeModelPtr->SetPosition(searchFeaturePtr->GetPosition());
						searchShapeModelPtr->SetRadius(qMax(5.0, maxScoreRadius * searchFeaturePtr->GetWeight()));

						m_foundPositions.PushBack(searchShapeModelPtr);
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

	UpdateAllViews();
}


// reimplemented (iqtgui::IGuiObject)

void CSearchBasedFeaturesSupplierGuiComp::OnGuiCreated()
{
	BaseClass::OnGuiCreated();
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


