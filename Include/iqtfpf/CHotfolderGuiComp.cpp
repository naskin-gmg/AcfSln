#include "iqtfpf/CHotfolderGuiComp.h"


// Qt includes
#include <QHeaderView>
#include <QProgressBar>
#include <QLabel>


// ACF includes
#include "istd/TChangeNotifier.h"

#include "iqt/CSignalBlocker.h"


namespace iqtfpf
{


// public methods

CHotfolderGuiComp::CHotfolderGuiComp()
{
}


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
			RebuildItemList();
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
	m_runCommand.SetVisuals(tr("&Run"), "Run", tr("Start/Continue the execution of the hotfolder"), QIcon(":/Icons/Play"));
	connect(&m_runCommand, SIGNAL(activated()), this, SLOT(OnRun()));
	hotfolderMenuPtr->InsertChild(&m_runCommand, false);

	m_holdCommand.SetGroupId(1);
	m_holdCommand.SetStaticFlags(iqtgui::CHierarchicalCommand::CF_GLOBAL_MENU | iqtgui::CHierarchicalCommand::CF_TOOLBAR);
	m_holdCommand.SetVisuals(tr("&Hold"), "Hold", tr("Hold the execution of the hotfolder"), QIcon(":/Icons/Pause"));
	m_holdCommand.setDisabled(true);
	connect(&m_holdCommand, SIGNAL(activated()), this, SLOT(OnHold()));
	hotfolderMenuPtr->InsertChild(&m_holdCommand, false);

	iqtgui::CHierarchicalCommand* resetCommandPtr = new iqtgui::CHierarchicalCommand;
	resetCommandPtr->SetGroupId(1);
	resetCommandPtr->SetStaticFlags(iqtgui::CHierarchicalCommand::CF_GLOBAL_MENU | iqtgui::CHierarchicalCommand::CF_TOOLBAR);
	resetCommandPtr->SetVisuals(tr("&Reset"), "Reset", tr("Reset the hotfolder"), QIcon(":/Icons/Reset"));
	connect(resetCommandPtr, SIGNAL(activated()), this, SLOT(OnReset()));
	hotfolderMenuPtr->InsertChild(resetCommandPtr, true);

	m_hotfolderCommands.InsertChild(hotfolderMenuPtr, true);

	// some visual details:
	FileList->header()->setResizeMode(QHeaderView::ResizeToContents);
	FileList->header()->setStretchLastSection(true);
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
		parentItemPtr = new DirectoryItem(fileDirectory, FileList);

		parentItemPtr->setExpanded(true);
	}
	
	parentItemPtr->AddFileItem(fileItem, m_stateIconsProviderCompPtr.GetPtr());

	if (m_statisticsModelCompPtr.IsValid()){
		if (!m_statisticsModelCompPtr->IsAttached(parentItemPtr)){
			m_statisticsModelCompPtr->AttachObserver(parentItemPtr);
		}
	}
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


void CHotfolderGuiComp::on_FileList_itemSelectionChanged()
{
	QList<QTreeWidgetItem*> selectedItems = FileList->selectedItems();
	if (selectedItems.isEmpty()){
		return;
	}

	imod::IModel* currentItemModelPtr = NULL;
	ProcessingItem* fileItemPtr = dynamic_cast<ProcessingItem*>(selectedItems.at(0));
	if (fileItemPtr != NULL){
		currentItemModelPtr = fileItemPtr->GetModelPtr();
	}
}



// public methods of the embedded class 

CHotfolderGuiComp::ProcessingItem::ProcessingItem(const iqtgui::IIconProvider* iconsProviderPtr, QTreeWidget* parent)
	:BaseClass(parent),
	m_iconsProviderPtr(iconsProviderPtr)
{
}


// protected methods of the embedded class 
		 
// reimplemented (imod::TSingleModelObserverBase)

void CHotfolderGuiComp::ProcessingItem::OnUpdate(int /*updateFlags*/, istd::IPolymorphic* /*updateParamsPtr*/)
{
	ifpf::IHotfolderProcessingItem* objectPtr = GetObjectPtr();
	if (objectPtr != NULL && m_iconsProviderPtr != NULL){
		int fileState = objectPtr->GetProcessingState();
		if (m_iconsProviderPtr->GetIconCount() > fileState){
			QPixmap pixmap = m_iconsProviderPtr->GetIcon(fileState).pixmap(QSize(12, 12),QIcon::Normal, QIcon::On);

			setIcon(0, pixmap);
		}
	}
}


CHotfolderGuiComp::DirectoryItem::DirectoryItem(const QDir& directory, QTreeWidget* parentPtr)
	:BaseClass(parentPtr),
	m_directory(directory),
	m_statisticsLabel(NULL)
{
	QFont font;
	font.setBold(true);

	QWidget* itemWidget = new QWidget();
	QHBoxLayout* mainLayout = new QHBoxLayout(itemWidget);
	itemWidget->setObjectName("Frame");
	//	itemWidget->setStyleSheet("QWidget#Frame{background: qlineargradient(x1: 0, y1: 0, x2: 0.3, y2: 0, stop: 0 #abf1ab, stop: 1 #fdfdfd);}");
	//	itemWidget->setStyleSheet("QWidget#Frame{background-color: rgba(223,223,223,255);}");
	QVBoxLayout* layout = new QVBoxLayout();
	QLabel* label = new QLabel(itemWidget);
	label->setMargin(2);
	label->setText(directory.absolutePath());
	label->setFont(font);
	layout->addWidget(label);
	layout->setSpacing(0);
	layout->setMargin(0);

	mainLayout->setSpacing(4);
	mainLayout->setMargin(0);

	font.setBold(false);
	font.setPointSize(7); 
	m_statisticsLabel = new QLabel(itemWidget);
	m_statisticsLabel->setMargin(2);
	layout->addWidget(m_statisticsLabel);

	QLabel* iconLabel = new QLabel(itemWidget);
	iconLabel->setPixmap(QIcon("C:\\Work\\Develop\\AcfSln\\Docs\\Images\\Hotfolder.svg").pixmap(QSize(32,32), QIcon::Normal, QIcon::On));
	mainLayout->addWidget(iconLabel);
	mainLayout->addLayout(layout);
	mainLayout->addSpacerItem(new QSpacerItem(20,10,QSizePolicy::Expanding));

	parentPtr->addTopLevelItem(this);
	parentPtr->setItemWidget(this, 0, itemWidget);
}


const QDir& CHotfolderGuiComp::DirectoryItem::GetDirectory() const
{
	return m_directory;
}


void CHotfolderGuiComp::DirectoryItem::AddFileItem(const ifpf::IHotfolderProcessingItem& fileItem, const iqtgui::IIconProvider* iconsProviderPtr)
{
	QString inputFilePath = iqt::GetQString(fileItem.GetInputFile());
	QFileInfo fileInfo(inputFilePath);

	ProcessingItem* fileItemPtr = new ProcessingItem(iconsProviderPtr);
	fileItemPtr->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
	fileItemPtr->setBackgroundColor(0, Qt::transparent);
	fileItemPtr->setText(0, fileInfo.fileName());

	BaseClass::addChild(fileItemPtr);

	imod::IModel* itemModelPtr = const_cast<imod::IModel*>(dynamic_cast<const imod::IModel*>(&fileItem));
	if (itemModelPtr != NULL){
		itemModelPtr->AttachObserver(fileItemPtr);
	}
}


// protected methods

void CHotfolderGuiComp::DirectoryItem::UpdateStatistics(const ifpf::IHotfolderStatistics& statistics)
{
	I_ASSERT(m_statisticsLabel != NULL);

	double itemsCount = statistics.GetAbortedCount() + statistics.GetWaitingCount() + statistics.GetProcessedCount();

	m_statisticsLabel->setText(
				QString("%1 (%2%) items waiting, %3 (%4%) items are ready")
						.arg(statistics.GetWaitingCount())
						.arg(itemsCount == 0 ? 0 : int(100 * statistics.GetWaitingCount() / itemsCount))
						.arg(statistics.GetProcessedCount())
						.arg(itemsCount == 0 ? 0 : int(100 * statistics.GetProcessedCount() / itemsCount)));
}


// reimplemented (imod::CMultiModelObserverBase)

void CHotfolderGuiComp::DirectoryItem::OnUpdate(int /*updateFlags*/, istd::IPolymorphic* /*updateParamsPtr*/)
{
	ifpf::IHotfolderStatistics* objectPtr = GetObjectPtr();;
	if (objectPtr != NULL){
		UpdateStatistics(*objectPtr);
	}
}



} // namespace iqtfpf


