#include "CEdgeLinesSupplierGuiComp.h"


// Qt includes
#include <QtGui/QMessageBox>

// ACF includes
#include "imath/CVarVector.h"

// ACF-Solutions includes
#include "iedge/IEdgeLinesProvider.h"
#include "iedge/CEdgeLine.h"

#include "iedgegui/CEdgeLineContainerShape.h"


namespace iedgegui
{


CEdgeLinesSupplierGuiComp::CEdgeLinesSupplierGuiComp()
{
	m_edgesColorSchema.SetPen(iview::IColorSchema::SP_NORMAL, QPen(Qt::blue));
}


// protected slots

void CEdgeLinesSupplierGuiComp::on_TestButton_clicked()
{
	DoTest();
}


void CEdgeLinesSupplierGuiComp::on_LoadParamsButton_clicked()
{
	LoadParams();
}


void CEdgeLinesSupplierGuiComp::on_SaveParamsButton_clicked()
{
	SaveParams();
}


// protected methods

// reimplemented (iqtinsp::TSupplierGuiCompBase)

QWidget* CEdgeLinesSupplierGuiComp::GetParamsWidget() const
{
	Q_ASSERT(IsGuiCreated());

	return ParamsFrame;
}


void CEdgeLinesSupplierGuiComp::OnSupplierParamsChanged()
{
	if (IsGuiCreated() && AutoUpdateButton->isChecked()){
		DoTest();
	}
}


// reimplemented (iqt2d::TViewExtenderCompBase)

void CEdgeLinesSupplierGuiComp::CreateShapes(int /*sceneId*/, Shapes& result)
{
	iedgegui::CEdgeLineContainerShape* shapePtr = new iedgegui::CEdgeLineContainerShape(); 

	if (shapePtr != NULL){
		if (m_contoursColorSchemaCompPtr.IsValid()){
			shapePtr->SetUserColorSchema(m_contoursColorSchemaCompPtr.GetPtr());
		}
		else{
			shapePtr->SetUserColorSchema(&m_edgesColorSchema);
		}

		shapePtr->AssignToLayer(iview::IViewLayer::LT_INACTIVE);

		m_foundModel.AttachObserver(shapePtr);

		result.PushBack(shapePtr);
	}	
}


// reimplemented (iqtgui::TGuiObserverWrap)

void CEdgeLinesSupplierGuiComp::OnGuiModelAttached()
{
	BaseClass::OnGuiModelAttached();

	ParamsGB->setVisible(AreParamsEditable() || IsLoadParamsSupported());
	LoadParamsButton->setVisible(IsLoadParamsSupported());
	SaveParamsButton->setVisible(IsSaveParamsSupported());
}


void CEdgeLinesSupplierGuiComp::UpdateGui(int updateFlags)
{
	BaseClass::UpdateGui(updateFlags);

	Q_ASSERT(IsGuiCreated());

	UpdateAllViews();
}


// reimplemented (iqtgui::IGuiObject)

void CEdgeLinesSupplierGuiComp::OnGuiCreated()
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


void CEdgeLinesSupplierGuiComp::OnGuiDestroyed()
{
	if (m_intermediateResultsGuiCompPtr.IsValid()){
		m_intermediateResultsGuiCompPtr->DestroyGui();
	}
	
	BaseClass::OnGuiDestroyed();
}


void CEdgeLinesSupplierGuiComp::OnGuiHidden()
{
	AutoUpdateButton->setChecked(false);

	BaseClass::OnGuiHidden();
}


// reimplemented (imod::IObserver)

void CEdgeLinesSupplierGuiComp::AfterUpdate(imod::IModel* modelPtr, int updateFlags, istd::IPolymorphic* updateParamsPtr)
{
	iedge::IEdgeLinesProvider* providerPtr = CompCastPtr<iedge::IEdgeLinesProvider>(GetObjectPtr());
	if (providerPtr != NULL ){
		const iedge::CEdgeLine::Container* resultContainerPtr = providerPtr->GetEdgesContainer();	

		if ((resultContainerPtr == NULL) || ! m_foundModel.CopyFrom(*resultContainerPtr)){
			m_foundModel.Reset();
		}
	}

	BaseClass::AfterUpdate(modelPtr, updateFlags, updateParamsPtr);
}


}//Namespace iedgegui





