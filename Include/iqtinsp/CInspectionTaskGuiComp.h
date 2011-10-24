#ifndef iqtinsp_CInspectionTaskGuiComp_included
#define iqtinsp_CInspectionTaskGuiComp_included


// STL includes
#include <map>
#include <set>

// ACF includes
#include "istd/CString.h"
#include "imod/CMultiModelObserverBase.h"
#include "iser/IFileLoader.h"
#include "iqtgui/TDesignerGuiObserverCompBase.h"
#include "iqt2d/ISceneExtender.h"
#include "iqt2d/ISceneProvider.h"

#include "iinsp/IInspectionTask.h"

#include "iqtinsp/Generated/ui_CInspectionTaskGuiComp.h"

#include "iqtinsp/iqtinsp.h"


namespace iqtinsp
{


class CInspectionTaskGuiComp:
			public iqtgui::TDesignerGuiObserverCompBase<Ui::CInspectionTaskGuiComp, iinsp::IInspectionTask>
{
	Q_OBJECT

public:
	typedef iqtgui::TDesignerGuiObserverCompBase<Ui::CInspectionTaskGuiComp, iinsp::IInspectionTask> BaseClass;

	I_BEGIN_COMPONENT(CInspectionTaskGuiComp);
		I_ASSIGN_MULTI_0(m_editorsCompPtr, "Editors", "List of GUI's for subtask parameters edition", true);
		I_ASSIGN_TO(m_guisCompPtr, m_editorsCompPtr, true);
		I_ASSIGN_TO(m_observersCompPtr, m_editorsCompPtr, true);
		I_ASSIGN_TO(m_extendersCompPtr, m_editorsCompPtr, false);
		I_ASSIGN_MULTI_0(m_previewGuisCompPtr, "PreviewGuis", "List of GUI's used as preview of  subtask results (the same GUI object can be reused many times)", true);
		I_ASSIGN_TO(m_previewObserversCompPtr, m_previewGuisCompPtr, false);
		I_ASSIGN_TO(m_previewSceneProvidersCompPtr, m_previewGuisCompPtr, false);
		I_ASSIGN(m_paramsLoaderCompPtr, "ParamsLoader", "Loader for the parameter set", false, "ParamsLoader");
		I_ASSIGN(m_generalParamsGuiCompPtr, "GeneralParamsGui", "Gui of general parameters", false, "GeneralParamsGui");
		I_ASSIGN_TO(m_generalParamsObserverCompPtr, m_generalParamsGuiCompPtr, false);
		I_ASSIGN_TO(m_generalParamsEditorCompPtr, m_generalParamsGuiCompPtr, false);
		I_ASSIGN_MULTI_0(m_namesAttrPtr, "Names", "List of of gui names", false);
		I_ASSIGN(m_useHorizontalLayoutAttrPtr, "UseHorizontalLayout", "Use horizontal layout for tool box design", true, false);
		I_ASSIGN(m_tabOrientationAttrPtr, "TabBarOrientation", "Orientation of the tab bar for tab design\n 0 - North\n 1 - South\n 2 - West\n 3 - East", true, 0);
		I_ASSIGN(m_designTypeAttrPtr, "DesignType", "Type of design:\n* 1 - tool box\n* 2 - tab", true, 1);
	I_END_COMPONENT;

	CInspectionTaskGuiComp();

	// reimplemented (imod::IModelEditor)
	virtual void UpdateModel() const;
	virtual void UpdateEditor(int updateFlags = 0);

	// reimplemented (imod::IObserver)
	virtual bool OnAttached(imod::IModel* modelPtr);
	virtual bool OnDetached(imod::IModel* modelPtr);

protected:
	// reimplemented (iqtgui::CGuiComponentBase)
	virtual void OnGuiCreated();
	virtual void OnGuiDestroyed();

Q_SIGNALS:
	void DoAutoTest();

protected Q_SLOTS:
	void OnEditorChanged(int index);
	void OnAutoTest();
	void on_TestAllButton_clicked();
	void on_AutoTestButton_clicked();
	void on_LoadParamsButton_clicked();
	void on_SaveParamsButton_clicked();

private:
	void UpdateProcessingState();

private:
	class TasksObserver: public imod::CMultiModelObserverBase
	{
	public:
		TasksObserver(CInspectionTaskGuiComp* parentPtr);
	protected:
		virtual void OnUpdate(imod::IModel* modelPtr, int updateFlags, istd::IPolymorphic* updateParamsPtr);

	private:
		CInspectionTaskGuiComp& m_parent;
	};

	I_MULTIREF(imod::IModelEditor, m_editorsCompPtr);
	I_MULTIREF(iqtgui::IGuiObject, m_guisCompPtr);
	I_MULTIREF(imod::IObserver, m_observersCompPtr);
	I_MULTIREF(iqt2d::ISceneExtender, m_extendersCompPtr);
	I_MULTIREF(iqtgui::IGuiObject, m_previewGuisCompPtr);
	I_MULTIREF(imod::IObserver, m_previewObserversCompPtr);
	I_MULTIREF(iqt2d::ISceneProvider, m_previewSceneProvidersCompPtr);
	I_MULTIATTR(istd::CString, m_namesAttrPtr);
	I_REF(iser::IFileLoader, m_paramsLoaderCompPtr);
	I_REF(iqtgui::IGuiObject, m_generalParamsGuiCompPtr);
	I_REF(imod::IObserver, m_generalParamsObserverCompPtr);
	I_REF(imod::IModelEditor, m_generalParamsEditorCompPtr);
	I_ATTR(bool, m_useHorizontalLayoutAttrPtr);
	I_ATTR(int, m_tabOrientationAttrPtr);
	I_ATTR(int, m_designTypeAttrPtr);

	TasksObserver m_tasksObserver;

	int m_currentGuiIndex;
	typedef std::map<int, int> GuiMap;
	GuiMap m_tabToStackIndexMap;	// map GUI index (for editors) to stack index (for preview GUI)
	GuiMap m_stackIndexToTabMap;	// map stack index (for preview GUI) to GUI index (for editors)
	typedef std::set<imod::IModelEditor*> EditorsList;
	EditorsList m_editorsList;
};


} // namespace iqtinsp


#endif // !iqtinsp_CInspectionTaskGuiComp_included


