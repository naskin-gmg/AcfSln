#ifndef iqtfpf_CHotfolderGuiComp_included
#define iqtfpf_CHotfolderGuiComp_included


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
	I_END_COMPONENT;

	// reimplemented (idoc::ICommandsProvider)
	virtual const idoc::IHierarchicalCommand* GetCommands() const;

	// reimplemented (imod::IModelEditor)
	virtual void UpdateModel() const;
	virtual void UpdateEditor(int updateFlags = 0);

	// reimplemented (iqtgui::CGuiComponentBase)
	virtual void OnGuiCreated();

private:
	void AddFileItem(const ifpf::IHotfolderProcessingItem& fileItem);
	void UpdateProcessingCommands();

private Q_SLOTS:
	void OnRun();
	void OnHold();

private:
	class ProcessingItem: public QTreeWidgetItem, public imod::TSingleModelObserverBase<ifpf::IHotfolderProcessingItem>
	{
	public:
		ProcessingItem(iqtgui::IIconProvider* iconsProviderPtr);

	protected:
		// reimplemented (imod::TSingleModelObserverBase)
		virtual void OnUpdate(int updateFlags, istd::IPolymorphic* updateParamsPtr);
	private:
		iqtgui::IIconProvider* m_iconsProviderPtr;
	};

	I_REF(iproc::IProgressManager, m_progressManagerCompPtr);
	I_REF(iqtgui::IGuiObject, m_progressManagerGuiCompPtr);
	I_REF(iqtgui::IIconProvider, m_stateIconsProviderCompPtr);

	iqtgui::CHierarchicalCommand m_hotfolderCommands;
	iqtgui::CHierarchicalCommand m_runCommand;
	iqtgui::CHierarchicalCommand m_holdCommand;
};


} // namespace iqtfpf


#endif // !iqtfpf_CHotfolderGuiComp_included


