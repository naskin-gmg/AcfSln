#include "iqtfpf/CProcessingItemPreviewGuiComp.h"


// ACF includes
#include "istd/TChangeNotifier.h"

#include "iqt/CSignalBlocker.h"


namespace iqtfpf
{


// public methods

// reimplemented (imod::IModelEditor)

void CProcessingItemPreviewGuiComp::UpdateModel() const
{
}


void CProcessingItemPreviewGuiComp::UpdateEditor(int /*updateFlags*/)
{
	ifpf::IHotfolderProcessingItem* objectPtr = GetObjectPtr();
	if (objectPtr != NULL){
	}
}


// reimplemented TGuiObserverWrap

void CProcessingItemPreviewGuiComp::OnGuiModelAttached()
{
	BaseClass::OnGuiModelAttached();

	if (m_inputPreviewObserverCompPtr.IsValid()){
		ifpf::IHotfolderProcessingItem* objectPtr = GetObjectPtr();
		if (objectPtr != NULL){
			const imod::IModel* modelPtr = dynamic_cast<const imod::IModel*>(&objectPtr->GetInputPreview());
			if (modelPtr != NULL){
				(const_cast<imod::IModel*>(modelPtr))->AttachObserver(m_inputPreviewObserverCompPtr.GetPtr());
			}
		}
	}

	if (m_outputPreviewObserverCompPtr.IsValid()){
		ifpf::IHotfolderProcessingItem* objectPtr = GetObjectPtr();
		if (objectPtr != NULL){
			const imod::IModel* modelPtr = dynamic_cast<const imod::IModel*>(&objectPtr->GetOutputPreview());
			if (modelPtr != NULL){
				(const_cast<imod::IModel*>(modelPtr))->AttachObserver(m_outputPreviewObserverCompPtr.GetPtr());
			}
		}
	}
}


void CProcessingItemPreviewGuiComp::OnGuiModelDetached()
{
	if (m_inputPreviewObserverCompPtr.IsValid()){
		ifpf::IHotfolderProcessingItem* objectPtr = GetObjectPtr();
		if (objectPtr != NULL){
			const imod::IModel* modelPtr = dynamic_cast<const imod::IModel*>(&objectPtr->GetInputPreview());
			if (modelPtr != NULL && modelPtr->IsAttached(m_inputPreviewObserverCompPtr.GetPtr())){
				(const_cast<imod::IModel*>(modelPtr))->DetachObserver(m_inputPreviewObserverCompPtr.GetPtr());
			}
		}
	}

	if (m_outputPreviewObserverCompPtr.IsValid()){
		ifpf::IHotfolderProcessingItem* objectPtr = GetObjectPtr();
		if (objectPtr != NULL){
			const imod::IModel* modelPtr = dynamic_cast<const imod::IModel*>(&objectPtr->GetOutputPreview());
			if (modelPtr != NULL && modelPtr->IsAttached(m_outputPreviewObserverCompPtr.GetPtr())){
				(const_cast<imod::IModel*>(modelPtr))->DetachObserver(m_outputPreviewObserverCompPtr.GetPtr());
			}
		}
	}

	BaseClass::OnGuiModelDetached();
}

// reimplemented (iqtgui::CGuiComponentBase)

void CProcessingItemPreviewGuiComp::OnGuiCreated()
{
	BaseClass::OnGuiCreated();

	if (m_inputPreviewObserverCompPtr.IsValid() && m_inputPreviewGuiCompPtr.IsValid()){
		m_inputPreviewGuiCompPtr->CreateGui(InputPreviewFrame);
	}

	if (m_outputPreviewObserverCompPtr.IsValid() && m_outputPreviewGuiCompPtr.IsValid()){
		m_outputPreviewGuiCompPtr->CreateGui(OutputPreviewFrame);
	}
}


void CProcessingItemPreviewGuiComp::OnGuiDestroyed()
{
	if (m_inputPreviewGuiCompPtr.IsValid()){
		m_inputPreviewGuiCompPtr->DestroyGui();
	}

	if (m_outputPreviewGuiCompPtr.IsValid()){
		m_outputPreviewGuiCompPtr->DestroyGui();
	}

	BaseClass::OnGuiDestroyed();
}


} // namespace iqtfpf


