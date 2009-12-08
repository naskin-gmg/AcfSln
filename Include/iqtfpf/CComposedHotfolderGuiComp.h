#ifndef iqtfpf_CComposedHotfolderGuiComp_included
#define iqtfpf_CComposedHotfolderGuiComp_included


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


namespace iqtfpf
{


class CComposedHotfolderGuiComp:
			public iqtgui::TGuiObserverWrap<
						iqtgui::TGuiComponentBase<QWidget>,
						imod::TSingleModelObserverBase<iprm::IParamsSet> >,
			virtual public idoc::ICommandsProvider
{
	Q_OBJECT

public:
	typedef iqtgui::TGuiObserverWrap<
				iqtgui::TGuiComponentBase<QWidget>,
					imod::TSingleModelObserverBase<iprm::IParamsSet> > BaseClass;

	I_BEGIN_COMPONENT(CComposedHotfolderGuiComp)
		I_REGISTER_INTERFACE(imod::IObserver);
		I_REGISTER_INTERFACE(imod::IModelEditor);
		I_REGISTER_INTERFACE(idoc::ICommandsProvider);
		I_ASSIGN(m_hotfolderGuiCompPtr, "HotfolderGui", "Hotfolder UI", true, "HotfolderGui");
		I_ASSIGN(m_hotfolderObserverCompPtr, "HotfolderGui", "Hotfolder UI", true, "HotfolderGui");
		I_ASSIGN(m_settingsGuiCompPtr, "HotfolderParametersEditor", "Hotfolder parameters editor", true, "HotfolderParametersEditor");
		I_ASSIGN(m_settingsObserverCompPtr, "HotfolderParametersEditor", "Hotfolder parameters editor", true, "HotfolderParametersEditor");
		I_ASSIGN(m_stateModelIdAttrPtr, "StateModelParamsId", "Parameter set ID for the hotfolder's state parameters", true, "");
		I_ASSIGN(m_hotfolderParamsIdAttrPtr, "HotfolderParamsId", "Parameter set ID for the hotfolder settings", true, "");
	I_END_COMPONENT;

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

private Q_SLOTS:
	void OnSettings();

private:
	I_REF(iqtgui::IGuiObject, m_hotfolderGuiCompPtr);
	I_REF(imod::IObserver, m_hotfolderObserverCompPtr);
	I_REF(iqtgui::IGuiObject, m_settingsGuiCompPtr);
	I_REF(imod::IObserver, m_settingsObserverCompPtr);
	I_ATTR(istd::CString, m_stateModelIdAttrPtr);
	I_ATTR(istd::CString, m_hotfolderParamsIdAttrPtr);

	istd::TDelPtr<iqtgui::CGuiComponentDialog> m_settingsDialogPtr;

	mutable iqtgui::CHierarchicalCommand m_commands;
};


} // namespace iqtfpf


#endif // !iqtfpf_CComposedHotfolderGuiComp_included


