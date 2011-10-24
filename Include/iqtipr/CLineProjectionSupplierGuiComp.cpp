#include "iqtipr/CLineProjectionSupplierGuiComp.h"


// Qt includes
#include <QMessageBox>


// ACF includes
#include "imath/CVarVector.h"

#include "istd/TChangeNotifier.h"
#include "iser/CMemoryReadArchive.h"


// IACF includes
#include "iqtipr/CProjectionShape.h"


namespace iqtipr
{


// protected slots

void CLineProjectionSupplierGuiComp::on_TestButton_clicked()
{
	DoTest();
}


void CLineProjectionSupplierGuiComp::on_LoadParamsButton_clicked()
{
	LoadParams();
}


void CLineProjectionSupplierGuiComp::on_SaveParamsButton_clicked()
{
	SaveParams();
}


// protected methods

// reimplemented (iqtinsp::TSupplierGuiCompBase)

QWidget* CLineProjectionSupplierGuiComp::GetParamsWidget() const
{
	I_ASSERT(IsGuiCreated());

	return ParamsFrame;
}


// reimplemented (iqt2d::TSceneExtenderCompBase)

void CLineProjectionSupplierGuiComp::CreateShapes(int /*sceneId*/, bool /*inactiveOnly*/, Shapes& result)
{
 	iqtipr::CProjectionShape* shapePtr = new iqtipr::CProjectionShape;
	if (shapePtr != NULL){
		result.PushBack(shapePtr);
	
		m_projectionData.AttachObserver(shapePtr);
	}
}


// reimplemented (iqtgui::TGuiObserverWrap)

void CLineProjectionSupplierGuiComp::OnGuiModelAttached()
{
	BaseClass::OnGuiModelAttached();

	ParamsGB->setVisible(AreParamsEditable() || IsLoadParamsSupported());
	LoadParamsButton->setVisible(IsLoadParamsSupported());
	SaveParamsButton->setVisible(IsSaveParamsSupported());

	if (m_projectionObserverCompPtr.IsValid()){
		m_projectionData.AttachObserver(m_projectionObserverCompPtr.GetPtr());
	}
}


void CLineProjectionSupplierGuiComp::UpdateGui(int /*updateFlags*/)
{
	I_ASSERT(IsGuiCreated());

	const imeas::IDataSequence* projectionPtr = NULL;

	iproc::ISupplier* supplierPtr = GetObjectPtr();
	if (supplierPtr != NULL){
		int workStatus = supplierPtr->GetWorkStatus();
		if (workStatus == iproc::ISupplier::WS_OK){
			const iipr::IDataSequenceProvider* providerPtr = dynamic_cast<const iipr::IDataSequenceProvider*>(supplierPtr);
			if (providerPtr != NULL){
				projectionPtr = providerPtr->GetDataSequence();
			}
		}
	}

	if (projectionPtr != NULL){
		istd::CChangeNotifier changePtr(&m_projectionData);

		m_projectionData.CopyFrom(*projectionPtr);
	}
	else{
		m_projectionData.ResetSequence();
	}
}


// reimplemented (iqtgui::IGuiObject)

void CLineProjectionSupplierGuiComp::OnGuiCreated()
{
	BaseClass::OnGuiCreated();

	if (m_projectionObserverGuiCompPtr.IsValid()){
		m_projectionObserverGuiCompPtr->CreateGui(ProjectionFrame);
	}
}


void CLineProjectionSupplierGuiComp::OnGuiDestroyed()
{
	if (m_projectionObserverGuiCompPtr.IsValid()){
		m_projectionObserverGuiCompPtr->DestroyGui();
	}
	
	BaseClass::OnGuiDestroyed();
}


} // namespace iqtipr


