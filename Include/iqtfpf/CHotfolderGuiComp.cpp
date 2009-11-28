#include "iqtfpf/CHotfolderGuiComp.h"


// Qt includes
#include <QHeaderView>


// ACF includes
#include "istd/TChangeNotifier.h"

#include "iqt/CSignalBlocker.h"

#include "iqtgui/CItemDelegate.h"


namespace iqtfpf
{



// reimplemented (idoc::ICommandsProvider)

const idoc::IHierarchicalCommand* CHotfolderGuiComp::GetCommands() const
{
	return &m_hotfolderCommands;
}


// reimplemented (imod::IModelEditor)

void CHotfolderGuiComp::UpdateModel() const
{
	ifpf::IHotfolder* objectPtr = GetObjectPtr();
	if (objectPtr != NULL && !IsUpdateBlocked()){
		UpdateBlocker blocker(const_cast<CHotfolderGuiComp*>(this));
	}
}


void CHotfolderGuiComp::UpdateEditor(int /*updateFlags*/)
{
	ifpf::IHotfolder* objectPtr = GetObjectPtr();
	if (objectPtr != NULL){
		iqt::CSignalBlocker block(this, true);
	}
}


// reimplemented TGuiObserverWrap

void CHotfolderGuiComp::OnGuiModelAttached()
{
	if (m_settingsObserverCompPtr.IsValid()){
		ifpf::IHotfolder* objectPtr = GetObjectPtr();
		imod::IModel* paramsModelPtr = dynamic_cast<imod::IModel*>(objectPtr->GetHotfolderParams());
		if (paramsModelPtr != NULL){
			paramsModelPtr->AttachObserver(m_settingsObserverCompPtr.GetPtr());
		}
	}
	
	BaseClass::OnGuiModelAttached();
}


void CHotfolderGuiComp::OnGuiModelDetached()
{
	if (m_settingsObserverCompPtr.IsValid()){
		ifpf::IHotfolder* objectPtr = GetObjectPtr();
		imod::IModel* paramsModelPtr = dynamic_cast<imod::IModel*>(objectPtr->GetHotfolderParams());
		if (paramsModelPtr != NULL){
			paramsModelPtr->DetachObserver(m_settingsObserverCompPtr.GetPtr());
		}
	}

	BaseClass::OnGuiModelDetached();

}


// reimplemented (iqtgui::CGuiComponentBase)

void CHotfolderGuiComp::OnGuiCreated()
{
	BaseClass::OnGuiCreated();

	if (m_progressManagerGuiCompPtr.IsValid() && m_progressManagerCompPtr.IsValid()){
		m_progressManagerGuiCompPtr->CreateGui(OverallProgressFrame);
	}

	iqtgui::CHierarchicalCommand* hotfolderMenuPtr = new iqtgui::CHierarchicalCommand("&Hotfolder");

	iqtgui::CHierarchicalCommand* settingsCommandPtr = new iqtgui::CHierarchicalCommand("&Settings...");
	settingsCommandPtr->SetStaticFlags(iqtgui::CHierarchicalCommand::CF_GLOBAL_MENU);
	connect(settingsCommandPtr, SIGNAL(activated()), this, SLOT(OnSettings()));
	hotfolderMenuPtr->InsertChild(settingsCommandPtr, true);

	m_hotfolderCommands.InsertChild(hotfolderMenuPtr, true);

	if (m_settingsGuiCompPtr.IsValid()){
		m_settingsDialogPtr.SetPtr(new iqtgui::CGuiComponentDialog(m_settingsGuiCompPtr.GetPtr(), 0, true, GetWidget())); 
	}
}


void CHotfolderGuiComp::OnGuiDestroyed()
{
	m_settingsDialogPtr.Reset();

	BaseClass::OnGuiDestroyed();
}


// protected slots

void CHotfolderGuiComp::OnSettings()
{
	if (m_settingsDialogPtr.IsValid()){
		m_settingsDialogPtr->exec();
	}
}

} // namespace iqtfpf


