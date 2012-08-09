#include "CEdgeLinesSupplierGuiComp.h"


// Qt includes
#include <QtGui/QMessageBox>

// ACF includes
#include "imath/CVarVector.h"
#include "iview/CInteractiveCircleShape.h"
#include "iview/CInteractivePinShape.h"

#include "iproc/IElapsedTimeProvider.h"

#include "iedge/IEdgeLinesProvider.h"

// QSF includes
#include "iedge/CEdgeLine.h"

#include "iedgegui/CEdgeLineContainerShape.h"

namespace iedgegui{

CEdgeLinesSupplierGuiComp::CEdgeLinesSupplierGuiComp()
:	m_paramsObserver(this)
{
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
	I_ASSERT(IsGuiCreated());

	return ParamsFrame;
}


// reimplemented (iqt2d::TViewExtenderCompBase)

void CEdgeLinesSupplierGuiComp::CreateShapes(int /*sceneId*/, Shapes& result)
{
	iedgegui::CEdgeLineContainerShape* lineContainerShapePtr = new iedgegui::CEdgeLineContainerShape(); 

	if(lineContainerShapePtr != NULL){
		
		m_foundModel.AttachObserver(lineContainerShapePtr);

		result.PushBack(lineContainerShapePtr);
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

	I_ASSERT(IsGuiCreated());

	iproc::ISupplier* supplierPtr = GetObjectPtr();
	if (supplierPtr != NULL){
		int workStatus = supplierPtr->GetWorkStatus();
		if (workStatus == iproc::ISupplier::WS_OK){
			iedge::IEdgeLinesProvider* providerPtr = dynamic_cast<iedge::IEdgeLinesProvider*>(supplierPtr);
			if (providerPtr != NULL ){
				const iedge::CEdgeLine::Container* resultValue = providerPtr->GetEdgesContainer();	

				m_foundModel.Reset();

				int numLines = resultValue->GetItemsCount();

				for(int lineIndex = 0; lineIndex < numLines; lineIndex++){
					const iedge::CEdgeLine& line = resultValue->GetAt(lineIndex);					
					m_foundModel.PushBack(iedge::CEdgeLine(line));
				}
				
			}
		}
	}
		
	bool isResultVisible = true;

	int shapesCount = m_foundModel.GetObserverCount();
	for (int i = 0; i < shapesCount; ++i){
		iview::CShapeBase* shapePtr = dynamic_cast<iview::CShapeBase*>(m_foundModel.GetObserverPtr(i));
		if (shapePtr != NULL){
			shapePtr->SetVisible(isResultVisible);
		}
	}

	UpdateAllViews();
}


// reimplemented (iqtgui::IGuiObject)

void CEdgeLinesSupplierGuiComp::OnGuiCreated()
{
	BaseClass::OnGuiCreated();

	ProcessingTimeLabel->setVisible(false);

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


// reimplemented (icomp::IComponentBase)

void CEdgeLinesSupplierGuiComp::OnComponentDestroyed()
{
	m_paramsObserver.EnsureModelDetached();

	BaseClass::OnComponentDestroyed();
}


// public methods of embedded class ParamsObserver

CEdgeLinesSupplierGuiComp::ParamsObserver::ParamsObserver(CEdgeLinesSupplierGuiComp* parentPtr)
:	m_parent(*parentPtr)
{
	I_ASSERT(parentPtr != NULL);
}


// reimplemented (imod::CSingleModelObserverBase)

void CEdgeLinesSupplierGuiComp::ParamsObserver::OnUpdate(int updateFlags, istd::IPolymorphic* /*updateParamsPtr*/)
{
	if (		((updateFlags & istd::IChangeable::CF_MODEL) != 0) &&
				m_parent.IsGuiCreated() &&
				m_parent.AutoUpdateButton->isChecked()){
		m_parent.DoTest();
	}
}

}//Namespace iedgegui





