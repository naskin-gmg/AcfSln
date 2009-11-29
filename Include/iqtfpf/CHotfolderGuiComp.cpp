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
			istd::CString lastFile = objectPtr->GetFileList().back();

			AddFileItem(lastFile, objectPtr->GetFileState(lastFile));
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

	iqtgui::CItemDelegate* itemDelegate = new iqtgui::CItemDelegate(20, this);
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

void CHotfolderGuiComp::AddFileItem(const istd::CString& fileItem, int fileState)
{
	QTreeWidgetItem* fileItemPtr = new QTreeWidgetItem(FileList);
	fileItemPtr->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

	QIcon stateIcon = GetIconForState(fileState);

	fileItemPtr->setIcon(0, stateIcon);
	fileItemPtr->setText(1, iqt::GetQString(fileItem));
	FileList->setItemWidget(fileItemPtr, 2, new QProgressBar());

	FileList->addTopLevelItem(fileItemPtr);
}


QIcon CHotfolderGuiComp::GetIconForState(int fileState) const
{
	if (m_stateIconsProviderCompPtr.IsValid()){
		if (m_stateIconsProviderCompPtr->GetIconCount() > fileState){
			return m_stateIconsProviderCompPtr->GetIcon(fileState);
		}
	}

	return QIcon();
}


// private slots

void CHotfolderGuiComp::OnSettings()
{
	if (m_settingsDialogPtr.IsValid()){
		m_settingsDialogPtr->exec();
	}
}


} // namespace iqtfpf


