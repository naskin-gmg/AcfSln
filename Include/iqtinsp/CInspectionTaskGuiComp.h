#ifndef iqtinsp_CInspectionTaskGuiComp_included
#define iqtinsp_CInspectionTaskGuiComp_included


// Qt includes
#include <QtCore/QString>
#include <QtCore/QMap>
#include <QtCore/QSet>
#include<QtCore/QtGlobal>
#if QT_VERSION >= 0x050000
#include <QtWidgets/QToolBox>
#include <QtWidgets/QTabWidget>
#else
#include <QtGui/QToolBox>
#include <QtGui/QTabWidget>
#endif

// ACF includes
#include <istd/TPointerVector.h>
#include <imod/CMultiModelObserverBase.h>
#include <imod/CMultiModelDispatcherBase.h>
#include <ifile/IFilePersistence.h>
#include <ilog/CMessageContainer.h>
#include <iview/IShape.h>
#include <iview/IShapeFactory.h>
#include <iqtgui/TDesignerGuiObserverCompBase.h>
#include <iqt2d/IViewExtender.h>
#include <iqt2d/IViewProvider.h>

// ACF-Solutions includes
#include <iinsp/IInspectionTask.h>
#include <iqtinsp/TResultShapeCreatorWrap.h>
#include <GeneratedFiles/iqtinsp/ui_CInspectionTaskGuiComp.h>


namespace iqtinsp
{


class CInspectionTaskGuiCompBase: public iqtgui::TDesignerGuiObserverCompBase<Ui::CInspectionTaskGuiComp, iinsp::IInspectionTask>
{
public:
	typedef iqtgui::TDesignerGuiObserverCompBase<Ui::CInspectionTaskGuiComp, iinsp::IInspectionTask> BaseClass;

	I_BEGIN_BASE_COMPONENT(CInspectionTaskGuiCompBase);
		I_ASSIGN_MULTI_0(m_namesAttrPtr, "Names", "List of of gui names", false);
		I_ASSIGN(m_useHorizontalLayoutAttrPtr, "UseHorizontalLayout", "Use horizontal layout for tool box design", true, false);
		I_ASSIGN(m_tabOrientationAttrPtr, "TabBarOrientation", "Orientation of the tab bar for tab design\n 0 - North\n 1 - South\n 2 - West\n 3 - East", true, 0);
		I_ASSIGN(m_designTypeAttrPtr, "DesignType", "Type of design:\n* 1 - tool box\n* 2 - tab", true, 1);
		I_ASSIGN(m_useVerticalSpacerAttrPtr, "UseVerticalSpacer", "Insert vertical spacer to shrunk the space at the bottom", true, true);
	I_END_COMPONENT;

protected:
	I_MULTIATTR(QString, m_namesAttrPtr);
	I_ATTR(bool, m_useHorizontalLayoutAttrPtr);
	I_ATTR(int, m_tabOrientationAttrPtr);
	I_ATTR(int, m_designTypeAttrPtr);
	I_ATTR(bool, m_useVerticalSpacerAttrPtr);
	I_ATTR(int, m_inspectionTaskIndexAttrPtr);
};


class CInspectionTaskGuiComp:
			public TResultShapeCreatorWrap<CInspectionTaskGuiCompBase>,
			protected imod::CMultiModelDispatcherBase
{
	Q_OBJECT

public:
	typedef TResultShapeCreatorWrap<CInspectionTaskGuiCompBase> BaseClass;

	enum DataRole
	{
		DR_TASK_INDEX = Qt::UserRole + 1,
		DR_SHAPE_INDICES
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
		I_ASSIGN(m_paramsLoaderCompPtr, "ParamsLoader", "Loader for the parameter set", false, "ParamsLoader");
		I_ASSIGN(m_generalParamsGuiCompPtr, "GeneralParamsGui", "Gui of general parameters", false, "GeneralParamsGui");
		I_ASSIGN_TO(m_generalParamsObserverCompPtr, m_generalParamsGuiCompPtr, false);
		I_ASSIGN_TO(m_generalParamsEditorCompPtr, m_generalParamsGuiCompPtr, false);
	I_END_COMPONENT;

	CInspectionTaskGuiComp();

	// reimplemented (imod::IModelEditor)
	virtual void UpdateModel() const;
	virtual void UpdateEditor(const istd::IChangeable::ChangeSet& changeSet);

	// reimplemented (imod::IObserver)
	virtual bool OnModelAttached(imod::IModel* modelPtr, istd::IChangeable::ChangeSet& changeMask);
	virtual bool OnModelDetached(imod::IModel* modelPtr);

protected:
	void UpdateProcessingState();
	void UpdateVisualElements();

	// reimplemented (iqtgui::CGuiComponentBase)
	virtual void OnGuiCreated();
	virtual void OnGuiDestroyed();
	virtual void OnGuiHidden();

	// reimplemented (imod::CMultiModelDispatcherBase)
	virtual void OnModelChanged(int modelId, const istd::IChangeable::ChangeSet& changeSet);

protected Q_SLOTS:
	void OnEditorChanged(int index);
	void OnAutoTest();
	void on_TestAllButton_clicked();
	void on_AutoTestButton_clicked();
	void OnLoadParams();
	void OnSaveParams();
	void OnCopyAll();
	void OnPasteAll();
	void OnCopyCurrent();
	void OnPasteCurrent();
	void on_MessageList_itemSelectionChanged();
	void on_MessageList_itemDoubleClicked(QTreeWidgetItem* item, int column);

Q_SIGNALS:
	void DoAutoTest();

private:
	typedef QSet<int> ShapeIndices;

	void AddTaskMessagesToLog(const ilog::IMessageContainer& messageContainer, int taskIndex, bool isAuxiliary);
	void UpdateTaskMessages();
	void DoUpdateEditor(int taskIndex);
	void ActivateTaskShapes(int taskIndex, const ShapeIndices& shapeIndices);
	void CreateMenu();
	void UpdateMenu();
	bool CopyTaskParametersToClipboard(iser::ISerializable* objectPtr, const char* mimeType) const;
	bool ReadTaskParametersFromClipboard(iser::ISerializable* objectPtr, const char* mimeType);

	// static methods
	static QIcon GetCategoryIcon(istd::IInformationProvider::InformationCategory category);

private:
	I_MULTIREF(imod::IModelEditor, m_editorsCompPtr);
	I_MULTIREF(iqtgui::IGuiObject, m_editorGuisCompPtr);
	I_MULTIREF(imod::IObserver, m_editorObserversCompPtr);
	I_MULTIREF(iqt2d::IViewExtender, m_editorViewExtendersCompPtr);
	I_MULTIREF(iqtgui::IVisualStatus, m_editorVisualInfosCompPtr);
	I_MULTIREF(imod::IModel, m_editorVisualModelsCompPtr);
	I_MULTIREF(iqtgui::IGuiObject, m_previewGuisCompPtr);
	I_MULTIREF(imod::IObserver, m_previewObserversCompPtr);
	I_MULTIREF(iqt2d::IViewProvider, m_previewSceneProvidersCompPtr);
	I_REF(ifile::IFilePersistence, m_paramsLoaderCompPtr);
	I_REF(iqtgui::IGuiObject, m_generalParamsGuiCompPtr);
	I_REF(imod::IObserver, m_generalParamsObserverCompPtr);
	I_REF(imod::IModelEditor, m_generalParamsEditorCompPtr);

	int m_currentGuiIndex;
	bool m_testStarted;

	typedef QMap<int, int> GuiMap;
	GuiMap m_tabToStackIndexMap;	// map GUI index (for editors) to stack index (for preview GUI)
	GuiMap m_stackIndexToTabMap;	// map stack index (for preview GUI) to GUI index (for editors)
	GuiMap m_tabToGuiIndexMap;		// map tab index to editor index

	typedef QMap<int, ilog::CMessageContainer> ResultMessagesMap;
	ResultMessagesMap m_resultMessagesMap;
	ResultMessagesMap m_tempMessagesMap;

	typedef QMap<int, istd::TPointerVector<iview::IShape> > ResultShapesMap;	// Map GUI index to list of result shapes
	ResultShapesMap m_resultShapesMap;
	ResultShapesMap m_tempShapesMap;

	typedef QSet<imod::IModelEditor*> EditorsList;
	EditorsList m_editorsList;

	QToolBox* m_toolBoxPtr;
	QTabWidget* m_tabWidgetPtr;
	QStackedWidget* m_stackedWidgetPtr;
	QButtonGroup* m_buttonGroupPtr;

	QAction* m_copyAllActionPtr;
	QAction* m_pasteAllActionPtr;
	QAction* m_copyCurrentTaskActionPtr;
	QAction* m_pasteCurrentTaskActionPtr;
	QAction* m_loadAllActionPtr;
	QAction* m_saveAllActionPtr;
};


} // namespace iqtinsp


#endif // !iqtinsp_CInspectionTaskGuiComp_included


