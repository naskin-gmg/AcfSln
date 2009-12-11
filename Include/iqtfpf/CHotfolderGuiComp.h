#ifndef iqtfpf_CHotfolderGuiComp_included
#define iqtfpf_CHotfolderGuiComp_included


// Qt includes
#include <QDir>
#include <QLabel>


// ACF includes
#include "imod/CMultiModelObserverBase.h"

#include "iproc/IProgressManager.h"

#include "idoc/ICommandsProvider.h"


#include "iqtgui/IIconProvider.h"
#include "iqtgui/TDesignerGuiObserverCompBase.h"
#include "iqtgui/CHierarchicalCommand.h"
#include "iqtgui/CGuiComponentDialog.h"


// AcfSln includes
#include "ifpf/IHotfolder.h"
#include "ifpf/IHotfolderProcessingItem.h"
#include "ifpf/IHotfolderStatistics.h"

#include "iqtfpf/Generated/ui_CHotfolderGuiComp.h"


namespace iqtfpf
{


class CHotfolderGuiComp:
			public iqtgui::TDesignerGuiObserverCompBase<Ui::CHotfolderGuiComp, ifpf::IHotfolder>,
			virtual public idoc::ICommandsProvider
{
	Q_OBJECT

public:
	typedef iqtgui::TDesignerGuiObserverCompBase<
				Ui::CHotfolderGuiComp,
				ifpf::IHotfolder> BaseClass;

	I_BEGIN_COMPONENT(CHotfolderGuiComp)
		I_ASSIGN(m_progressManagerCompPtr, "ProgressManager", "Progress manager for the hotfolder", true, "ProgressManager");
		I_ASSIGN(m_progressManagerGuiCompPtr, "ProgressManager", "Progress manager for the hotfolder", true, "ProgressManager");
		I_ASSIGN(m_stateIconsProviderCompPtr, "StateIcons", "Icons for the file state", true, "StateIcons");
		I_ASSIGN(m_statisticsCompPtr, "HotfolderStatistics", "Simple statistics of the hotfolder", true, "HotfolderStatistics");
		I_ASSIGN(m_statisticsHotfolderObserverCompPtr, "HotfolderStatistics", "Simple statistics of the hotfolder", true, "HotfolderStatistics");
		I_ASSIGN(m_statisticsModelCompPtr, "HotfolderStatistics", "Simple statistics of the hotfolder", true, "HotfolderStatistics");
	I_END_COMPONENT;

	CHotfolderGuiComp();

	// reimplemented (idoc::ICommandsProvider)
	virtual const idoc::IHierarchicalCommand* GetCommands() const;

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
	void AddFileItem(const ifpf::IHotfolderProcessingItem& fileItem);
	void UpdateProcessingCommands();
	void RebuildItemList();

private Q_SLOTS:
	void OnRun();
	void OnHold();
	void on_FileList_itemSelectionChanged();

private:
	class ProcessingItem: public QTreeWidgetItem, public imod::TSingleModelObserverBase<ifpf::IHotfolderProcessingItem>
	{
	public:
		typedef QTreeWidgetItem BaseClass;
		typedef imod::TSingleModelObserverBase<ifpf::IHotfolderProcessingItem> BaseClass2;

		ProcessingItem(const iqtgui::IIconProvider* iconsProviderPtr, QTreeWidget* parentPtr = NULL);

	protected:
		// reimplemented (imod::TSingleModelObserverBase)
		virtual void OnUpdate(int updateFlags, istd::IPolymorphic* updateParamsPtr);
	private:
		const iqtgui::IIconProvider* m_iconsProviderPtr;
	};

	class DirectoryItem: public QTreeWidgetItem, public imod::TSingleModelObserverBase<ifpf::IHotfolderStatistics>
	{
	public:
		typedef QTreeWidgetItem BaseClass;

		DirectoryItem(const QDir& directory, QTreeWidget* parentPtr);
		const QDir& GetDirectory() const;
		void AddFileItem(const ifpf::IHotfolderProcessingItem& fileItem, const iqtgui::IIconProvider* iconsProviderPtr);

	protected:
		void UpdateStatistics(const ifpf::IHotfolderStatistics& statistics);
		
		// reimplemented (imod::TSingleModelObserverBase)
		virtual void OnUpdate(int updateFlags, istd::IPolymorphic* updateParamsPtr);

	private:
		QDir m_directory;
		QLabel* m_statisticsLabel;
	};

	I_REF(iproc::IProgressManager, m_progressManagerCompPtr);
	I_REF(iqtgui::IGuiObject, m_progressManagerGuiCompPtr);
	I_REF(iqtgui::IIconProvider, m_stateIconsProviderCompPtr);
	I_REF(ifpf::IHotfolderStatistics, m_statisticsCompPtr);
	I_REF(imod::IObserver, m_statisticsHotfolderObserverCompPtr);
	I_REF(imod::IModel, m_statisticsModelCompPtr);

	iqtgui::CHierarchicalCommand m_hotfolderCommands;
	iqtgui::CHierarchicalCommand m_runCommand;
	iqtgui::CHierarchicalCommand m_holdCommand;
};


} // namespace iqtfpf


#endif // !iqtfpf_CHotfolderGuiComp_included


