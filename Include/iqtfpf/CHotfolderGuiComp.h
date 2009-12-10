#ifndef iqtfpf_CHotfolderGuiComp_included
#define iqtfpf_CHotfolderGuiComp_included


// Qt includes
#include <QDir>


// ACF includes
#include "iproc/IProgressManager.h"

#include "idoc/ICommandsProvider.h"


#include "iqtgui/IIconProvider.h"
#include "iqtgui/TDesignerGuiObserverCompBase.h"
#include "iqtgui/CHierarchicalCommand.h"
#include "iqtgui/CGuiComponentDialog.h"


// AcfSln includes
#include "ifpf/IHotfolder.h"
#include "ifpf/IHotfolderProcessingItem.h"

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
		I_ASSIGN(m_itemObserverCompPtr, "ProcessingItemView", "View of the processing item", true, "ProcessingItemView");
		I_ASSIGN(m_itemGuiCompPtr, "ProcessingItemView", "View of the processing item", true, "ProcessingItemView");

	I_END_COMPONENT;

	CHotfolderGuiComp();

	// reimplemented (idoc::ICommandsProvider)
	virtual const idoc::IHierarchicalCommand* GetCommands() const;

	// reimplemented (imod::IModelEditor)
	virtual void UpdateModel() const;
	virtual void UpdateEditor(int updateFlags = 0);

	// reimplemented (TGuiObserverWrap)
	virtual void OnGuiModelAttached();

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

		ProcessingItem(iqtgui::IIconProvider* iconsProviderPtr, QTreeWidget* parentPtr = NULL);

	protected:
		// reimplemented (imod::TSingleModelObserverBase)
		virtual void OnUpdate(int updateFlags, istd::IPolymorphic* updateParamsPtr);
	private:
		iqtgui::IIconProvider* m_iconsProviderPtr;
	};

	class DirectoryItem: public QTreeWidgetItem
	{
	public:
		typedef QTreeWidgetItem BaseClass;

		DirectoryItem(const QDir& directory, QTreeWidget* parentPtr)
			:BaseClass(parentPtr),
			m_directory(directory)
		{
			QFont font;
			font.setBold(true);

			setText(0, directory.absolutePath());
			setFont(0, font);

			parentPtr->addTopLevelItem(this);
		}

		QDir GetDirectory() const
		{
			return m_directory;
		}
	private:
		QDir m_directory;
	};

	I_REF(iproc::IProgressManager, m_progressManagerCompPtr);
	I_REF(iqtgui::IGuiObject, m_progressManagerGuiCompPtr);
	I_REF(iqtgui::IIconProvider, m_stateIconsProviderCompPtr);
	I_REF(imod::IObserver, m_itemObserverCompPtr);
	I_REF(iqtgui::IGuiObject, m_itemGuiCompPtr);

	iqtgui::CHierarchicalCommand m_hotfolderCommands;
	iqtgui::CHierarchicalCommand m_runCommand;
	iqtgui::CHierarchicalCommand m_holdCommand;

	imod::IModel* m_lastItemModelPtr;
};


} // namespace iqtfpf


#endif // !iqtfpf_CHotfolderGuiComp_included


