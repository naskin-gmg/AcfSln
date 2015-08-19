#include "iqtipr/CLineProjectionSupplierGuiComp.h"


// ACF includes
#include "imath/CVarVector.h"

#include "istd/CChangeNotifier.h"
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
	Q_ASSERT(IsGuiCreated());

	return ParamsFrame;
}


// reimplemented (iqt2d::TViewExtenderCompBase)

void CLineProjectionSupplierGuiComp::CreateShapes(int /*sceneId*/, Shapes& result)
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


void CLineProjectionSupplierGuiComp::UpdateGui(const istd::IChangeable::ChangeSet& changeSet)
{
	BaseClass::UpdateGui(changeSet);

	Q_ASSERT(IsGuiCreated());

	const imeas::IDataSequence* projectionPtr = NULL;

	const imeas::IDataSequenceProvider* providerPtr = dynamic_cast<const imeas::IDataSequenceProvider*>(GetObjectPtr());
	if (providerPtr != NULL){
		projectionPtr = providerPtr->GetDataSequence();
	}

	if (projectionPtr != NULL){
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


