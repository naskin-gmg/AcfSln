#include "iqtfpf/CHotfolderGuiComp.h"


// Qt includes
#include <QHeaderView>
#include <QProgressBar>


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


void CHotfolderGuiComp::UpdateEditor(int updateFlags)
{
	ifpf::IHotfolder* objectPtr = GetObjectPtr();
	if (objectPtr != NULL){
		iqt::CSignalBlocker block(this, true);

		if ((updateFlags & ifpf::IHotfolder::CF_CREATE) != 0){
			int itemsCount = objectPtr->GetProcessingItemsCount();
			for (int itemIndex = 0; itemIndex < itemsCount; itemIndex++){
				ifpf::IHotfolderProcessingItem* pocessingItem = objectPtr->GetProcessingItem(itemIndex);

				AddFileItem(*pocessingItem);
			}
		}

		if ((updateFlags & ifpf::IHotfolder::CF_FILE_ADDED) != 0){
			int itemsCount = objectPtr->GetProcessingItemsCount();
			if (itemsCount > 0){
				ifpf::IHotfolderProcessingItem* pocessingItem = objectPtr->GetProcessingItem(itemsCount - 1);

				AddFileItem(*pocessingItem);
			}
		}

		if ((updateFlags & ifpf::IHotfolder::CF_WORKING_STATE_CHANGED) != 0){
			UpdateProcessingCommands();
		}
	}
}


// reimplemented (iqtgui::CGuiComponentBase)

void CHotfolderGuiComp::OnGuiCreated()
{
	BaseClass::OnGuiCreated();

	if (m_progressManagerGuiCompPtr.IsValid() && m_progressManagerCompPtr.IsValid()){
		m_progressManagerGuiCompPtr->CreateGui(OverallProgressFrame);
	}

	iqtgui::CHierarchicalCommand* hotfolderMenuPtr = new iqtgui::CHierarchicalCommand("&Hotfolder");

	m_runCommand.SetGroupId(1);
	m_runCommand.SetStaticFlags(iqtgui::CHierarchicalCommand::CF_GLOBAL_MENU | iqtgui::CHierarchicalCommand::CF_TOOLBAR);
	m_runCommand.SetVisuals(tr("&Run"), "Run", tr("Start/Continue the execution of the hotfolder"), QIcon(":/Icons/PlayerPlay"));
	connect(&m_runCommand, SIGNAL(activated()), this, SLOT(OnRun()));
	hotfolderMenuPtr->InsertChild(&m_runCommand, false);

	m_holdCommand.SetGroupId(1);
	m_holdCommand.SetStaticFlags(iqtgui::CHierarchicalCommand::CF_GLOBAL_MENU | iqtgui::CHierarchicalCommand::CF_TOOLBAR);
	m_holdCommand.SetVisuals(tr("&Hold"), "Hold", tr("Hold the execution of the hotfolder"), QIcon(":/Icons/PlayerPause"));
	m_holdCommand.setDisabled(true);
	connect(&m_holdCommand, SIGNAL(activated()), this, SLOT(OnHold()));
	hotfolderMenuPtr->InsertChild(&m_holdCommand, false);

	m_hotfolderCommands.InsertChild(hotfolderMenuPtr, true);

	UpdateProcessingCommands();

	// some visual details:
	FileList->header()->setResizeMode(QHeaderView::ResizeToContents);
	FileList->header()->setStretchLastSection(true);

	iqtgui::CItemDelegate* itemDelegate = new iqtgui::CItemDelegate(25, this);
	FileList->setItemDelegate(itemDelegate);

	FileList->header()->setResizeMode(0, QHeaderView::Fixed);
	FileList->header()->resizeSection(0, itemDelegate->GetItemHeight());

}


// private methods

void CHotfolderGuiComp::AddFileItem(const ifpf::IHotfolderProcessingItem& fileItem)
{
	ProcessingItem* fileItemPtr = new ProcessingItem(m_stateIconsProviderCompPtr.GetPtr());
	fileItemPtr->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

	fileItemPtr->setText(1, iqt::GetQString(fileItem.GetInputFile()));
	fileItemPtr->setText(2, iqt::GetQString(fileItem.GetOutputFile()));
	// FileList->setItemWidget(fileItemPtr, 3, new QProgressBar());

	imod::IModel* itemModelPtr = const_cast<imod::IModel*>(dynamic_cast<const imod::IModel*>(&fileItem));
	if (itemModelPtr != NULL){
		itemModelPtr->AttachObserver(fileItemPtr);
	}

	FileList->addTopLevelItem(fileItemPtr);
}


void CHotfolderGuiComp::UpdateProcessingCommands()
{
	ifpf::IHotfolder* objectPtr = GetObjectPtr();
	if (objectPtr != NULL){
		bool isWorking = objectPtr->IsWorking();

		iqt::CSignalBlocker block´(&m_runCommand);
		iqt::CSignalBlocker block2(&m_holdCommand);
		m_runCommand.setEnabled(!isWorking);
		m_holdCommand.setEnabled(isWorking);
	}
}


// private slots

void CHotfolderGuiComp::OnRun()
{
	ifpf::IHotfolder* objectPtr = GetObjectPtr();
	if (objectPtr != NULL){
		objectPtr->SetWorking(true);
	}
}


void CHotfolderGuiComp::OnHold()
{
	ifpf::IHotfolder* objectPtr = GetObjectPtr();
	if (objectPtr != NULL){
		objectPtr->SetWorking(false);
	}
}



// public methods of the embedded class 

CHotfolderGuiComp::ProcessingItem::ProcessingItem(iqtgui::IIconProvider* iconsProviderPtr)
	:m_iconsProviderPtr(iconsProviderPtr)
{
}

// protected methods of the embedded class 
		
// reimplemented (imod::TSingleModelObserverBase)

void CHotfolderGuiComp::ProcessingItem::OnUpdate(int updateFlags, istd::IPolymorphic* updateParamsPtr)
{
	ifpf::IHotfolderProcessingItem* objectPtr = GetObjectPtr();
	if (objectPtr != NULL && m_iconsProviderPtr != NULL){
		int fileState = objectPtr->GetProcessingState();
		if (m_iconsProviderPtr->GetIconCount() > fileState){
			setIcon(0, m_iconsProviderPtr->GetIcon(fileState));
		}
	}
}


} // namespace iqtfpf


