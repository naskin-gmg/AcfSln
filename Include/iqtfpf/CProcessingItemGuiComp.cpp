#include "iqtfpf/CProcessingItemGuiComp.h"


// ACF includes
#include "istd/TChangeNotifier.h"

#include "iqt/CSignalBlocker.h"


namespace iqtfpf
{


// public methods

// reimplemented (imod::IModelEditor)

void CProcessingItemGuiComp::UpdateModel() const
{
}


void CProcessingItemGuiComp::UpdateEditor(int /*updateFlags*/)
{
	ifpf::IHotfolderProcessingItem* objectPtr = GetObjectPtr();
	if (objectPtr != NULL){
		iqt::CSignalBlocker block(this, true);

		InputFileEdit->setText(iqt::GetQString(objectPtr->GetInputFile()));
		OutputFileEdit->setText(iqt::GetQString(objectPtr->GetOutputFile()));
	}
}


// reimplemented (iqtgui::CGuiComponentBase)

void CProcessingItemGuiComp::OnGuiCreated()
{
	BaseClass::OnGuiCreated();

	if (m_inputPreviewObserverCompPtr.IsValid() && m_inputPreviewGuiCompPtr.IsValid()){
		m_inputPreviewGuiCompPtr->CreateGui(InputPreviewFrame);
	}

	if (m_outputPreviewObserverCompPtr.IsValid() && m_outputPreviewGuiCompPtr.IsValid()){
		m_outputPreviewGuiCompPtr->CreateGui(OutputPreviewFrame);
	}
}


void CProcessingItemGuiComp::OnGuiDestroyed()
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


