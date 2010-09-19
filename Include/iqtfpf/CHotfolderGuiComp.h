#ifndef iqtfpf_CHotfolderGuiComp_included
#define iqtfpf_CHotfolderGuiComp_included


// Qt includes
#include <QDir>
#include <QLabel>


// ACF includes
#include "imod/CMultiModelObserverBase.h"
#include "imod/CModelProxy.h"

#include "iproc/IProgressManager.h"

#include "ibase/ICommandsProvider.h"


#include "iqtgui/IIconProvider.h"
#include "iqtgui/IDialog.h"
#include "iqtgui/TDesignerGuiObserverCompBase.h"
#include "iqtgui/CHierarchicalCommand.h"
#include "iqtgui/CGuiComponentDialog.h"
#include "iqtgui/CExtLineEdit.h"


// AcfSln includes
#include "ifpf/IHotfolderProcessingInfo.h"
#include "ifpf/IHotfolderProcessingItem.h"
#include "ifpf/IHotfolderStatistics.h"

#include "iqtfpf/Generated/ui_CHotfolderGuiComp.h"


namespace iqtfpf
{


class CHotfolderGuiComp:
			public iqtgui::TDesignerGuiObserverCompBase<Ui::CHotfolderGuiComp, ifpf::IHotfolderProcessingInfo>,
			public imod::CModelProxy,
			virtual public ibase::ICommandsProvider
{
	Q_OBJECT

public:
	typedef iqtgui::TDesignerGuiObserverCompBase<
				Ui::CHotfolderGuiComp,
				ifpf::IHotfolderProcessingInfo> BaseClass;
	typedef imod::CModelProxy BaseClass2;

	I_BEGIN_COMPONENT(CHotfolderGuiComp)
		I_REGISTER_INTERFACE(imod::IModel);
		I_REGISTER_INTERFACE(ibase::ICommandsProvider);
		I_ASSIGN(m_stateIconsProviderCompPtr, "StateIcons", "Icons for the file state", true, "StateIcons");
		I_ASSIGN(m_statisticsCompPtr, "HotfolderStatistics", "Simple statistics of the hotfolder", true, "HotfolderStatistics");
		I_ASSIGN(m_statisticsHotfolderObserverCompPtr, "HotfolderStatistics", "Simple statistics of the hotfolder", true, "HotfolderStatistics");
		I_ASSIGN(m_statisticsModelCompPtr, "HotfolderStatistics", "Simple statistics of the hotfolder", true, "HotfolderStatistics");
		I_ASSIGN(m_directoryItemGuiFactCompPtr, "DirectoryItemGui", "GUI for the directory tree item", false, "DirectoryItemGui");
		I_ASSIGN(m_directoryItemObserverFactCompPtr, "DirectoryItemGui", "GUI for the directory tree item", false, "DirectoryItemGui");
		I_ASSIGN(m_processingItemPreviewCompPtr, "ProcessingItemPreview", "GUI for the processing item's preview", false, "ProcessingItemPreview");
		I_ASSIGN(m_processingParamsDialogCompPtr, "ProcessingParamsDialog", "Dialog for the processing parameters", false, "ProcessingParamsDialog");
	I_END_COMPONENT;

	CHotfolderGuiComp();

	// reimplemented (ibase::ICommandsProvider)
	virtual const ibase::IHierarchicalCommand* GetCommands() const;

	// reimplemented (imod::IModelEditor)
	virtual void UpdateModel() const;
	virtual void UpdateEditor(int updateFlags = 0);

	// reimplemented (TGuiObserverWrap)
	virtual void OnGuiModelAttached();
	virtual void OnGuiModelDetached();

	// reimplemented (iqtgui::CGuiComponentBase)
	virtual void OnGuiCreated();
	virtual void OnGuiDestroyed();

private:
	typedef std::vector<ifpf::IHotfolderProcessingItem*> ProcessingItems;

	void AddFileItem(const ifpf::IHotfolderProcessingItem& fileItem);
	void UpdateProcessingCommands();
	void UpdateItemCommands();
	void RebuildItemList();
	ProcessingItems GetSelectedProcessingItems() const;
	QIcon GetStateIcon(int fileState) const;
	void UpdateItemsVisibility(const QString& textFilter, bool showOnlyErrors);

private Q_SLOTS:
	void OnRun();
	void OnHold();
	void OnItemRemove();
	void OnItemCancel();
	void OnRestart();
	void OnShowProcessingParamsDialog();
	void OnContextMenuRequested(const QPoint& menuPoint);
	void on_FileList_itemSelectionChanged();
	void on_AllRadio_toggled(bool isChecked);
	void on_ErrorsRadio_toggled(bool isChecked);
	void OnTextFilterChanged(const QString& filterText);

private:
	class ProcessingItem: public QTreeWidgetItem, public imod::TSingleModelObserverBase<ifpf::IHotfolderProcessingItem>
	{
	public:
		typedef QTreeWidgetItem BaseClass;
		typedef imod::TSingleModelObserverBase<ifpf::IHotfolderProcessingItem> BaseClass2;

		ProcessingItem(CHotfolderGuiComp& parent, QTreeWidget* treeWidgetPtr = NULL);

	protected:
		// reimplemented (imod::TSingleModelObserverBase)
		virtual void OnUpdate(int updateFlags, istd::IPolymorphic* updateParamsPtr);

	private:
		CHotfolderGuiComp& m_parent;
	};

	class DirectoryItem: public QTreeWidgetItem
	{
	public:
		typedef QTreeWidgetItem BaseClass;

		DirectoryItem(CHotfolderGuiComp& parent, const QDir& directory, QTreeWidget* treeWidgetPtr);
		const QDir& GetDirectory() const;
		void AddFileItem(const ifpf::IHotfolderProcessingItem& fileItem);

	private:
		QDir m_directory;
		CHotfolderGuiComp& m_parent;
	};

	I_REF(iqtgui::IIconProvider, m_stateIconsProviderCompPtr);
	I_REF(ifpf::IHotfolderStatistics, m_statisticsCompPtr);
	I_REF(imod::IObserver, m_statisticsHotfolderObserverCompPtr);
	I_REF(imod::IModel, m_statisticsModelCompPtr);
	I_FACT(iqtgui::IGuiObject, m_directoryItemGuiFactCompPtr);
	I_FACT(imod::IObserver, m_directoryItemObserverFactCompPtr);
	I_REF(iqtgui::IGuiObject, m_processingItemPreviewCompPtr);
	I_REF(iqtgui::IDialog, m_processingParamsDialogCompPtr);

	iqtgui::CHierarchicalCommand m_hotfolderCommands;
	iqtgui::CHierarchicalCommand m_runCommand;
	iqtgui::CHierarchicalCommand m_holdCommand;
	iqtgui::CHierarchicalCommand m_removeItemCommand;
	iqtgui::CHierarchicalCommand m_cancelItemCommand;
	iqtgui::CHierarchicalCommand m_restartItemCommand;
	iqtgui::CHierarchicalCommand m_showProcessingDialogCommand;

	typedef std::map<int, QIcon> StateIconsMap;

	iqtgui::CExtLineEdit* m_filterEditor;
	StateIconsMap m_stateIconsMap;
};


} // namespace iqtfpf


#endif // !iqtfpf_CHotfolderGuiComp_included


