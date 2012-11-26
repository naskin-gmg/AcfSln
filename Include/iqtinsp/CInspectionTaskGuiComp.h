#ifndef iqtinsp_CInspectionTaskGuiComp_included
#define iqtinsp_CInspectionTaskGuiComp_included


// Qt includes
#include <QtCore/QString>
#include <QtCore/QMap>
#include <QtCore/QSet>
#include <QtGui/QToolBox>
#include <QtGui/QTabWidget>

// ACF includes
#include "istd/TPointerVector.h"
#include "imod/CMultiModelObserverBase.h"
#include "imod/CMultiModelDispatcherBase.h"
#include "iser/IFileLoader.h"
#include "ibase/IMessageContainer.h"
#include "iview/IShape.h"
#include "iview/IShapeFactory.h"
#include "iqtgui/TDesignerGuiObserverCompBase.h"
#include "iqt2d/IViewExtender.h"
#include "iqt2d/IViewProvider.h"

// ACF-Solutions includes
#include "iinsp/IInspectionTask.h"

#include "iqtinsp/iqtinsp.h"
#include "iqtinsp/Generated/ui_CInspectionTaskGuiComp.h"


namespace iqtinsp
{


class CInspectionTaskGuiComp:
			public iqtgui::TDesignerGuiObserverCompBase<Ui::CInspectionTaskGuiComp, iinsp::IInspectionTask>,
			protected imod::CMultiModelDispatcherBase
{
	Q_OBJECT

public:
	typedef iqtgui::TDesignerGuiObserverCompBase<Ui::CInspectionTaskGuiComp, iinsp::IInspectionTask> BaseClass;

	enum DataRole
	{
		DR_TASK_INDEX = Qt::UserRole + 1,
		DR_SHAPE_INDEX
	};

	I_BEGIN_COMPONENT(CInspectionTaskGuiComp);
		I_ASSIGN_MULTI_0(m_editorsCompPtr, "Editors", "List of GUI's for subtask parameters edition", true);
		I_ASSIGN_TO(m_editorGuisCompPtr, m_editorsCompPtr, true);
		I_ASSIGN_TO(m_editorObserversCompPtr, m_editorsCompPtr, true);
		I_ASSIGN_TO(m_editorViewExtendersCompPtr, m_editorsCompPtr, false);
		I_ASSIGN_MULTI_0(m_editorVisualInfosCompPtr, "EditorVisualInfos", "List editor visual informations for each editor", false);
		I_ASSIGN_TO(m_editorVisualModelsCompPtr, m_editorVisualInfosCompPtr, false);
		I_ASSIGN_MULTI_0(m_previewGuisCompPtr, "PreviewGuis", "List of GUI's used as preview of  subtask results (the same GUI object can be reused many times)", true);
		I_ASSIGN_TO(m_previewObserversCompPtr, m_previewGuisCompPtr, false);
		I_ASSIGN_TO(m_previewSceneProvidersCompPtr, m_previewGuisCompPtr, false);
		I_ASSIGN(m_resultShapeFactoryCompPtr, "ResultShapeFactory", "Creates shapes to display on preview (if iqt2d::IViewProvider is used)", false, "ResultShapeFactory");
		I_ASSIGN(m_paramsLoaderCompPtr, "ParamsLoader", "Loader for the parameter set", false, "ParamsLoader");
		I_ASSIGN(m_generalParamsGuiCompPtr, "GeneralParamsGui", "Gui of general parameters", false, "GeneralParamsGui");
		I_ASSIGN_TO(m_generalParamsObserverCompPtr, m_generalParamsGuiCompPtr, false);
		I_ASSIGN_TO(m_generalParamsEditorCompPtr, m_generalParamsGuiCompPtr, false);
		I_ASSIGN_MULTI_0(m_namesAttrPtr, "Names", "List of of gui names", false);
		I_ASSIGN(m_useHorizontalLayoutAttrPtr, "UseHorizontalLayout", "Use horizontal layout for tool box design", true, false);
		I_ASSIGN(m_tabOrientationAttrPtr, "TabBarOrientation", "Orientation of the tab bar for tab design\n 0 - North\n 1 - South\n 2 - West\n 3 - East", true, 0);
		I_ASSIGN(m_designTypeAttrPtr, "DesignType", "Type of design:\n* 1 - tool box\n* 2 - tab", true, 1);
		I_ASSIGN(m_useVerticalSpacerAttrPtr, "UseVerticalSpacer", "Insert vertical spacer to shrunk the space at the bottom", true, true);
	I_END_COMPONENT;

	CInspectionTaskGuiComp();

	// reimplemented (imod::IModelEditor)
	virtual void UpdateModel() const;
	virtual void UpdateEditor(int updateFlags = 0);

	// reimplemented (imod::IObserver)
	virtual bool OnAttached(imod::IModel* modelPtr);
	virtual bool OnDetached(imod::IModel* modelPtr);

protected:
	void UpdateProcessingState();
	void UpdateVisualElements();

	// reimplemented (iqtgui::CGuiComponentBase)
	virtual void OnGuiCreated();
	virtual void OnGuiDestroyed();
	virtual void OnGuiHidden();

	// reimplemented (imod::CMultiModelDispatcherBase)
	virtual void OnModelChanged(int modelId, int changeFlags, istd::IPolymorphic* updateParamsPtr);

private:
	void AddTaskMessagesToLog(const ibase::IMessageContainer& messageContainer, int taskIndex);
	void UpdateTaskMessages();
	void DoUpdateEditor(int taskIndex);
	void ActivateTaskShapes(int taskIndex);
	static QIcon GetCategoryIcon(istd::IInformationProvider::InformationCategory category);

Q_SIGNALS:
	void DoAutoTest();

protected Q_SLOTS:
	void OnEditorChanged(int index);
	void OnAutoTest();
	void on_TestAllButton_clicked();
	void on_AutoTestButton_clicked();
	void on_LoadParamsButton_clicked();
	void on_SaveParamsButton_clicked();
	void on_MessageList_itemSelectionChanged();
	void on_MessageList_itemDoubleClicked(QTreeWidgetItem* item, int column);

private:
	I_MULTIREF(imod::IModelEditor, m_editorsCompPtr);
	I_MULTIREF(iqtgui::IGuiObject, m_editorGuisCompPtr);
	I_MULTIREF(imod::IObserver, m_editorObserversCompPtr);
	I_MULTIREF(iqt2d::IViewExtender, m_editorViewExtendersCompPtr);
	I_MULTIREF(iqtgui::IVisualStatusProvider, m_editorVisualInfosCompPtr);
	I_MULTIREF(imod::IModel, m_editorVisualModelsCompPtr);
	I_MULTIREF(iqtgui::IGuiObject, m_previewGuisCompPtr);
	I_MULTIREF(imod::IObserver, m_previewObserversCompPtr);
	I_MULTIREF(iqt2d::IViewProvider, m_previewSceneProvidersCompPtr);
	I_REF(iview::IShapeFactory, m_resultShapeFactoryCompPtr);
	I_MULTIATTR(QString, m_namesAttrPtr);
	I_REF(iser::IFileLoader, m_paramsLoaderCompPtr);
	I_REF(iqtgui::IGuiObject, m_generalParamsGuiCompPtr);
	I_REF(imod::IObserver, m_generalParamsObserverCompPtr);
	I_REF(imod::IModelEditor, m_generalParamsEditorCompPtr);
	I_ATTR(bool, m_useHorizontalLayoutAttrPtr);
	I_ATTR(int, m_tabOrientationAttrPtr);
	I_ATTR(int, m_designTypeAttrPtr);
	I_ATTR(bool, m_useVerticalSpacerAttrPtr);

	int m_currentGuiIndex;
	typedef QMap<int, int> GuiMap;
	GuiMap m_tabToStackIndexMap;	// map GUI index (for editors) to stack index (for preview GUI)
	GuiMap m_stackIndexToTabMap;	// map stack index (for preview GUI) to GUI index (for editors)
	GuiMap m_tabToGuiIndexMap;		// map tab index to editor index

	typedef QMap< int, istd::TPointerVector<iview::IShape> > ResultShapesMap;	// Map GUI index to list of result shapes
	ResultShapesMap m_resultShapesMap;

	typedef QSet<imod::IModelEditor*> EditorsList;
	EditorsList m_editorsList;

	QToolBox* m_toolBoxPtr;
	QTabWidget* m_tabWidgetPtr;
};


} // namespace iqtinsp


#endif // !iqtinsp_CInspectionTaskGuiComp_included


