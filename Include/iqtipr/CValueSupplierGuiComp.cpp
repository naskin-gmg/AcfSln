#include "iqtipr/CValueSupplierGuiComp.h"


// Qt includes
#include <QtGui/QMessageBox>

// ACF includes
#include "imath/CVarVector.h"
#include "iview/CInteractiveCircleShape.h"
#include "iview/CInteractivePinShape.h"


namespace iqtipr
{


CValueSupplierGuiComp::CValueSupplierGuiComp()
:	m_paramsObserver(this)
{
}


// protected slots

void CValueSupplierGuiComp::on_TestButton_clicked()
{
	DoTest();
}


void CValueSupplierGuiComp::on_LoadParamsButton_clicked()
{
	LoadParams();
}


void CValueSupplierGuiComp::on_SaveParamsButton_clicked()
{
	SaveParams();
}


// protected methods

// reimplemented (iqtinsp::TSupplierGuiCompBase)

QWidget* CValueSupplierGuiComp::GetParamsWidget() const
{
	I_ASSERT(IsGuiCreated());

	return ParamsFrame;
}


// reimplemented (iqt2d::TSceneExtenderCompBase)

void CValueSupplierGuiComp::CreateShapes(int /*sceneId*/, Shapes& result)
{
	iview::CInteractiveCircleShape* circleShapePtr = new iview::CInteractiveCircleShape();
	if (circleShapePtr != NULL){
		circleShapePtr->SetVisible(false);

		m_foundModel.AttachObserver(circleShapePtr);

		result.PushBack(circleShapePtr);
	}

	iview::CInteractivePinShape* posShapePtr = new iview::CInteractivePinShape();
	if (posShapePtr != NULL){
		posShapePtr->SetVisible(false);

		m_foundModel.AttachObserver(posShapePtr);

		result.PushBack(posShapePtr);
	}
}


// reimplemented (iqtgui::TGuiObserverWrap)

void CValueSupplierGuiComp::OnGuiModelAttached()
{
	BaseClass::OnGuiModelAttached();

	ParamsGB->setVisible(AreParamsEditable() || IsLoadParamsSupported());
	LoadParamsButton->setVisible(IsLoadParamsSupported());
	SaveParamsButton->setVisible(IsSaveParamsSupported());
}


void CValueSupplierGuiComp::UpdateGui(int /*updateFlags*/)
{
	I_ASSERT(IsGuiCreated());

	imath::CVarVector position;
	imath::CVarVector radius;

	iproc::ISupplier* supplierPtr = GetObjectPtr();
	if (supplierPtr != NULL){
		int workStatus = supplierPtr->GetWorkStatus();
		if (workStatus == iproc::ISupplier::WS_OK){
			iproc::IValueProvider* providerPtr = dynamic_cast<iproc::IValueProvider*>(supplierPtr);
			if (providerPtr != NULL){
				position = providerPtr->GetValue(-1, iproc::IValueProvider::VTI_POSITION);
				radius = providerPtr->GetValue(-1, iproc::IValueProvider::VTI_RADIUS);
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

	bool isResultVisible = false;
	if (position.GetElementsCount() >= 2){
		m_foundModel.SetPosition(i2d::CVector2d(position[0], position[1]));

		isResultVisible = true;

		if (IsGuiCreated()){
			PositionLabel->setText(tr("(%1, %2)").arg(position[0]).arg(position[1]));
		}
	}
	else{
		m_foundModel.SetPosition(i2d::CVector2d(0, 0));

		if (IsGuiCreated()){
			PositionLabel->setText("No position");
		}
	}

	if (radius.GetElementsCount() >= 1){
		m_foundModel.SetRadius(radius[0]);
	}
	else{
		m_foundModel.SetRadius(0);
	}

	int shapesCount = m_foundModel.GetObserverCount();
	for (int i = 0; i < shapesCount; ++i){
		iview::CShapeControl* shapePtr = dynamic_cast<iview::CShapeControl*>(m_foundModel.GetObserverPtr(i));
		if (shapePtr != NULL){
			shapePtr->SetVisible(isResultVisible);
		}
	}

	UpdateAllViews();
}


// reimplemented (iqtgui::IGuiObject)

void CValueSupplierGuiComp::OnGuiCreated()
{
	BaseClass::OnGuiCreated();

	if (m_intermediateResultsGuiCompPtr.IsValid()){
		m_intermediateResultsGuiCompPtr->CreateGui(IntResultsFrame);

		IntResultsFrame->setVisible(true);
	}
	else{
		IntResultsFrame->setVisible(false);
	}
}


void CValueSupplierGuiComp::OnGuiDestroyed()
{
	if (m_intermediateResultsGuiCompPtr.IsValid()){
		m_intermediateResultsGuiCompPtr->DestroyGui();
	}
	
	BaseClass::OnGuiDestroyed();
}


// reimplemented (icomp::IComponentBase)

void CValueSupplierGuiComp::OnComponentDestroyed()
{
	m_paramsObserver.EnsureModelDetached();

	BaseClass::OnComponentDestroyed();
}


// public methods of embedded class ParamsObserver

CValueSupplierGuiComp::ParamsObserver::ParamsObserver(CValueSupplierGuiComp* parentPtr)
:	m_parent(*parentPtr)
{
	I_ASSERT(parentPtr != NULL);
}


// reimplemented (imod::CSingleModelObserverBase)

void CValueSupplierGuiComp::ParamsObserver::OnUpdate(int updateFlags, istd::IPolymorphic* /*updateParamsPtr*/)
{
	if (		((updateFlags & istd::IChangeable::CF_MODEL) != 0) &&
				m_parent.IsGuiCreated() &&
				m_parent.AutoUpdateButton->isChecked()){
		m_parent.DoTest();
	}
}


} // namespace iqtipr


