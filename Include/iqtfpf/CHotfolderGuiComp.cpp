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

		if ((updateFlags & ifpf::IHotfolder::CF_FILE_ADDED) != 0){
			int itemsCount = objectPtr->GetProcessingItemsCount();
			if (itemsCount > 0){
				ifpf::IHotfolderProcessingItem* pocessingItem = objectPtr->GetProcessingItem(itemsCount - 1);

				AddFileItem(*pocessingItem);
			}
		}
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

	// some visual details:
	FileList->header()->setResizeMode(QHeaderView::ResizeToContents);
	FileList->header()->setStretchLastSection(true);

	iqtgui::CItemDelegate* itemDelegate = new iqtgui::CItemDelegate(25, this);
	FileList->setItemDelegate(itemDelegate);

	FileList->header()->setResizeMode(0, QHeaderView::Fixed);
	FileList->header()->resizeSection(0, itemDelegate->GetItemHeight());

}


void CHotfolderGuiComp::OnGuiDestroyed()
{
	m_settingsDialogPtr.Reset();

	BaseClass::OnGuiDestroyed();
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


// private slots

void CHotfolderGuiComp::OnSettings()
{
	if (m_settingsDialogPtr.IsValid()){
		m_settingsDialogPtr->exec();
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


