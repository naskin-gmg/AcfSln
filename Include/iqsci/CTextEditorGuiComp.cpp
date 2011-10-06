#include "iqsci/CTextEditorGuiComp.h"


namespace iqsci
{


// reimplemented (imod::IModelEditor)

void CTextEditorGuiComp::UpdateModel() const
{
	I_ASSERT(IsGuiCreated());

	ibase::ITextDocument* objectPtr = GetObjectPtr();
	I_ASSERT(objectPtr != NULL);

	CTextEditor* textEditPtr = GetQtWidget();
	I_ASSERT(textEditPtr != NULL);

	objectPtr->SetText(iqt::GetCString(textEditPtr->GetText()));
}


// reimplemented (iqtgui::TGuiObserverWrap)

void CTextEditorGuiComp::UpdateGui(int /*updateFlags*/)
{
	CTextEditor* textEditPtr = GetQtWidget();
	I_ASSERT(textEditPtr != NULL);

	ibase::ITextDocument* objectPtr = GetObjectPtr();
	if (objectPtr != NULL){
		textEditPtr->SetText(iqt::GetQString(objectPtr->GetText()));
	}
}


void CTextEditorGuiComp::OnGuiModelDetached()
{
	CTextEditor* textEditPtr = GetQtWidget();
	I_ASSERT(textEditPtr != NULL);

	textEditPtr->SetText(QString());

	BaseClass::OnGuiModelDetached();
}


// reimplemented (ibase::ICommandsProvider)

const ibase::IHierarchicalCommand* CTextEditorGuiComp::GetCommands() const
{
	if (IsGuiCreated()){
		CTextEditor* textEditPtr = GetQtWidget();
		I_ASSERT(textEditPtr != NULL);
		
		return textEditPtr->GetCommands();
	}

	return NULL;
}


// protected methods

// reimplemented (iqtgui::CGuiComponentBase)

void CTextEditorGuiComp::OnGuiCreated()
{
	BaseClass::OnGuiCreated();

	CTextEditor* textEditorPtr = GetQtWidget();
	I_ASSERT(textEditorPtr != NULL);
	if (textEditorPtr != NULL){
		connect(textEditorPtr, SIGNAL(DataChanged()), this, SLOT(OnTextChanged()));

		if (m_useFoldingAttrPtr.IsValid()){
			textEditorPtr->SetFoldingEnabled(*m_useFoldingAttrPtr);
		}

		if (m_languageAttrPtr.IsValid()){
			textEditorPtr->SetLanguage(iqt::GetQString(*m_languageAttrPtr));
		}

		if (m_readOnlyAttrPtr.IsValid() && *m_readOnlyAttrPtr){
			textEditorPtr->SetReadOnly();
		}
	}
}


void CTextEditorGuiComp::OnGuiRetranslate()
{
	BaseClass::OnGuiRetranslate();

	if (IsGuiCreated()){
		CTextEditor* textEditPtr = GetQtWidget();
		I_ASSERT(textEditPtr != NULL);
	
		textEditPtr->OnRetranslate();
	}
}


// protected slots

void CTextEditorGuiComp::OnTextChanged()
{
	UpdateBlocker updateBlocker(this);

	UpdateModel();
}


} // namespace iqsci


