#include "iqtfpf/CHotfolderGuiComp.h"


// Qt includes
#include <QtGui/QHeaderView>
#include <QtGui/QProgressBar>
#include <QtGui/QLabel>
#include <QtGui/QMenu>

// ACF includes
#include "istd/TChangeNotifier.h"
#include "iprm/ISelectionConstraints.h"
#include "iproc/IProcessor.h"
#include "iqt/CSignalBlocker.h"


// ACF-Solutions includes
#include "iqtfpf/CDirectoryItemGuiComp.h"


namespace iqtfpf
{


// public methods

CHotfolderGuiComp::CHotfolderGuiComp()
	:m_filterEditor(NULL),
	m_inputDirectoriesObserver(*this)
{
}


// reimplemented (ibase::ICommandsProvider)

const ibase::IHierarchicalCommand* CHotfolderGuiComp::GetCommands() const
{
	return &m_hotfolderCommands;
}


// reimplemented (iqtgui::TGuiObserverWrap)

void CHotfolderGuiComp::UpdateGui(int updateFlags)
{
	I_ASSERT(IsGuiCreated());

	ifpf::IHotfolderProcessingInfo* objectPtr = GetObjectPtr();
	if (objectPtr != NULL){
		if ((updateFlags & ifpf::IHotfolderProcessingInfo::CF_CREATE) != 0){
			RebuildItemList();

			UpdateItemCommands();
		}

		if ((updateFlags & ifpf::IHotfolderProcessingInfo::CF_FILE_ADDED) != 0){
			UpdateAddedItemList();
		}

		if ((updateFlags & ifpf::IHotfolderProcessingInfo::CF_FILE_REMOVED) != 0){
			UpdateRemovedItemList();
		}

		if ((updateFlags & ifpf::IHotfolderProcessingInfo::CF_WORKING_STATE_CHANGED) != 0){
			UpdateProcessingCommands();
		}

		I_ASSERT(m_filterEditor != NULL);

		UpdateItemsVisibility(m_filterEditor->GetText(), ErrorsRadio->isChecked());
	}
}


void CHotfolderGuiComp::OnGuiModelAttached()
{
	BaseClass::OnGuiModelAttached();

	UpdateProcessingCommands();

	UpdateGui(ifpf::IHotfolderProcessingInfo::CF_CREATE);

	if (m_statisticsHotfolderObserverCompPtr.IsValid()){
		imod::IModel* hotfolderModelPtr = GetModelPtr();
		I_ASSERT(hotfolderModelPtr != NULL);

		hotfolderModelPtr->AttachObserver(m_statisticsHotfolderObserverCompPtr.GetPtr());
	}

	if (m_inputDirectoriesParamsManagerModelCompPtr.IsValid()){
		m_inputDirectoriesParamsManagerModelCompPtr->AttachObserver(&m_inputDirectoriesObserver);
	}

	if (m_processingItemPreviewCompPtr.IsValid()){
		m_itemModelProxy.AttachObserver(m_processingItemPreviewCompPtr.GetPtr());
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

	if (m_inputDirectoriesParamsManagerModelCompPtr.IsValid()){
		m_inputDirectoriesParamsManagerModelCompPtr->DetachObserver(&m_inputDirectoriesObserver);
	}

	if (m_processingItemPreviewCompPtr.IsValid()){
		m_itemModelProxy.DetachObserver(m_processingItemPreviewCompPtr.GetPtr());
	}

	BaseClass::OnGuiModelDetached();
}


// reimplemented (iqtgui::CGuiComponentBase)

void CHotfolderGuiComp::OnGuiCreated()
{
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

	m_restartItemCommand.SetGroupId(2);
	m_restartItemCommand.SetStaticFlags(iqtgui::CHierarchicalCommand::CF_GLOBAL_MENU | iqtgui::CHierarchicalCommand::CF_TOOLBAR);
	m_restartItemCommand.SetVisuals(tr("&Restart"), "Restart Job", tr("Restart selected job(s)"), QIcon(":/Icons/Reload.svg"));
	m_restartItemCommand.setShortcut(Qt::CTRL + Qt::Key_R);
	connect(&m_restartItemCommand, SIGNAL(activated()), this, SLOT(OnRestart()));
	hotfolderMenuPtr->InsertChild(&m_restartItemCommand, false);

	m_removeItemCommand.SetGroupId(2);
	m_removeItemCommand.SetStaticFlags(iqtgui::CHierarchicalCommand::CF_GLOBAL_MENU | iqtgui::CHierarchicalCommand::CF_TOOLBAR);
	m_removeItemCommand.SetVisuals(tr("&Remove Job"), "Remove Job", tr("Remove processing item"), QIcon(":/Icons/Delete.svg"));
	m_removeItemCommand.setDisabled(true);
	m_removeItemCommand.setShortcut(Qt::Key_Delete);
	connect(&m_removeItemCommand, SIGNAL(activated()), this, SLOT(OnItemRemove()));
	hotfolderMenuPtr->InsertChild(&m_removeItemCommand, false);

	m_cancelItemCommand.SetGroupId(2);
	m_cancelItemCommand.SetStaticFlags(iqtgui::CHierarchicalCommand::CF_GLOBAL_MENU | iqtgui::CHierarchicalCommand::CF_TOOLBAR);
	m_cancelItemCommand.SetVisuals(tr("&Cancel Job"), "Cancel Job", tr("Cancel processing of the selected item"), QIcon(":/Icons/Cancel.svg"));
	m_cancelItemCommand.setDisabled(true);
	connect(&m_cancelItemCommand, SIGNAL(activated()), this, SLOT(OnItemCancel()));
	hotfolderMenuPtr->InsertChild(&m_cancelItemCommand, false);

	if (m_processingParamsDialogCompPtr.IsValid()){
		m_showProcessingDialogCommand.SetGroupId(3);
		m_showProcessingDialogCommand.SetStaticFlags(iqtgui::CHierarchicalCommand::CF_GLOBAL_MENU | iqtgui::CHierarchicalCommand::CF_TOOLBAR);
		m_showProcessingDialogCommand.SetVisuals(tr("&Processing Parameters..."), "Processing Parameters", tr("Define parameters for the hotfolder processing"), QIcon(":/Icons/HotfolderSettings.svg"));
		connect(&m_showProcessingDialogCommand, SIGNAL(activated()), this, SLOT(OnShowProcessingParamsDialog()));
		hotfolderMenuPtr->InsertChild(&m_showProcessingDialogCommand, false);
	}

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

	FileList->setContextMenuPolicy( Qt::CustomContextMenu);

	connect(FileList, 
				SIGNAL(customContextMenuRequested(const QPoint&)),
				this, 
				SLOT(OnContextMenuRequested(const QPoint&)));

	// Create item filter:
	m_filterEditor = new iqtgui::CExtLineEdit("<Filter>", 2, TextFilterFrame);
	QLayout* layoutPtr = TextFilterFrame->layout();
	if (layoutPtr != NULL){
		layoutPtr->addWidget(m_filterEditor);
	}

	connect(m_filterEditor, SIGNAL(textChanged(const QString&)), this, SLOT(OnTextFilterChanged(const QString&)));

	// Create preview GUI:
	if (m_processingItemPreviewGuiCompPtr.IsValid()){
		I_ASSERT(!m_processingItemPreviewGuiCompPtr->IsGuiCreated());

		m_processingItemPreviewGuiCompPtr->CreateGui(PreviewFrame);
	}

	BaseClass::OnGuiCreated();
}


void CHotfolderGuiComp::OnGuiDestroyed()
{
	if (m_processingItemPreviewGuiCompPtr.IsValid()){
		m_processingItemPreviewGuiCompPtr->DestroyGui();
	}

	BaseClass::OnGuiDestroyed();
}


// private methods

void CHotfolderGuiComp::AddFileItem(const ifpf::IHotfolderProcessingItem& fileItem)
{
	QString inputFilePath = fileItem.GetInputFile();
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

	if (parentItemPtr != NULL){
		parentItemPtr->AddFileItem(fileItem);
	}
}


void CHotfolderGuiComp::UpdateProcessingCommands()
{
	ifpf::IHotfolderProcessingInfo* objectPtr = GetObjectPtr();
	if (objectPtr != NULL){
		bool isWorking = objectPtr->IsWorking();

		iqt::CSignalBlocker block(&m_runCommand);
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
	bool enableRestartAction = false;

	for (int itemIndex = 0; itemIndex < int(selectedItems.size()); itemIndex++){
		I_ASSERT(selectedItems[itemIndex] != NULL);

		int itemState = selectedItems[itemIndex]->GetObjectPtr()->GetProcessingState();

		enableRemoveAction = enableRemoveAction || (itemState != iproc::IProcessor::TS_WAIT);
		enableCancelAction = enableCancelAction || ((itemState == iproc::IProcessor::TS_NONE) || itemState == iproc::IProcessor::TS_WAIT);
		enableRestartAction = enableRestartAction || (itemState != iproc::IProcessor::TS_WAIT);
	}
	
	m_removeItemCommand.setEnabled(enableRemoveAction);
	m_cancelItemCommand.setEnabled(enableCancelAction);
	m_restartItemCommand.setEnabled(enableRestartAction);
}


void CHotfolderGuiComp::RebuildItemList()
{
	FileList->clearSelection();

	while (FileList->topLevelItemCount() > 0){
		QTreeWidgetItem* itemPtr = FileList->takeTopLevelItem(0);

		delete itemPtr;
	}

	if (m_inputDirectoriesParamsManagerCompPtr.IsValid()){
		int setsCount = m_inputDirectoriesParamsManagerCompPtr->GetParamsSetsCount(); 
		for (int setIndex = 0; setIndex < setsCount; setIndex++){
			QString setName = m_inputDirectoriesParamsManagerCompPtr->GetParamsSetName(setIndex);

			iprm::IParamsSet* paramSetPtr = m_inputDirectoriesParamsManagerCompPtr->GetParamsSet(setIndex);

			const iprm::IFileNameParam* fileNameParamPtr = dynamic_cast<const iprm::IFileNameParam*>(paramSetPtr->GetParameter(*m_directoryPathIdAttrPtr));
			if (fileNameParamPtr != NULL){
				QString directoryPath = fileNameParamPtr->GetPath();
				DirectoryItem* directoryItemPtr = new DirectoryItem(*this, setIndex, QDir(directoryPath), FileList);

				FileList->addTopLevelItem(directoryItemPtr);
			}
		}
	}

	ifpf::IHotfolderProcessingInfo* objectPtr = GetObjectPtr();
	if (objectPtr != NULL){
		int itemsCount = objectPtr->GetProcessingItemsCount();
		for (int itemIndex = 0; itemIndex < itemsCount; itemIndex++){
			ifpf::IHotfolderProcessingItem* processingItemPtr = objectPtr->GetProcessingItem(itemIndex);

			AddFileItem(*processingItemPtr);
		}
	}
}


void CHotfolderGuiComp::UpdateAddedItemList()
{
	ifpf::IHotfolderProcessingInfo* objectPtr = GetObjectPtr();
	if (objectPtr != NULL){
		int itemsCount = objectPtr->GetProcessingItemsCount();

		int currentItemsCount = 0;
		
		for (int itemIndex = 0; itemIndex < FileList->topLevelItemCount(); itemIndex++){
			currentItemsCount += FileList->topLevelItem(itemIndex)->childCount();
		}

		I_ASSERT(currentItemsCount < itemsCount);

		for (int itemIndex = currentItemsCount; itemIndex < itemsCount; itemIndex++){
			ifpf::IHotfolderProcessingItem* processingItemPtr = objectPtr->GetProcessingItem(itemIndex);

			AddFileItem(*processingItemPtr);
		}
	}
}


void CHotfolderGuiComp::UpdateRemovedItemList()
{
	QTreeWidgetItemIterator itemsIter(FileList);

	bool workDone = false;
	while (!workDone){
		workDone = true;
		while ((*itemsIter) != NULL){
			ProcessingItem* processingItemPtr = dynamic_cast<ProcessingItem*>(*itemsIter);

			if (processingItemPtr != NULL && processingItemPtr->GetObjectPtr() == NULL){
				QTreeWidgetItem* parentItemPtr = processingItemPtr->parent();

				if (processingItemPtr->isSelected()){
					FileList->clearSelection();
				}

				parentItemPtr->removeChild(processingItemPtr);

				workDone = false;
				break;
			}
			
			itemsIter++;
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
			items.push_back(processingItemPtr);
		}
	}

	return items;
}


QIcon CHotfolderGuiComp::GetStateIcon(int fileState) const
{
	static QIcon emptyIcon;

	StateIconsMap::const_iterator foundIconIter = m_stateIconsMap.find(fileState);
	if (foundIconIter != m_stateIconsMap.end()){
		return foundIconIter.value();
	}

	return emptyIcon;
}


void CHotfolderGuiComp::UpdateItemsVisibility(const QString& textFilter, bool showOnlyErrors)
{
	QTreeWidgetItemIterator treeIterator(FileList);
     while (*treeIterator){
		 ProcessingItem* itemPtr = dynamic_cast<ProcessingItem*>(*treeIterator);
		 if (itemPtr != NULL){
			 ifpf::IHotfolderProcessingItem* processingItemPtr = itemPtr->GetObjectPtr();
			 if (processingItemPtr != NULL){
				 QString fileName = processingItemPtr->GetInputFile();
				 QRegExp regExp(textFilter, Qt::CaseInsensitive, QRegExp::RegExp);
				 bool isError = (processingItemPtr->GetProcessingState() == iproc::IProcessor::TS_INVALID);
				 bool isMatched = fileName.contains(regExp);
				 bool isVisible = ((showOnlyErrors && isError) && isMatched) || (!showOnlyErrors && isMatched);
				 
				 itemPtr->setHidden(!isVisible);
			 }
		 }

		 ++treeIterator;
     }
}


// private slots

void CHotfolderGuiComp::OnRun()
{
	ifpf::IHotfolderProcessingInfo* objectPtr = GetObjectPtr();
	if (objectPtr != NULL){
		objectPtr->SetWorking(true);
	}
}


void CHotfolderGuiComp::OnHold()
{
	ifpf::IHotfolderProcessingInfo* objectPtr = GetObjectPtr();
	if (objectPtr != NULL){
		objectPtr->SetWorking(false);
	}
}


void CHotfolderGuiComp::OnItemRemove()
{
	UpdateBlocker updateBlocker(this);

	ProcessingItems processingItems = GetSelectedProcessingItems();
	istd::TChangeNotifier<ifpf::IHotfolderProcessingInfo> changePtr(GetObjectPtr());
	if (changePtr.IsValid()){
		FileList->clearSelection();
	
		for (int itemIndex = 0; itemIndex < int(processingItems.size()); itemIndex++){
			ProcessingItem* processingItemPtr = processingItems[itemIndex];

			ifpf::IHotfolderProcessingItem* itemPtr = processingItemPtr->GetObjectPtr();
			I_ASSERT(itemPtr != NULL);
			if (itemPtr != NULL && itemPtr->GetProcessingState() == iproc::IProcessor::TS_WAIT){		
				itemPtr->SetProcessingState(iproc::IProcessor::TS_CANCELED);
			}

			QTreeWidgetItem* parentItem = processingItemPtr->parent();
			I_ASSERT(parentItem != NULL);

			parentItem->removeChild(processingItemPtr);

			if (parentItem->childCount() == 0){
				delete FileList->takeTopLevelItem(FileList->indexOfTopLevelItem(parentItem));
			}

			changePtr->RemoveProcessingItem(itemPtr);
		}
	}
}


void CHotfolderGuiComp::OnItemCancel()
{
	ProcessingItems processingItems = GetSelectedProcessingItems();
	ifpf::IHotfolderProcessingInfo* objectPtr = GetObjectPtr();
	if (objectPtr != NULL){
		for (int itemIndex = 0; itemIndex < int(processingItems.size()); itemIndex++){
			ifpf::IHotfolderProcessingItem* processingItemPtr = processingItems[itemIndex]->GetObjectPtr();
			I_ASSERT(processingItemPtr != NULL);

			int itemState = processingItemPtr->GetProcessingState();
			if ((itemState == iproc::IProcessor::TS_NONE) || (itemState == iproc::IProcessor::TS_WAIT)){
				processingItemPtr->SetProcessingState(iproc::IProcessor::TS_CANCELED);
			}
		}
	}

	UpdateItemCommands();
}


void CHotfolderGuiComp::OnRestart()
{
	ProcessingItems processingItems = GetSelectedProcessingItems();
	
	ifpf::IHotfolderProcessingInfo* objectPtr = GetObjectPtr();
	if (objectPtr != NULL){
		istd::CChangeNotifier changePtr(objectPtr);
		bool fireChangeEvent = false;

		for (int itemIndex = 0; itemIndex < int(processingItems.size()); itemIndex++){
			int itemState = processingItems[itemIndex]->GetObjectPtr()->GetProcessingState();
			if (itemState != iproc::IProcessor::TS_WAIT){
				processingItems[itemIndex]->GetObjectPtr()->SetProcessingState(iproc::IProcessor::TS_NONE);
				
				fireChangeEvent = true;
			}
		}

		if (!fireChangeEvent){
			changePtr.Abort();
		}
	}

	UpdateItemCommands();
}


void CHotfolderGuiComp::OnShowProcessingParamsDialog()
{
	if (m_processingParamsDialogCompPtr.IsValid()){
		m_processingParamsDialogCompPtr->ExecuteDialog(this);
	}
}


void CHotfolderGuiComp::OnContextMenuRequested(const QPoint& menuPoint)
{
	I_ASSERT(IsGuiCreated());

	QPoint localPoint;
	localPoint = FileList->viewport()->mapToGlobal(menuPoint);

	QList<QAction*> actionList;
	actionList << &m_removeItemCommand << &m_cancelItemCommand << &m_restartItemCommand;

	QMenu::exec(actionList, localPoint);
}


void CHotfolderGuiComp::on_FileList_itemSelectionChanged()
{
	ProcessingItems processingItems = GetSelectedProcessingItems();
	if (!processingItems.isEmpty()){
		imod::IModel* itemModelPtr = processingItems[0]->GetModelPtr();
		if (itemModelPtr != NULL){
			m_itemModelProxy.SetModelPtr(itemModelPtr);
		}
	}
	else{
		m_itemModelProxy.SetModelPtr(NULL);
	}

	UpdateItemCommands();
}


void CHotfolderGuiComp::on_AllRadio_toggled(bool isChecked)
{
	if (isChecked){
		I_ASSERT(m_filterEditor != NULL);

		UpdateItemsVisibility(m_filterEditor->GetText(), false);
	}
}


void CHotfolderGuiComp::on_ErrorsRadio_toggled(bool isChecked)
{
	if (isChecked){
		I_ASSERT(m_filterEditor != NULL);

		UpdateItemsVisibility(m_filterEditor->GetText(), true);
	}
}


void CHotfolderGuiComp::OnTextFilterChanged(const QString& filterText)
{
	UpdateItemsVisibility(filterText, ErrorsRadio->isChecked());
}


// public methods of the embedded class 

CHotfolderGuiComp::ProcessingItem::ProcessingItem(CHotfolderGuiComp& parent, QTreeWidget* treeWidgetPtr)
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

		m_parent.UpdateItemCommands();
	}
}


CHotfolderGuiComp::DirectoryItem::DirectoryItem(
			CHotfolderGuiComp& parent,
			int setIndex,
			const QDir& directory,
			QTreeWidget* treeWidgetPtr)
	:BaseClass(treeWidgetPtr),
	m_parent(parent),
	m_directory(directory)
{
	setFlags(Qt::ItemIsEnabled);

	// if special directory item UI controller is set, place it into the item:
	if (m_parent.m_directoryItemGuiFactCompPtr.IsValid()){
		m_directoryItemCompPtr.SetPtr(m_parent.m_directoryItemGuiFactCompPtr.CreateComponent());
		if (m_directoryItemCompPtr.IsValid()){
			if (m_parent.m_directoryItemSelectionFactCompPtr.IsValid()){
				iprm::ISelectionParam* directoryItemSelectionPtr = m_parent.m_directoryItemSelectionFactCompPtr.ExtractInterface(m_directoryItemCompPtr.GetPtr());
				if (directoryItemSelectionPtr != NULL){
					directoryItemSelectionPtr->SetSelectedOptionIndex(setIndex);
				}
			}

			istd::TDelPtr<QWidget> widgetWrapperPtr(new QWidget(m_parent.FileList));
			QVBoxLayout* layout = new QVBoxLayout(widgetWrapperPtr.GetPtr());
			layout->setContentsMargins(4, 4, 4, 4);

			if (m_directoryItemCompPtr.IsValid() && m_parent.m_directoryItemGuiFactCompPtr.IsValid() && m_parent.m_directoryItemObserverFactCompPtr.IsValid()){
				iqtgui::IGuiObject* directoryItemGuiPtr = m_parent.m_directoryItemGuiFactCompPtr.ExtractInterface(m_directoryItemCompPtr.GetPtr());
				if (directoryItemGuiPtr->CreateGui(widgetWrapperPtr.GetPtr())){
					m_parent.FileList->setItemWidget(this, 0, widgetWrapperPtr.PopPtr());

					imod::IObserver* directoryItemObserverPtr = m_parent.m_directoryItemObserverFactCompPtr.ExtractInterface(m_directoryItemCompPtr.GetPtr());
					if (directoryItemObserverPtr != NULL && m_parent.m_statisticsModelCompPtr.IsValid()){
						if (!m_parent.m_statisticsModelCompPtr->IsAttached(directoryItemObserverPtr)){
							m_parent.m_statisticsModelCompPtr->AttachObserver(directoryItemObserverPtr);
						}
					}
				}
			}
		}
	}
}


const QDir& CHotfolderGuiComp::DirectoryItem::GetDirectory() const
{
	return m_directory;
}


void CHotfolderGuiComp::DirectoryItem::AddFileItem(const ifpf::IHotfolderProcessingItem& fileItem)
{
	QString inputFilePath = fileItem.GetInputFile();
	QFileInfo fileInfo(inputFilePath);

	ProcessingItem* fileItemPtr = new ProcessingItem(m_parent);
	fileItemPtr->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
	fileItemPtr->setText(0, fileInfo.fileName());

	BaseClass::addChild(fileItemPtr);

	imod::IModel* itemModelPtr = const_cast<imod::IModel*>(dynamic_cast<const imod::IModel*>(&fileItem));
	if (itemModelPtr != NULL){
		itemModelPtr->AttachObserver(fileItemPtr);
	}
}


CHotfolderGuiComp::InputDirectoriesObserver::InputDirectoriesObserver(CHotfolderGuiComp& parent)
	:m_parent(parent)
{
}


// protected methods

// reimplemented (imod::TSingleModelObserverBase)

void CHotfolderGuiComp::InputDirectoriesObserver::OnUpdate(int /*updateFlags*/, istd::IPolymorphic* /*updateParamsPtr*/)
{
	iprm::IParamsManager* objectPtr = GetObjectPtr();
	if (objectPtr != NULL){
		m_parent.RebuildItemList();
	}
}


} // namespace iqtfpf


