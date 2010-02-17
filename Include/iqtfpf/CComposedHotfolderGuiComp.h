#ifndef iqtfpf_CComposedHotfolderGuiComp_included
#define iqtfpf_CComposedHotfolderGuiComp_included


// ACF includes
#include "iproc/IProgressManager.h"

#include "ibase/ICommandsProvider.h"


#include "iqtgui/IIconProvider.h"
#include "iqtgui/IDialog.h"
#include "iqtgui/TDesignerGuiObserverCompBase.h"
#include "iqtgui/CHierarchicalCommand.h"


// AcfSln includes
#include "ifpf/IHotfolder.h"
#include "ifpf/IHotfolderProcessingItem.h"


namespace iqtfpf
{


class CComposedHotfolderGuiComp:
			public iqtgui::TGuiObserverWrap<
						iqtgui::TGuiComponentBase<QWidget>,
						imod::TSingleModelObserverBase<iprm::IParamsSet> >,
			virtual public ibase::ICommandsProvider
{
	Q_OBJECT

public:
	typedef iqtgui::TGuiObserverWrap<
				iqtgui::TGuiComponentBase<QWidget>,
					imod::TSingleModelObserverBase<iprm::IParamsSet> > BaseClass;

	I_BEGIN_COMPONENT(CComposedHotfolderGuiComp)
		I_REGISTER_INTERFACE(imod::IObserver);
		I_REGISTER_INTERFACE(imod::IModelEditor);
		I_REGISTER_INTERFACE(ibase::ICommandsProvider);
		I_ASSIGN(m_hotfolderGuiCompPtr, "HotfolderGui", "Hotfolder UI", true, "HotfolderGui");
		I_ASSIGN(m_hotfolderObserverCompPtr, "HotfolderGui", "Hotfolder UI", true, "HotfolderGui");
		I_ASSIGN(m_settingsDialogCompPtr, "SettingsDialog", "Hotfolder parameters editor", true, "SettingsDialog");
		I_ASSIGN(m_stateModelIdAttrPtr, "StateModelParamsId", "Parameter set ID for the hotfolder's state parameters", true, "");
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

private Q_SLOTS:
	void OnSettings();

private:
	I_REF(iqtgui::IGuiObject, m_hotfolderGuiCompPtr);
	I_REF(imod::IObserver, m_hotfolderObserverCompPtr);
	I_REF(iqtgui::IDialog, m_settingsDialogCompPtr);
	I_ATTR(istd::CString, m_stateModelIdAttrPtr);
	I_ATTR(istd::CString, m_hotfolderParamsIdAttrPtr);

	mutable iqtgui::CHierarchicalCommand m_commands;
};


} // namespace iqtfpf


#endif // !iqtfpf_CComposedHotfolderGuiComp_included


