#include "iqtfpf/CHotfolderGuiComp.h"


// Qt includes
#include <QHeaderView>
#include <QProgressBar>
#include <QLabel>


// ACF includes
#include "istd/TChangeNotifier.h"

#include "iproc/IProcessor.h"

#include "iqt/CSignalBlocker.h"


#include "iqtfpf/CDirectoryItemGuiComp.h"


namespace iqtfpf
{


// public methods

CHotfolderGuiComp::CHotfolderGuiComp()
{
}


// reimplemented (ibase::ICommandsProvider)

const ibase::IHierarchicalCommand* CHotfolderGuiComp::GetCommands() const
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

		if ((updateFlags & ifpf::IHotfolder::CF_CREATE) != 0 || (updateFlags & ifpf::IHotfolder::CF_FILE_REMOVED) != 0){
			RebuildItemList();

			UpdateItemCommands();
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


// reimplemented (TGuiObserverWrap)

void CHotfolderGuiComp::OnGuiModelAttached()
{
	BaseClass::OnGuiModelAttached();

	UpdateProcessingCommands();

	UpdateEditor(ifpf::IHotfolder::CF_CREATE);

	if (m_statisticsHotfolderObserverCompPtr.IsValid()){
		imod::IModel* hotfolderModelPtr = GetModelPtr();
		I_ASSERT(hotfolderModelPtr != NULL);

		hotfolderModelPtr->AttachObserver(m_statisticsHotfolderObserverCompPtr.GetPtr());
	}
}


void CHotfolderGuiComp::OnGuiModelDetached()
{
	if (m_statisticsHotfolderObserverCompPtr.IsValid()){
		imod::IModel* hotfolderModelPtr = GetModelPtr();
		I_ASSERT(hotfolderModelPtr != NULL);

		if (hotfolderModelPtr->IsAttached(m_statisticsHotfolderObserverCompPtr.GetPtr())){
			hotfolderModelPtr->DetachObserver(m_statisticsHotfolderObserverCompPtr.GetPtr());
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

	m_runCommand.SetGroupId(1);
	m_runCommand.SetStaticFlags(iqtgui::CHierarchicalCommand::CF_GLOBAL_MENU | iqtgui::CHierarchicalCommand::CF_TOOLBAR);
	m_runCommand.SetVisuals(tr("&Run"), "Run", tr("Start/Continue the execution of the hotfolder"), QIcon(":/Icons/Play.svg"));
	connect(&m_runCommand, SIGNAL(activated()), this, SLOT(OnRun()));
	hotfolderMenuPtr->InsertChild(&m_runCommand, false);

	m_holdCommand.SetGroupId(1);
	m_holdCommand.SetStaticFlags(iqtgui::CHierarchicalCommand::CF_GLOBAL_MENU | iqtgui::CHierarchicalCommand::CF_TOOLBAR);
	m_holdCommand.SetVisuals(tr("&Hold"), "Hold", tr("Hold the execution of the hotfolder"), QIcon(":/Icons/Pause.svg"));
	m_holdCommand.setDisabled(true);
	connect(&m_holdCommand, SIGNAL(activated()), this, SLOT(OnHold()));
	hotfolderMenuPtr->InsertChild(&m_holdCommand, false);

	iqtgui::CHierarchicalCommand* resetCommandPtr = new iqtgui::CHierarchicalCommand;
	resetCommandPtr->SetGroupId(1);
	resetCommandPtr->SetStaticFlags(iqtgui::CHierarchicalCommand::CF_GLOBAL_MENU | iqtgui::CHierarchicalCommand::CF_TOOLBAR);
	resetCommandPtr->SetVisuals(tr("&Reset"), "Reset", tr("Reset the hotfolder"), QIcon(":/Icons/Reset"));
	connect(resetCommandPtr, SIGNAL(activated()), this, SLOT(OnReset()));
	hotfolderMenuPtr->InsertChild(resetCommandPtr, true);

	m_removeItemCommand.SetGroupId(2);
	m_removeItemCommand.SetStaticFlags(iqtgui::CHierarchicalCommand::CF_GLOBAL_MENU | iqtgui::CHierarchicalCommand::CF_TOOLBAR);
	m_removeItemCommand.SetVisuals(tr("&Remove Job"), "Remove Job", tr("Remove processing item"), QIcon(":/Icons/Delete.svg"));
	m_removeItemCommand.setDisabled(true);
	connect(&m_removeItemCommand, SIGNAL(activated()), this, SLOT(OnItemRemove()));
	hotfolderMenuPtr->InsertChild(&m_removeItemCommand, false);

	m_cancelItemCommand.SetGroupId(2);
	m_cancelItemCommand.SetStaticFlags(iqtgui::CHierarchicalCommand::CF_GLOBAL_MENU | iqtgui::CHierarchicalCommand::CF_TOOLBAR);
	m_cancelItemCommand.SetVisuals(tr("&Cancel Job"), "Cancel Job", tr("Cancel processing of the selected item"), QIcon(":/Icons/Cancel.svg"));
	m_cancelItemCommand.setDisabled(true);
	connect(&m_cancelItemCommand, SIGNAL(activated()), this, SLOT(OnItemCancel()));
	hotfolderMenuPtr->InsertChild(&m_cancelItemCommand, false);

	m_hotfolderCommands.InsertChild(hotfolderMenuPtr, true);

	// some visual details:
	FileList->header()->setResizeMode(QHeaderView::ResizeToContents);
	FileList->header()->setStretchLastSection(true);

	// initialize state icons map:
	if (m_stateIconsProviderCompPtr.IsValid()){
		for (int fileState = 0; fileState < m_stateIconsProviderCompPtr->GetIconCount(); fileState++){
			QPixmap pixmap = m_stateIconsProviderCompPtr->GetIcon(fileState).pixmap(QSize(12, 12),QIcon::Normal, QIcon::On);

			m_stateIconsMap[fileState] = QIcon(pixmap);
		}
	}
}


void CHotfolderGuiComp::OnGuiDestroyed()
{
	if (m_progressManagerGuiCompPtr.IsValid()){
		m_progressManagerGuiCompPtr->DestroyGui();
	}

	BaseClass::OnGuiDestroyed();
}


// private methods

void CHotfolderGuiComp::AddFileItem(const ifpf::IHotfolderProcessingItem& fileItem)
{
	QString inputFilePath = iqt::GetQString(fileItem.GetInputFile());
	QFileInfo fileInfo(inputFilePath);
	QDir fileDirectory = fileInfo.dir();

	DirectoryItem* parentItemPtr = NULL;
	int itemsCount = FileList->topLevelItemCount();
	for (int index = 0; index < itemsCount;index++){
		DirectoryItem* itemPtr = dynamic_cast<DirectoryItem*>(FileList->topLevelItem(index));
		I_ASSERT(itemPtr != NULL);

		if (itemPtr->GetDirectory() == fileDirectory){
			parentItemPtr = itemPtr;
		}
	}

	if (parentItemPtr == NULL){
		parentItemPtr = new DirectoryItem(*this, fileDirectory, FileList);
		FileList->addTopLevelItem(parentItemPtr);
		parentItemPtr->setExpanded(true);

		// if special directory item UI controller is set,
		// place it into the item:
		if (m_directoryItemGuiFactCompPtr.IsValid() && m_directoryItemObserverFactCompPtr.IsValid()){
			istd::TDelPtr<icomp::IComponent> directoryItemCompPtr(m_directoryItemGuiFactCompPtr.CreateComponent());
			if (directoryItemCompPtr.IsValid()){
				iqtgui::IGuiObject* directoryItemGuiPtr = m_directoryItemGuiFactCompPtr.ExtractInterface(directoryItemCompPtr.GetPtr());
				imod::IObserver* directoryItemObserverPtr = m_directoryItemObserverFactCompPtr.ExtractInterface(directoryItemCompPtr.GetPtr());
				I_ASSERT(directoryItemGuiPtr != NULL);
				I_ASSERT(directoryItemObserverPtr != NULL);

				// TODO: clean this solution!
				iqtfpf::CDirectoryItemGuiComp* dirItemGuiCompPtr = dynamic_cast<iqtfpf::CDirectoryItemGuiComp*>(directoryItemGuiPtr);
				if (dirItemGuiCompPtr != NULL){
					dirItemGuiCompPtr->SetDirectoryPath(fileDirectory.absolutePath());
				}

				istd::TDelPtr<QWidget> widgetWrapperPtr(new QWidget(FileList));
				QVBoxLayout* layout = new QVBoxLayout(widgetWrapperPtr.GetPtr());
				layout->setMargin(4);
			
				if (directoryItemGuiPtr->CreateGui(widgetWrapperPtr.GetPtr())){
					FileList->setItemWidget(parentItemPtr, 0, widgetWrapperPtr.PopPtr());

					if (m_statisticsModelCompPtr.IsValid()){
						if (!m_statisticsModelCompPtr->IsAttached(directoryItemObserverPtr)){
							m_statisticsModelCompPtr->AttachObserver(directoryItemObserverPtr);
						}
					}
				}
			}

			directoryItemCompPtr.PopPtr();
		}
		else{
			parentItemPtr->setText(0, fileDirectory.absolutePath());
		}
	}
	
	parentItemPtr->AddFileItem(fileItem);
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


void CHotfolderGuiComp::UpdateItemCommands()
{
	ProcessingItems selectedItems = GetSelectedProcessingItems();

	bool enableRemoveAction = false;
	bool enableCancelAction = false;

	for (int itemIndex = 0; itemIndex < int(selectedItems.size()); itemIndex++){
		I_ASSERT(selectedItems[itemIndex] != NULL);

		int itemState = selectedItems[itemIndex]->GetProcessingState();

		enableRemoveAction = enableRemoveAction || (itemState != iproc::IProcessor::TS_WAIT);
		enableCancelAction = enableCancelAction || ((itemState == iproc::IProcessor::TS_NONE) || itemState == iproc::IProcessor::TS_WAIT);
	}
	
	m_removeItemCommand.setEnabled(enableRemoveAction);
	m_cancelItemCommand.setEnabled(enableCancelAction);
}



void CHotfolderGuiComp::RebuildItemList()
{
	while (FileList->topLevelItemCount() > 0){
		QTreeWidgetItem* itemPtr = FileList->takeTopLevelItem(0);

		delete itemPtr;
	}
	
	ifpf::IHotfolder* objectPtr = GetObjectPtr();
	if (objectPtr != NULL){
		int itemsCount = objectPtr->GetProcessingItemsCount();
		for (int itemIndex = 0; itemIndex < itemsCount; itemIndex++){
			ifpf::IHotfolderProcessingItem* pocessingItem = objectPtr->GetProcessingItem(itemIndex);

			AddFileItem(*pocessingItem);
		}
	}
}

CHotfolderGuiComp::ProcessingItems CHotfolderGuiComp::GetSelectedProcessingItems() const
{
	ProcessingItems items;

	QList<QTreeWidgetItem*> selectedItems = FileList->selectedItems();

	for (int itemIndex = 0; itemIndex < selectedItems.count(); itemIndex++){
		ProcessingItem* processingItemPtr = dynamic_cast<ProcessingItem*>(selectedItems.at(itemIndex));
		if (processingItemPtr != NULL){
			items.push_back(processingItemPtr->GetObjectPtr());
		}
	}

	return items;
}


QIcon CHotfolderGuiComp::GetStateIcon(int fileState) const
{
	static QIcon emptyIcon;

	StateIconsMap::const_iterator foundIconIter = m_stateIconsMap.find(fileState);
	if (foundIconIter != m_stateIconsMap.end()){
		return foundIconIter->second;
	}

	return emptyIcon;
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


void CHotfolderGuiComp::OnItemRemove()
{
	ProcessingItems processingItems = GetSelectedProcessingItems();
	ifpf::IHotfolder* objectPtr = GetObjectPtr();
	if (objectPtr != NULL){
		istd::CChangeNotifier changePtr(objectPtr, ifpf::IHotfolder::CF_FILE_REMOVED);
		for (int itemIndex = 0; itemIndex < int(processingItems.size()); itemIndex++){
			objectPtr->RemoveProcessingItem(processingItems[itemIndex]);
		}
	}
}


void CHotfolderGuiComp::OnItemCancel()
{
	ProcessingItems processingItems = GetSelectedProcessingItems();
	for (int itemIndex = 0; itemIndex < int(processingItems.size()); itemIndex++){
		int itemState = processingItems[itemIndex]->GetProcessingState();
		if ((itemState == iproc::IProcessor::TS_NONE) || (itemState == iproc::IProcessor::TS_WAIT)){
			processingItems[itemIndex]->SetProcessingState(iproc::IProcessor::TS_CANCELED);
		}
	}
}


void CHotfolderGuiComp::on_FileList_itemSelectionChanged()
{
	ProcessingItems processingItems = GetSelectedProcessingItems();
	if (!processingItems.empty()){
		imod::IModel* itemModelPtr = dynamic_cast<imod::IModel*>(processingItems[0]);
		if (itemModelPtr != NULL){
			BaseClass2::SetModelPtr(itemModelPtr);
		}
	}
	else{
		BaseClass2::SetModelPtr(NULL);
	}

	UpdateItemCommands();
}


// public methods of the embedded class 

CHotfolderGuiComp::ProcessingItem::ProcessingItem(const CHotfolderGuiComp& parent, QTreeWidget* treeWidgetPtr)
	:BaseClass(treeWidgetPtr),
	m_parent(parent)
{
}


// protected methods of the embedded class 
		 
// reimplemented (imod::TSingleModelObserverBase)

void CHotfolderGuiComp::ProcessingItem::OnUpdate(int /*updateFlags*/, istd::IPolymorphic* /*updateParamsPtr*/)
{
	ifpf::IHotfolderProcessingItem* objectPtr = GetObjectPtr();
	if (objectPtr != NULL){
		int fileState = objectPtr->GetProcessingState();

		QIcon stateIcon = m_parent.GetStateIcon(fileState);

		setIcon(0, stateIcon);
	}
}


CHotfolderGuiComp::DirectoryItem::DirectoryItem(
			const CHotfolderGuiComp& parent,
			const QDir& directory,
			QTreeWidget* treeWidgetPtr)
	:BaseClass(treeWidgetPtr),
	m_parent(parent),
	m_directory(directory)
{
}


const QDir& CHotfolderGuiComp::DirectoryItem::GetDirectory() const
{
	return m_directory;
}


void CHotfolderGuiComp::DirectoryItem::AddFileItem(const ifpf::IHotfolderProcessingItem& fileItem)
{
	QString inputFilePath = iqt::GetQString(fileItem.GetInputFile());
	QFileInfo fileInfo(inputFilePath);

	ProcessingItem* fileItemPtr = new ProcessingItem(m_parent);
	fileItemPtr->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
	fileItemPtr->setBackgroundColor(0, Qt::transparent);
	fileItemPtr->setText(0, fileInfo.fileName());

	BaseClass::addChild(fileItemPtr);

	imod::IModel* itemModelPtr = const_cast<imod::IModel*>(dynamic_cast<const imod::IModel*>(&fileItem));
	if (itemModelPtr != NULL){
		itemModelPtr->AttachObserver(fileItemPtr);
	}
}


} // namespace iqtfpf


