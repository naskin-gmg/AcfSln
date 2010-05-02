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
#include "iqtgui/TDesignerGuiObserverCompBase.h"
#include "iqtgui/CHierarchicalCommand.h"
#include "iqtgui/CGuiComponentDialog.h"


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
		I_ASSIGN(m_progressManagerCompPtr, "ProgressManager", "Progress manager for the hotfolder", true, "ProgressManager");
		I_ASSIGN(m_progressManagerGuiCompPtr, "ProgressManager", "Progress manager for the hotfolder", true, "ProgressManager");
		I_ASSIGN(m_stateIconsProviderCompPtr, "StateIcons", "Icons for the file state", true, "StateIcons");
		I_ASSIGN(m_statisticsCompPtr, "HotfolderStatistics", "Simple statistics of the hotfolder", true, "HotfolderStatistics");
		I_ASSIGN(m_statisticsHotfolderObserverCompPtr, "HotfolderStatistics", "Simple statistics of the hotfolder", true, "HotfolderStatistics");
		I_ASSIGN(m_statisticsModelCompPtr, "HotfolderStatistics", "Simple statistics of the hotfolder", true, "HotfolderStatistics");
		I_ASSIGN(m_directoryItemGuiFactCompPtr, "DirectoryItemGui", "GUI for the directory tree item", false, "DirectoryItemGui");
		I_ASSIGN(m_directoryItemObserverFactCompPtr, "DirectoryItemGui", "GUI for the directory tree item", false, "DirectoryItemGui");
	I_END_COMPONENT;

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

private Q_SLOTS:
	void OnRun();
	void OnHold();
	void OnItemRemove();
	void OnItemCancel();
	void on_FileList_itemSelectionChanged();

private:
	class ProcessingItem: public QTreeWidgetItem, public imod::TSingleModelObserverBase<ifpf::IHotfolderProcessingItem>
	{
	public:
		typedef QTreeWidgetItem BaseClass;
		typedef imod::TSingleModelObserverBase<ifpf::IHotfolderProcessingItem> BaseClass2;

		ProcessingItem(const CHotfolderGuiComp& parent, QTreeWidget* treeWidgetPtr = NULL);

	protected:
		// reimplemented (imod::TSingleModelObserverBase)
		virtual void OnUpdate(int updateFlags, istd::IPolymorphic* updateParamsPtr);

	private:
		const CHotfolderGuiComp& m_parent;
	};

	class DirectoryItem: public QTreeWidgetItem
	{
	public:
		typedef QTreeWidgetItem BaseClass;

		DirectoryItem(const CHotfolderGuiComp& parent, const QDir& directory, QTreeWidget* treeWidgetPtr);
		const QDir& GetDirectory() const;
		void AddFileItem(const ifpf::IHotfolderProcessingItem& fileItem);

	private:
		QDir m_directory;
		const CHotfolderGuiComp& m_parent;
	};

	I_REF(iproc::IProgressManager, m_progressManagerCompPtr);
	I_REF(iqtgui::IGuiObject, m_progressManagerGuiCompPtr);
	I_REF(iqtgui::IIconProvider, m_stateIconsProviderCompPtr);
	I_REF(ifpf::IHotfolderStatistics, m_statisticsCompPtr);
	I_REF(imod::IObserver, m_statisticsHotfolderObserverCompPtr);
	I_REF(imod::IModel, m_statisticsModelCompPtr);
	I_FACT(iqtgui::IGuiObject, m_directoryItemGuiFactCompPtr);
	I_FACT(imod::IObserver, m_directoryItemObserverFactCompPtr);

	iqtgui::CHierarchicalCommand m_hotfolderCommands;
	iqtgui::CHierarchicalCommand m_runCommand;
	iqtgui::CHierarchicalCommand m_holdCommand;
	iqtgui::CHierarchicalCommand m_removeItemCommand;
	iqtgui::CHierarchicalCommand m_cancelItemCommand;

	typedef std::map<int, QIcon> StateIconsMap;

	StateIconsMap m_stateIconsMap;
};


} // namespace iqtfpf


#endif // !iqtfpf_CHotfolderGuiComp_included


