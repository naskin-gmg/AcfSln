#include "iqtfpf/CComposedHotfolderGuiComp.h"


// Qt includes
#include <QProgressBar>
#include <QVBoxLayout>


namespace iqtfpf
{


// reimplemented (idoc::ICommandsProvider)

const idoc::IHierarchicalCommand* CComposedHotfolderGuiComp::GetCommands() const
{
	if (m_hotfolderGuiCompPtr.IsValid()){
		idoc::ICommandsProvider* commandsProviderPtr = dynamic_cast<idoc::ICommandsProvider*>(m_hotfolderGuiCompPtr.GetPtr());
		if (commandsProviderPtr != NULL){
			const iqtgui::CHierarchicalCommand* rootCommands = dynamic_cast<const iqtgui::CHierarchicalCommand*>(commandsProviderPtr->GetCommands());
			if (rootCommands != NULL){
				(const_cast<iqtgui::CHierarchicalCommand*>(rootCommands))->JoinLinkFrom(&m_commands);

				return rootCommands;
			}
		}
	}

	return &m_commands;
}


// reimplemented (imod::IModelEditor)

void CComposedHotfolderGuiComp::UpdateModel() const
{
}


void CComposedHotfolderGuiComp::UpdateEditor(int /*updateFlags*/)
{
}


// reimplemented TGuiObserverWrap

void CComposedHotfolderGuiComp::OnGuiModelAttached()
{
	if (m_settingsObserverCompPtr.IsValid() && m_hotfolderParamsIdAttrPtr.IsValid()){
		iprm::IParamsSet* objectPtr = GetObjectPtr();
		I_ASSERT(objectPtr != NULL);
		if (objectPtr != NULL){
			imod::IModel* paramsModelPtr = dynamic_cast<imod::IModel*>(objectPtr->GetEditableParameter((*m_hotfolderParamsIdAttrPtr).ToString()));
			if (paramsModelPtr != NULL){
				paramsModelPtr->AttachObserver(m_settingsObserverCompPtr.GetPtr());
			}
		}
	}

	if (m_hotfolderObserverCompPtr.IsValid() && m_stateModelIdAttrPtr.IsValid()){
		iprm::IParamsSet* objectPtr = GetObjectPtr();
		I_ASSERT(objectPtr != NULL);
		if (objectPtr != NULL){
			imod::IModel* paramsModelPtr = dynamic_cast<imod::IModel*>(objectPtr->GetEditableParameter((*m_stateModelIdAttrPtr).ToString()));
			if (paramsModelPtr != NULL && m_hotfolderGuiCompPtr.IsValid()){
				QWidget* widgetPtr = GetWidget();
				QVBoxLayout* layout = new QVBoxLayout(widgetPtr);
				layout->setMargin(0);

				if (m_hotfolderGuiCompPtr->CreateGui(widgetPtr)){
					paramsModelPtr->AttachObserver(m_hotfolderObserverCompPtr.GetPtr());
				}
			}
		}
	}

	BaseClass::OnGuiModelAttached();
}


void CComposedHotfolderGuiComp::OnGuiModelDetached()
{
	if (m_settingsObserverCompPtr.IsValid()){
		iprm::IParamsSet* objectPtr = GetObjectPtr();
		imod::IModel* paramsModelPtr = dynamic_cast<imod::IModel*>(objectPtr->GetEditableParameter((*m_hotfolderParamsIdAttrPtr).ToString()));
		if (paramsModelPtr != NULL && paramsModelPtr->IsAttached(m_settingsObserverCompPtr.GetPtr())){
			paramsModelPtr->DetachObserver(m_settingsObserverCompPtr.GetPtr());
		}
	}

	if (m_hotfolderObserverCompPtr.IsValid()){
		iprm::IParamsSet* objectPtr = GetObjectPtr();
		imod::IModel* paramsModelPtr = dynamic_cast<imod::IModel*>(objectPtr->GetEditableParameter((*m_stateModelIdAttrPtr).ToString()));
		if (paramsModelPtr != NULL && paramsModelPtr->IsAttached(m_hotfolderObserverCompPtr.GetPtr())){
			paramsModelPtr->DetachObserver(m_hotfolderObserverCompPtr.GetPtr());
		}
	}

	BaseClass::OnGuiModelDetached();
}


// reimplemented (iqtgui::CGuiComponentBase)

void CComposedHotfolderGuiComp::OnGuiCreated()
{
	iqtgui::CHierarchicalCommand* hotfolderMenuPtr = new iqtgui::CHierarchicalCommand("&Hotfolder");
	iqtgui::CHierarchicalCommand* settingsCommandPtr = new iqtgui::CHierarchicalCommand();
	settingsCommandPtr->SetGroupId(20);
	settingsCommandPtr->SetStaticFlags(iqtgui::CHierarchicalCommand::CF_GLOBAL_MENU | iqtgui::CHierarchicalCommand::CF_TOOLBAR);
	settingsCommandPtr->SetVisuals(tr("&Settings..."), "Settings...", "Edit setting of the hotfolder", QIcon(":/Icons/Settings.svg"));
	connect(settingsCommandPtr, SIGNAL(activated()), this, SLOT(OnSettings()));
	
	hotfolderMenuPtr->InsertChild(settingsCommandPtr, true);

	m_commands.InsertChild(hotfolderMenuPtr, true);

	if (m_settingsGuiCompPtr.IsValid()){
		m_settingsDialogPtr.SetPtr(new iqtgui::CGuiComponentDialog(m_settingsGuiCompPtr.GetPtr(), 0, true, GetWidget()));
		m_settingsDialogPtr->setWindowTitle(tr("Hotfolder Settings"));
		m_settingsDialogPtr->setWindowIcon(QIcon(":/Icons/Settings.svg"));
	}

	BaseClass::OnGuiCreated();
}


void CComposedHotfolderGuiComp::OnGuiDestroyed()
{
	m_settingsDialogPtr.Reset();

	if (m_hotfolderGuiCompPtr.IsValid()){
		m_hotfolderGuiCompPtr->DestroyGui();
	}

	BaseClass::OnGuiDestroyed();
}


// private methods

// private slots

void CComposedHotfolderGuiComp::OnSettings()
{
	if (m_settingsDialogPtr.IsValid()){
		m_settingsDialogPtr->exec();
	}
}


} // namespace iqtfpf


