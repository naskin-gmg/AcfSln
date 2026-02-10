// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#ifndef iqtinsp_CInspectionTaskGuiComp_included
#define iqtinsp_CInspectionTaskGuiComp_included


// Qt includes
#include <QtCore/QString>
#include <QtCore/QMap>
#include <QtCore/QSet>
#include<QtCore/QtGlobal>
#include<QtCore/QTimer>
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
#include <ibase/ICommandsProvider.h>
#include <ibase/ICommandsDisplayer.h>
#include <ifile/IFilePersistence.h>
#include <ilog/CMessageContainer.h>
#include <iview/IShape.h>
#include <iview/IShapeFactory.h>
#include <iview/IViewEventObserver.h>
#include <iqtgui/TDesignerGuiObserverCompBase.h>
#include <iqtgui/TRestorableGuiWrap.h>
#include <iqtgui/CHierarchicalCommand.h>
#include <iqtgui/TMakeStateIconWrapper.h>
#include <iqt2d/IViewExtender.h>
#include <iqt2d/IViewProvider.h>
#include <iprm/ISelectionParam.h>
#include <iinsp/IInspectionTask.h>
#include <iqtinsp/TResultShapeCreatorWrap.h>
#include <iauth/IRightsProvider.h>

#include <GeneratedFiles/iqtinsp/ui_CInspectionTaskGuiComp.h>


namespace iqtinsp
{


class CInspectionTaskGuiCompBase:
	public iqtgui::StateIconWrapper< iqtgui::TDesignerGuiObserverCompBase<
						Ui::CInspectionTaskGuiComp, iinsp::IInspectionTask> >
{
public:
	typedef iqtgui::StateIconWrapper< iqtgui::TDesignerGuiObserverCompBase<
		Ui::CInspectionTaskGuiComp, iinsp::IInspectionTask> > BaseClass;

	I_BEGIN_BASE_COMPONENT(CInspectionTaskGuiCompBase);
		I_ASSIGN_MULTI_0(m_namesAttrPtr, "Names", "List of GUI names", true);
		I_ASSIGN_MULTI_0(m_shortNamesAttrPtr, "ShortNames", "List of GUI short names (stacked mode only)", false);
		I_ASSIGN_MULTI_0(m_iconsAttrPtr, "Icons", "List of GUI icons (stacked mode only)", false);
		I_ASSIGN(m_useHorizontalLayoutAttrPtr, "UseHorizontalLayout", "Use horizontal layout for tool box design", true, false);
		I_ASSIGN(m_tabOrientationAttrPtr, "TabBarOrientation", "Orientation of the tab bar for tab design\n 0 - North\n 1 - South\n 2 - West\n 3 - East", true, 0);
		I_ASSIGN(m_designTypeAttrPtr, "DesignType", "Type of design:\n* 0 - stack\n* 1 - tool box\n* 2 - tab", true, 1);
		I_ASSIGN(m_useVerticalSpacerAttrPtr, "UseVerticalSpacer", "Insert vertical spacer to shrunk the space at the bottom", true, true);
		I_ASSIGN_MULTI_0(m_commandsProvidersCompPtr, "TaskCommandsProviders", "Additional commands per task", false);
		I_ASSIGN(m_taskCommandsDisplayerPtr, "TaskCommandsDisplayer", "Displayer for additional commands", false, "");
		I_ASSIGN(m_buttonNameAttrPtr, "ButtonText", "Text shown on the button", false, "");
		I_ASSIGN(m_buttonTooltipAttrPtr, "ButtonTooltip", "Tooltip shown on the button", false, "");
		I_ASSIGN(m_frameNameAttrPtr, "FrameText", "Text shown on the frame", false, "");
	I_END_COMPONENT;

protected:
	I_MULTITEXTATTR(m_namesAttrPtr);
	I_MULTITEXTATTR(m_shortNamesAttrPtr);
	I_MULTIATTR(QString, m_iconsAttrPtr);
	I_ATTR(bool, m_useHorizontalLayoutAttrPtr);
	I_ATTR(int, m_tabOrientationAttrPtr);
	I_ATTR(int, m_designTypeAttrPtr);
	I_ATTR(bool, m_useVerticalSpacerAttrPtr);
	I_ATTR(int, m_inspectionTaskIndexAttrPtr);
	I_TEXTATTR(m_buttonNameAttrPtr);
	I_TEXTATTR(m_buttonTooltipAttrPtr);
	I_TEXTATTR(m_frameNameAttrPtr);

	// #10853
	I_MULTIREF(ibase::ICommandsProvider, m_commandsProvidersCompPtr);
	I_REF(ibase::ICommandsDisplayer, m_taskCommandsDisplayerPtr);
};


class CInspectionTaskGuiCompBase2 : public CInspectionTaskGuiCompBase
{
public:
	typedef CInspectionTaskGuiCompBase BaseClass;

	I_BEGIN_BASE_COMPONENT(CInspectionTaskGuiCompBase2);
		I_ASSIGN(m_licenseProviderCompPtr, "LicenseRightsProvider", "LicenseRightsProvider", false, "LicenseRightsProvider");
		I_ASSIGN_MULTI_0(m_licenseEnableIds, "LicenseEnableIds", "IDs to enable the buttons", false);

		I_ASSIGN(m_showControlPanelAttrPtr, "ShowControlPanel", "If enabled, the control button panel is shown", true, true);
		I_ASSIGN(m_hideRightPanelAttrPtr, "HideRightPanel", "Hide right button panel", false, false);
		I_ASSIGN(m_hideMessageListAttrPtr, "HideMessageList", "Hide bottom message list", false, false);
		I_ASSIGN(m_disableAutoTestAttrPtr, "DisableAutoTest", "Disable auto (continuous) test button on ui", true, false);
	I_END_COMPONENT;

protected:
	I_REF(iauth::IRightsProvider, m_licenseProviderCompPtr);
	I_MULTIATTR(QByteArray, m_licenseEnableIds);

	I_ATTR(bool, m_showControlPanelAttrPtr);
	I_ATTR(bool, m_hideRightPanelAttrPtr);
	I_ATTR(bool, m_hideMessageListAttrPtr);
	I_ATTR(bool, m_disableAutoTestAttrPtr);
};


class CInspectionTaskGuiComp:
	public iqtgui::TRestorableGuiWrap<
				TResultShapeCreatorWrap<CInspectionTaskGuiCompBase2>>,
	protected imod::CMultiModelDispatcherBase,
	virtual public ibase::ICommandsProvider,
	virtual public iview::IViewEventObserver
{
	Q_OBJECT

public:
	typedef iqtgui::TRestorableGuiWrap<
				TResultShapeCreatorWrap<CInspectionTaskGuiCompBase2> > BaseClass;

	enum DataRole
	{
		DR_TASK_INDEX = Qt::UserRole + 1,
		DR_SHAPE_INDICES,
		DR_SHAPE_POINTERS,
		DR_IS_RESULT,
		DR_CATEGORY
	};

	enum
	{
		MI_SUPPLIER_RESULTS_START_ID = 0,
		MI_VISUAL_STATUS_START_ID = 1000
	};

	enum CommandGroups
	{
		CG_TASK = 4800,
		CG_EDIT
	};

	I_BEGIN_COMPONENT(CInspectionTaskGuiComp);
		I_REGISTER_INTERFACE(ibase::ICommandsProvider);
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
		I_ASSIGN(m_generalParamsGuiCompPtr, "GeneralParamsGui", "GUI of general parameters", false, "GeneralParamsGui");
		I_ASSIGN_TO(m_generalParamsObserverCompPtr, m_generalParamsGuiCompPtr, false);
		I_ASSIGN_TO(m_generalParamsEditorCompPtr, m_generalParamsGuiCompPtr, false);
		I_ASSIGN(m_resultShapeFactoryCompPtr, "ResultShapeFactory", "Creates shapes to display on preview (if iqt2d::IViewProvider is used)", false, "ResultShapeFactory");
		I_ASSIGN(m_settingsKeyAttrPtr, "SettingsKey", "Key for saving/restoring of the layout information in the registry", true, "Settings");
		I_ASSIGN(m_currentTaskIndexOutputPtr, "CurrentIndexOutput", "The recently selected task index will be written here", false, "");
	I_END_COMPONENT;

	CInspectionTaskGuiComp();

	// selection API
	virtual bool SelectTask(int index);

	// reimplemented (ibase::ICommandsProvider)
	virtual const ibase::IHierarchicalCommand* GetCommands() const override;

	// reimplemented (imod::IObserver)
	virtual bool OnModelAttached(imod::IModel* modelPtr, istd::IChangeable::ChangeSet& changeMask) override;
	virtual bool OnModelDetached(imod::IModel* modelPtr) override;

protected:
	void UpdateProcessingState();
	void UpdateVisualElements();
	void UpdateParametersVisibility();

	// reimplemented (iqtgui::TRestorableGuiWrap)
	virtual void OnRestoreSettings(const QSettings& settings) override;
	virtual void OnSaveSettings(QSettings& settings) const override;

	// reimplemented (iqtgui::TGuiObserverWrap)
	virtual void UpdateModel() const override;
	virtual void UpdateGui(const istd::IChangeable::ChangeSet& changeSet) override;
	virtual void OnGuiModelAttached() override;
	virtual void OnGuiModelDetached() override;

	// reimplemented (iqtgui::CGuiComponentBase)
	virtual void OnGuiCreated() override;
	virtual void OnGuiDestroyed() override;
	virtual void OnGuiHidden() override;
	virtual void OnGuiRetranslate() override;
	virtual void OnGuiDesignChanged() override;

	// reimplemented (imod::CMultiModelDispatcherBase)
	virtual void OnModelChanged(int modelId, const istd::IChangeable::ChangeSet& changeSet) override;

	// reimplemented (QObject)
	virtual bool eventFilter(QObject* obj, QEvent* event) override;

	// reimplemented (iview::IViewEventObserver)
	virtual bool OnSelectChange(const iview::IShapeView& view, const istd::CIndex2d& position, const iview::IInteractiveShape& shape, bool state);
	virtual bool OnViewMouseButton(const iview::IShapeView& view, const istd::CIndex2d& position, Qt::MouseButton buttonType, bool state, const iview::IInteractiveShape* shapePtr);
	virtual bool OnViewMouseMove(const iview::IShapeView& view, const istd::CIndex2d& position) override;

Q_SIGNALS:
	void AfterGuiModelAttached();

protected Q_SLOTS:
	void OnEditorChanged(int index);
	void OnAutoTest();
	void OnTestAll();
	void OnAutoTestButtonClicked();
	void OnLoadParams();
	void OnSaveParams();
	void OnCopyAll();
	void OnPasteAll();
	void OnCopyCurrent();
	void OnPasteCurrent();
	void on_MessageList_itemSelectionChanged();
	void on_MessageList_itemDoubleClicked(QTreeWidgetItem* item, int column);

private:
	typedef QSet<int> ShapeIndices;

	void CreateMessagesFilter();
	void AddTaskMessagesToLog(const ilog::IMessageContainer& messageContainer, int taskIndex, bool isAuxiliary);
	void UpdateTaskMessages();
	void DoUpdateEditor(int taskIndex);
	void ActivateTaskShapes(int taskIndex, const ShapeIndices& shapeIndices, bool useAuxiliary);
	void CreateMenu();
	void UpdateMenu();
	bool CopyTaskParametersToClipboard(iser::ISerializable* objectPtr, const char* mimeType) const;
	bool ReadTaskParametersFromClipboard(iser::ISerializable* objectPtr, const char* mimeType);

	void DecorateButton(QWidget* buttonPtr, int category) const;

	/**
		Create a key for saving/restoring of the layout information over settings provider.
	*/
	QString GetSettingsKey() const;

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
	I_REF(iview::IShapeFactory, m_resultShapeFactoryCompPtr);
	I_ATTR(QByteArray, m_settingsKeyAttrPtr);
	I_REF(iprm::ISelectionParam, m_currentTaskIndexOutputPtr);

	int m_currentGuiIndex = -1;
	bool m_testStarted = false;
	int m_filterStatus = 0;

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

	QToolBox* m_toolBoxPtr = nullptr;
	QTabWidget* m_tabWidgetPtr = nullptr;
	QStackedWidget* m_stackedWidgetPtr = nullptr;
	QButtonGroup* m_buttonGroupPtr = nullptr;

	QAction* m_copyAllActionPtr = nullptr;
	QAction* m_pasteAllActionPtr = nullptr;
	QAction* m_copyCurrentTaskActionPtr = nullptr;
	QAction* m_pasteCurrentTaskActionPtr = nullptr;
	QAction* m_loadAllActionPtr = nullptr;
	QAction* m_saveAllActionPtr = nullptr;

	QTimer m_autoTestTimer;

	int m_lastPanelWidth = 0;
	bool m_darkMode = false;

	// commands
	iqtgui::CHierarchicalCommand m_rootCommands;
	iqtgui::CHierarchicalCommand m_commands;

	iqtgui::CHierarchicalCommand m_executeTaskCommand;
	iqtgui::CHierarchicalCommand m_continuousExecuteCommand;
};


} // namespace iqtinsp


#endif // !iqtinsp_CInspectionTaskGuiComp_included


