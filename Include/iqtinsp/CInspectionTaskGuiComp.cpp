#include <iqtinsp/CInspectionTaskGuiComp.h>


// Qt includes
#include<QtCore/QtGlobal>
#include <QtCore/QMimeData>
#include <QtGui/QClipboard>
#if QT_VERSION >= 0x050000
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QMenu>
#include <QtWidgets/QButtonGroup>
#else
#include <QtGui/QButtonGroup>
#include <QtGui/QHBoxLayout>
#include <QtGui/QVBoxLayout>
#include <QtGui/QSpacerItem>
#include <QtGui/QGroupBox>
#include <QtGui/QMenu>
#endif

// ACF includes
#include <istd/CChangeGroup.h>
#include <imod/IModel.h>
#include <imod/IObserver.h>
#include <iser/CCompactXmlMemReadArchive.h>
#include <iser/CCompactXmlMemWriteArchive.h>
#include <ilog/CExtMessage.h>
#include <iview/IShapeView.h>
#include <iview/IInteractiveShape.h>
#include <iview/CShapeBase.h>
#include <iview/CImageShape.h>


namespace iqtinsp
{


static const char InspectionTaskMimeType[] = "acf/iqtinsp::CInspectionTask";
static const char SupplierTaskMimeType[] = "acf/iinsp::ISupplier";


CInspectionTaskGuiComp::CInspectionTaskGuiComp()
:	m_currentGuiIndex(-1),
 	m_testStarted(false),
	m_toolBoxPtr(NULL),
	m_tabWidgetPtr(NULL),
	m_stackedWidgetPtr(NULL),
	m_buttonGroupPtr(NULL),
	m_copyAllActionPtr(NULL),
	m_pasteAllActionPtr(NULL),
	m_copyCurrentTaskActionPtr(NULL),
	m_pasteCurrentTaskActionPtr(NULL),
	m_loadAllActionPtr(NULL),
	m_saveAllActionPtr(NULL),
	m_rootCommands("", 100, ibase::ICommand::CF_GLOBAL_MENU),
	m_commands("Inspection", 100, ibase::ICommand::CF_GLOBAL_MENU),
	m_executeTaskCommand("Execute", 100, ibase::ICommand::CF_GLOBAL_MENU | ibase::ICommand::CF_TOOLBAR, CG_TASK),
	m_continuousExecuteCommand("Continuous", 100, ibase::ICommand::CF_GLOBAL_MENU | ibase::ICommand::CF_TOOLBAR | ibase::ICommand::CF_ONOFF, CG_TASK)
{
	connect(&m_executeTaskCommand, SIGNAL(triggered()), this, SLOT(OnAutoTest()));

	m_commands.InsertChild(&m_executeTaskCommand);
	m_commands.InsertChild(&m_continuousExecuteCommand);

	m_rootCommands.InsertChild(&m_commands);

	m_autoTestTimer.setSingleShot(true);
	m_autoTestTimer.setInterval(0);
	connect(&m_autoTestTimer, SIGNAL(timeout()), this, SLOT(OnAutoTest()));
}


// reimplemented (ibase::ICommandsProvider)

const ibase::IHierarchicalCommand* CInspectionTaskGuiComp::GetCommands() const
{
	return &m_rootCommands;
}


// reimplemented (imod::IObserver)

bool CInspectionTaskGuiComp::OnModelAttached(imod::IModel* modelPtr, istd::IChangeable::ChangeSet& changeMask)
{
	m_editorsList.clear();

	if (!BaseClass::OnModelAttached(modelPtr, changeMask)){
		return false;
	}

	iinsp::IInspectionTask* inspectionTaskPtr = GetObservedObject();
	Q_ASSERT(inspectionTaskPtr != NULL);

	int subtasksCount = inspectionTaskPtr->GetSubtasksCount();
	for (int i = 0; i < subtasksCount; ++i){
		iinsp::ISupplier* subTaskPtr = inspectionTaskPtr->GetSubtask(i);

		imod::IModel* supplierModelPtr = dynamic_cast<imod::IModel*>(subTaskPtr);
		if (supplierModelPtr == NULL){
			supplierModelPtr = CompCastPtr<imod::IModel>(subTaskPtr);
		}

		if (supplierModelPtr == NULL){
			continue;
		}

		if (i < m_editorObserversCompPtr.GetCount()){
			imod::IObserver* observerPtr = m_editorObserversCompPtr[i];

			if (observerPtr != NULL){
				if (supplierModelPtr->AttachObserver(observerPtr)){
					if (i < m_editorsCompPtr.GetCount()){
						imod::IModelEditor* editorPtr = m_editorsCompPtr[i];
						if (editorPtr != NULL){
							m_editorsList.insert(editorPtr);
						}
					}
				}
			}
		}

		if (i < m_previewObserversCompPtr.GetCount()){
			imod::IObserver* observerPtr = m_previewObserversCompPtr[i];

			if (observerPtr != NULL){
				supplierModelPtr->AttachObserver(observerPtr);
			}
		}

		RegisterModel(supplierModelPtr, MI_SUPPLIER_RESULTS_START_ID + i);
	}

	if (m_generalParamsObserverCompPtr.IsValid()){
		imod::IModel* generalParamsModelPtr = dynamic_cast<imod::IModel*>(inspectionTaskPtr->GetGeneralParameters());
		if (generalParamsModelPtr != NULL){
			generalParamsModelPtr->AttachObserver(m_generalParamsObserverCompPtr.GetPtr());
		}
	}

	return true;
}


bool CInspectionTaskGuiComp::OnModelDetached(imod::IModel* modelPtr)
{
	m_editorsList.clear();

	iinsp::IInspectionTask* inspectionTaskPtr = GetObservedObject();
	Q_ASSERT(inspectionTaskPtr != NULL);

	int subtasksCount = inspectionTaskPtr->GetSubtasksCount();
	for (int i = 0; i < subtasksCount; ++i){
		imod::IModel* parameterModelPtr = dynamic_cast<imod::IModel*>(inspectionTaskPtr->GetSubtask(i));
		if (parameterModelPtr == NULL){
			continue;
		}

		if (i < m_editorObserversCompPtr.GetCount()){
			imod::IObserver* observerPtr = m_editorObserversCompPtr[i];

			if ((observerPtr != NULL) && parameterModelPtr->IsAttached(observerPtr)){
				parameterModelPtr->DetachObserver(observerPtr);
			}
		}

		if (i < m_previewObserversCompPtr.GetCount()){
			imod::IObserver* observerPtr = m_previewObserversCompPtr[i];

			if ((observerPtr != NULL) && parameterModelPtr->IsAttached(observerPtr)){
				parameterModelPtr->DetachObserver(observerPtr);
			}
		}
	}

	if (m_generalParamsObserverCompPtr.IsValid()){
		imod::IModel* generalParamsModelPtr = dynamic_cast<imod::IModel*>(inspectionTaskPtr->GetGeneralParameters());
		if ((generalParamsModelPtr != NULL) && generalParamsModelPtr->IsAttached(m_generalParamsObserverCompPtr.GetPtr())){
			generalParamsModelPtr->DetachObserver(m_generalParamsObserverCompPtr.GetPtr());
		}
	}

	return BaseClass::OnModelDetached(modelPtr);
}


// protected methods

void CInspectionTaskGuiComp::UpdateProcessingState()
{
	int workStatus = iinsp::ISupplier::WS_INVALID;

	const iinsp::ISupplier* supplierPtr = dynamic_cast<const iinsp::ISupplier*>(GetObservedObject());
	if (supplierPtr != NULL){
		workStatus = supplierPtr->GetWorkStatus();
	}

	const istd::IInformationProvider* infoProviderPtr = dynamic_cast<const istd::IInformationProvider*>(supplierPtr);

	switch (workStatus){
		case iinsp::ISupplier::WS_OK:
			if (infoProviderPtr != NULL){
				switch (infoProviderPtr->GetInformationCategory()){
					case istd::IInformationProvider::IC_NONE:
						StateIconLabel->setPixmap(QPixmap(":/Icons/StateUnknown"));
						break;

					case istd::IInformationProvider::IC_WARNING:
						StateIconLabel->setPixmap(QPixmap(":/Icons/StateWarning"));
						break;

					case istd::IInformationProvider::IC_ERROR:
					case istd::IInformationProvider::IC_CRITICAL:
						StateIconLabel->setPixmap(QPixmap(":/Icons/StateInvalid"));
						break;

					default:
						StateIconLabel->setPixmap(QPixmap(":/Icons/StateOk"));
						break;
				}
			}
			else{
				StateIconLabel->setPixmap(QPixmap(":/Icons/StateOk"));
			}
			break;

		case iinsp::ISupplier::WS_FAILED:
			StateIconLabel->setPixmap(QPixmap(":/Icons/Error"));
			break;

		default:
			StateIconLabel->setPixmap(QPixmap(":/Icons/StateUnknown"));
			break;
	}
}


void CInspectionTaskGuiComp::UpdateVisualElements()
{
	if (!IsGuiCreated()){
		return;
	}

	int visualProvidersCount = m_editorVisualInfosCompPtr.GetCount();

	for (		GuiMap::ConstIterator iter = m_tabToGuiIndexMap.constBegin();
				iter != m_tabToGuiIndexMap.constEnd();
				++iter){
		int tabIndex = iter.key();
		Q_ASSERT(tabIndex >= 0);

		int guiIndex = iter.value();
		Q_ASSERT(guiIndex >= 0);

		if (guiIndex >= m_editorVisualInfosCompPtr.GetCount()){
			continue;
		}

		QIcon tabIcon;
		QString toolTip;

		if (guiIndex < visualProvidersCount){
			const iqtgui::IVisualStatus* visualProviderPtr = m_editorVisualInfosCompPtr[guiIndex];
			if (visualProviderPtr != NULL){
				tabIcon = visualProviderPtr->GetStatusIcon();
				toolTip = visualProviderPtr->GetStatusText();
			}
		}

		if (m_toolBoxPtr != NULL){
			Q_ASSERT(tabIndex < m_toolBoxPtr->count());
			m_toolBoxPtr->setItemIcon(tabIndex, tabIcon);
			m_toolBoxPtr->setItemToolTip(tabIndex, toolTip);
		}

		if (m_tabWidgetPtr != NULL){
			Q_ASSERT(tabIndex < m_tabWidgetPtr->count());
			m_tabWidgetPtr->setTabIcon(tabIndex, tabIcon);
			m_tabWidgetPtr->setTabToolTip(tabIndex, toolTip);
		}

		if (m_stackedWidgetPtr != NULL && m_buttonGroupPtr != NULL){
			Q_ASSERT(tabIndex < m_stackedWidgetPtr->count());
			QAbstractButton* buttonPtr = m_buttonGroupPtr->button(tabIndex);
			Q_ASSERT(buttonPtr != NULL);
			buttonPtr->setIcon(tabIcon);
			buttonPtr->setToolTip(toolTip);
		}
	}

	if (m_disableAutoTestAttrPtr.IsValid()){
		bool disable = m_disableAutoTestAttrPtr->GetValue();
		m_continuousExecuteCommand.SetEnabled(!disable);
		AutoTestButton->setEnabled(!disable);
		AutoTestButton->setVisible(!disable);
	}
}


// reimplemented (iqtgui::TRestorableGuiWrap)

void CInspectionTaskGuiComp::OnRestoreSettings(const QSettings& settings)
{
	Q_ASSERT(IsGuiCreated());
	Q_ASSERT(PreviewSplitter != NULL);

	// preserve overriding of splitter orientation:
	Qt::Orientation splitterOrientation = PreviewSplitter->orientation();

	QByteArray splitterState = settings.value(GetSettingsKey()).toByteArray();

	PreviewSplitter->restoreState(splitterState);

	PreviewSplitter->setOrientation(splitterOrientation);
}


void CInspectionTaskGuiComp::OnSaveSettings(QSettings& settings) const
{
	Q_ASSERT(IsGuiCreated());
	Q_ASSERT(PreviewSplitter != NULL);

	QByteArray splitterState = PreviewSplitter->saveState();

	settings.setValue(GetSettingsKey(), splitterState);
}


// reimplemented (iqtgui::TGuiObserverWrap)

void CInspectionTaskGuiComp::UpdateModel() const
{
	Q_ASSERT(IsGuiCreated() && (GetObservedObject() != NULL));

	for (		EditorsList::const_iterator iter = m_editorsList.constBegin();
				iter != m_editorsList.constEnd();
				++iter){
		const imod::IModelEditor* editorPtr = *iter;
		Q_ASSERT(editorPtr != NULL);
		
		editorPtr->UpdateModelFromEditor();
	}
}


void CInspectionTaskGuiComp::UpdateGui(const istd::IChangeable::ChangeSet& /*changeSet*/)
{
	Q_ASSERT(IsGuiCreated());

	UpdateMenu();

	UpdateVisualElements();

	UpdateProcessingState();

	UpdateTaskMessages();

	DoUpdateEditor(m_currentGuiIndex);

	if (AutoTestButton->isChecked()){
		m_autoTestTimer.start();
	}
}


// reimplemented (iqtgui::CGuiComponentBase)

void CInspectionTaskGuiComp::OnGuiCreated()
{
	QLayout* layoutPtr = ParamsFrame->layout();
	if (layoutPtr == NULL){
		if (*m_useHorizontalLayoutAttrPtr){
			layoutPtr = new QHBoxLayout(ParamsFrame);
		}
		else{
			layoutPtr = new QVBoxLayout(ParamsFrame);
		}
	}

	layoutPtr->setMargin(0);

	ControlFrame->setVisible(*m_showControlPanelAttrPtr);

	ParamsFrame->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::MinimumExpanding);

	bool useSpacer = *m_useVerticalSpacerAttrPtr;

	switch (*m_designTypeAttrPtr){
		case 1: // toolbox
		{
			m_toolBoxPtr = new QToolBox(ParamsFrame);
			m_toolBoxPtr->setBackgroundRole(QPalette::Window);
			m_toolBoxPtr->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Preferred);

			int subtasksCount = m_editorGuisCompPtr.GetCount();
			for (int i = 0; i < subtasksCount; ++i){
				iqtgui::IGuiObject* guiPtr = m_editorGuisCompPtr[i];

				if (guiPtr != NULL){
					QWidget* panelPtr = new QWidget(m_toolBoxPtr);
					QLayout* panelLayoutPtr = new QVBoxLayout(panelPtr);
					panelLayoutPtr->setContentsMargins(6, 0, 6, 0);
					QString name;
					if (i < m_namesAttrPtr.GetCount()){
						name = m_namesAttrPtr[i];
					}

					guiPtr->CreateGui(panelPtr);

					int toolBoxIndex = m_toolBoxPtr->addItem(panelPtr, name);

					if (useSpacer){
						QSpacerItem* spacerPtr = new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding);

						panelLayoutPtr->addItem(spacerPtr);
					}

					m_tabToGuiIndexMap[toolBoxIndex] = i;

					if (i < m_editorVisualModelsCompPtr.GetCount()){
						imod::IModel* modelPtr = m_editorVisualModelsCompPtr[i];
						if (modelPtr != NULL){
							RegisterModel(modelPtr, MI_VISUAL_STATUS_START_ID + i);
						}
					}
				}
			}

			QObject::connect(m_toolBoxPtr, SIGNAL(currentChanged(int)), this, SLOT(OnEditorChanged(int)));

			layoutPtr->addWidget(m_toolBoxPtr);
		}
		break;

		default: // tabs
		{
			m_tabWidgetPtr = new QTabWidget(ParamsFrame);
			m_tabWidgetPtr->setTabPosition(QTabWidget::TabPosition(*m_tabOrientationAttrPtr));

			int subtasksCount = m_editorGuisCompPtr.GetCount();
			for (int i = 0; i < subtasksCount; ++i){
				iqtgui::IGuiObject* guiPtr = m_editorGuisCompPtr[i];

				if (guiPtr != NULL){
					QWidget* panelPtr = new QWidget(m_tabWidgetPtr);
					QLayout* panelLayoutPtr = new QVBoxLayout(panelPtr);
					QString name;
					if (i < m_namesAttrPtr.GetCount()){
						name = m_namesAttrPtr[i];
					}

					guiPtr->CreateGui(panelPtr);

					int tabIndex = m_tabWidgetPtr->addTab(panelPtr, name);

					if (useSpacer){
						QSpacerItem* spacerPtr = new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding);

						panelLayoutPtr->addItem(spacerPtr);
					}

					m_tabToGuiIndexMap[tabIndex] = i;

					if (i < m_editorVisualModelsCompPtr.GetCount()){
						imod::IModel* modelPtr = m_editorVisualModelsCompPtr[i];
						if (modelPtr != NULL){
							RegisterModel(modelPtr, MI_VISUAL_STATUS_START_ID + i);
						}
					}
				}
			}

			QObject::connect(m_tabWidgetPtr, SIGNAL(currentChanged(int)), this, SLOT(OnEditorChanged(int)));

			layoutPtr->addWidget(m_tabWidgetPtr);
		}
		break;
	}

	QMap<iqtgui::IGuiObject*, int> guiToStackIndexMap;

	int previewGuisCount = m_previewGuisCompPtr.GetCount();
	for (int previewIndex = 0; previewIndex < previewGuisCount; ++previewIndex){
		iqtgui::IGuiObject* guiObjectPtr = m_previewGuisCompPtr[previewIndex];
		if (guiObjectPtr == NULL){
			continue;
		}

		int stackIndex;
		QMap<iqtgui::IGuiObject*, int>::ConstIterator stackRepeatIter = guiToStackIndexMap.constFind(guiObjectPtr);
		if (stackRepeatIter == guiToStackIndexMap.constEnd()){
			istd::TDelPtr<QWidget> newPreviewPagePtr(new QWidget());
			if (!newPreviewPagePtr.IsValid()){
				continue;
			}

			QVBoxLayout* pageLayoutPtr = new QVBoxLayout(newPreviewPagePtr.GetPtr());
			pageLayoutPtr->setContentsMargins(0, 0, 0, 0);

			if (!guiObjectPtr->CreateGui(newPreviewPagePtr.GetPtr())){
				continue;
			}

			stackIndex = PreviewStack->addWidget(newPreviewPagePtr.PopPtr());
			m_stackIndexToTabMap[stackIndex] = previewIndex;

			guiToStackIndexMap[guiObjectPtr] = stackIndex;
		}
		else{
			stackIndex = stackRepeatIter.value();
		}

		m_tabToStackIndexMap[previewIndex] = stackIndex;
	}

	if (m_generalParamsGuiCompPtr.IsValid()){
		m_generalParamsGuiCompPtr->CreateGui(GeneralParamsFrame);
	}
	else{
		GeneralParamsFrame->hide();
	}

	CreateMenu();
	MenuButton->hide();

	UpdateTaskMessages();

	OnEditorChanged(0);

	UpdateVisualElements();

	BaseClass::OnGuiCreated();

	connect(&m_continuousExecuteCommand, SIGNAL(toggled(bool)), AutoTestButton, SLOT(setChecked(bool)));
}


void CInspectionTaskGuiComp::OnGuiDestroyed()
{
	m_editorsList.clear();

	int subtasksCount = m_editorGuisCompPtr.GetCount();
	for (int i = 0; i < subtasksCount; ++i){
		iqtgui::IGuiObject* guiPtr = m_editorGuisCompPtr[i];

		if ((guiPtr != NULL) && guiPtr->IsGuiCreated()){
			guiPtr->DestroyGui();
		}
	}

	for (		GuiMap::const_iterator iter = m_stackIndexToTabMap.constBegin();
				iter != m_stackIndexToTabMap.constEnd();
				++iter){
		int guiIndex = iter.value();
		Q_ASSERT(guiIndex >= 0);
		Q_ASSERT(guiIndex < m_previewGuisCompPtr.GetCount());

		iqtgui::IGuiObject* guiPtr = m_previewGuisCompPtr[guiIndex];
		Q_ASSERT(guiPtr != NULL);

		guiPtr->DestroyGui();
	}

	if (m_generalParamsGuiCompPtr.IsValid() && m_generalParamsGuiCompPtr->IsGuiCreated()){
		m_generalParamsGuiCompPtr->DestroyGui();
	}

	if (m_toolBoxPtr != NULL){
		delete m_toolBoxPtr;

		m_toolBoxPtr = NULL;
	}

	if (m_tabWidgetPtr != NULL){
		delete m_tabWidgetPtr;

		m_tabWidgetPtr = NULL;
	}

	UnregisterAllModels();

	m_tabToStackIndexMap.clear();
	m_stackIndexToTabMap.clear();
	m_tabToGuiIndexMap.clear();

	BaseClass::OnGuiDestroyed();
}


void CInspectionTaskGuiComp::OnGuiHidden()
{
	AutoTestButton->setChecked(false);

	BaseClass::OnGuiHidden();
}


void CInspectionTaskGuiComp::OnGuiRetranslate()
{
	BaseClass::OnGuiRetranslate();

	m_executeTaskCommand.SetVisuals(tr("Execute"), tr("Execute"), tr("Execute supplier chain"), QIcon(":/Icons/Play"));
	m_continuousExecuteCommand.SetVisuals(tr("Continuous"), tr("Continuous"), tr("Enable continuous execution of the supplier chain"), QIcon(":/Icons/AutoUpdate"));

	int subtasksCount = m_editorGuisCompPtr.GetCount();

	for (int i = 0; i < subtasksCount; ++i){
		iqtgui::IGuiObject* guiPtr = m_editorGuisCompPtr[i];
		if (guiPtr != NULL){
			QString name;
			if (i < m_namesAttrPtr.GetCount()){
				name = m_namesAttrPtr[i];

				switch (*m_designTypeAttrPtr){
				case 1: // toolbox
					if (m_toolBoxPtr != NULL){
						m_toolBoxPtr->setItemText(i, name);
					}
					break;

				case 2: // tab
					if (m_tabWidgetPtr != NULL){
						m_tabWidgetPtr->setTabText(i, name);
					}
					break;

				default:
					break;
				}
			}
		}
	}

	UpdateVisualElements();
}


// reimplemented (imod::CMultiModelDispatcherBase)

void CInspectionTaskGuiComp::OnModelChanged(int modelId, const istd::IChangeable::ChangeSet& changeSet)
{
	if (!m_testStarted){
		int visualStatusModelId = modelId - MI_VISUAL_STATUS_START_ID;
		if (visualStatusModelId == m_currentGuiIndex){
			UpdateVisualElements();
		}

		int supplierResultsModelId = modelId - MI_SUPPLIER_RESULTS_START_ID;
		if (supplierResultsModelId == m_currentGuiIndex){
			if (changeSet.ContainsExplicit(iinsp::ISupplier::CF_SUPPLIER_RESULTS)){
				UpdateProcessingState();

				UpdateTaskMessages();

				DoUpdateEditor(m_currentGuiIndex);
			}
		}
	}
}


// protected slots

void CInspectionTaskGuiComp::OnEditorChanged(int index)
{
	if (index != m_currentGuiIndex){
		DoUpdateEditor(index);

		m_currentGuiIndex = index;
		int stackIndex = m_tabToStackIndexMap[index];
		PreviewStack->setCurrentIndex(stackIndex);

		if (AutoTestButton->isChecked()){
			m_autoTestTimer.start();
		}
	}
}


void CInspectionTaskGuiComp::OnAutoTest()
{
	if (!IsGuiCreated()){
		return;
	}

	if (m_testStarted) {
		return;
	}

	m_testStarted = true;

	MessageList->clear();
	m_resultShapesMap.clear();
	m_tempShapesMap.clear();

	iinsp::ISupplier* supplierPtr = dynamic_cast<iinsp::ISupplier*>(GetObservedObject());
	if (supplierPtr != NULL){
		istd::CChangeGroup changeGroup(supplierPtr);

		supplierPtr->InvalidateSupplier();
		supplierPtr->EnsureWorkInitialized();
		supplierPtr->EnsureWorkFinished();
	}

	m_testStarted = false;

	if (AutoTestButton->isChecked()) {
		m_autoTestTimer.start();
	}
}


void CInspectionTaskGuiComp::on_TestAllButton_clicked()
{
	if (m_generalParamsEditorCompPtr.IsValid()){
		m_generalParamsEditorCompPtr->UpdateModelFromEditor();
	}

	OnAutoTest();
}


void CInspectionTaskGuiComp::on_AutoTestButton_clicked()
{
	if (AutoTestButton->isChecked()){
		OnAutoTest();
	}
}


void CInspectionTaskGuiComp::OnLoadParams()
{
	iinsp::IInspectionTask* objectPtr = GetObservedObject();
	if (objectPtr != NULL){
		m_paramsLoaderCompPtr->LoadFromFile(*objectPtr);
	}
}


void CInspectionTaskGuiComp::OnSaveParams()
{
	iinsp::IInspectionTask* objectPtr = GetObservedObject();
	if (objectPtr != NULL){
		m_paramsLoaderCompPtr->SaveToFile(*objectPtr);
	}
}


void CInspectionTaskGuiComp::OnCopyAll()
{
	iinsp::IInspectionTask* objectPtr = GetObservedObject();
	if (objectPtr != NULL){
		CopyTaskParametersToClipboard(objectPtr, InspectionTaskMimeType);
	}

	UpdateMenu();
}


void CInspectionTaskGuiComp::OnPasteAll()
{
	iinsp::IInspectionTask* objectPtr = GetObservedObject();
	if (objectPtr != NULL){
		ReadTaskParametersFromClipboard(objectPtr, InspectionTaskMimeType);
	}
}


void CInspectionTaskGuiComp::OnCopyCurrent()
{
	iinsp::IInspectionTask* objectPtr = GetObservedObject();
	if (objectPtr != NULL){
		iinsp::ISupplier* supplierPtr = objectPtr->GetSubtask(m_currentGuiIndex);
		if (supplierPtr != NULL){
			iprm::IParamsSet* paramsPtr = supplierPtr->GetModelParametersSet();
			if (paramsPtr != NULL){
				CopyTaskParametersToClipboard(paramsPtr, SupplierTaskMimeType);
			}
		}
	}

	UpdateMenu();
}


void CInspectionTaskGuiComp::OnPasteCurrent()
{
	iinsp::IInspectionTask* objectPtr = GetObservedObject();
	if (objectPtr != NULL){
		iinsp::ISupplier* supplierPtr = objectPtr->GetSubtask(m_currentGuiIndex);
		if (supplierPtr != NULL){
			iprm::IParamsSet* paramsPtr = supplierPtr->GetModelParametersSet();
			if (paramsPtr != NULL){
				ReadTaskParametersFromClipboard(paramsPtr, SupplierTaskMimeType);
			}
		}
	}
}


void CInspectionTaskGuiComp::on_MessageList_itemSelectionChanged()
{
	QList<QTreeWidgetItem*> selectedItems = MessageList->selectedItems();
	if (selectedItems.empty()){
		return;
	}

	QTreeWidgetItem* itemPtr = selectedItems.front();
	Q_ASSERT(itemPtr != NULL);

	int taskIndex = itemPtr->data(0, DR_TASK_INDEX).toInt();
	QList<QVariant> indices = itemPtr->data(0, DR_SHAPE_INDICES).toList();
	bool useAuxiliary = !itemPtr->data(0, DR_IS_RESULT).toBool();

	ShapeIndices shapeIndices;
	for (QList<QVariant>::ConstIterator iter = indices.constBegin(); iter != indices.constEnd(); ++iter){
		shapeIndices += iter->toInt();
	}

	ActivateTaskShapes(taskIndex, shapeIndices, useAuxiliary);
}


void CInspectionTaskGuiComp::on_MessageList_itemDoubleClicked(QTreeWidgetItem* item, int /*column*/)
{
	Q_ASSERT(item != NULL);

	int taskIndex = item->data(0, DR_TASK_INDEX).toInt();

	for (		GuiMap::ConstIterator tabsIter = m_tabToGuiIndexMap.constBegin();
				tabsIter != m_tabToGuiIndexMap.constEnd();
				++tabsIter){
		int tabIndex = tabsIter.key();
		if (tabsIter.value() == taskIndex){
			if (m_toolBoxPtr != NULL){
				m_toolBoxPtr->setCurrentIndex(tabIndex);
			}

			if (m_tabWidgetPtr != NULL){
				m_tabWidgetPtr->setCurrentIndex(tabIndex);
			}

			return;
		}
	}
}


// private methods

void CInspectionTaskGuiComp::AddTaskMessagesToLog(const ilog::IMessageContainer& messageContainer, int taskIndex, bool isAuxiliary)
{
	ilog::IMessageContainer::Messages messagesList = messageContainer.GetMessages();

	int messagesCount = messagesList.count();
	if (messagesCount == 0){
		return;
	}

	QString tabName;
	if (taskIndex < m_namesAttrPtr.GetCount()){
		tabName = m_namesAttrPtr[taskIndex];
	}

	istd::TPointerVector<iview::IShape>& resultShapes = isAuxiliary? m_tempShapesMap[taskIndex]: m_resultShapesMap[taskIndex];

	// find view associated with this task
	iview::IShapeView* viewPtr = NULL;
	if (taskIndex < m_previewSceneProvidersCompPtr.GetCount()){
		const iqt2d::IViewProvider* viewProviderPtr = m_previewSceneProvidersCompPtr[taskIndex];
		if (viewProviderPtr != NULL){
			viewPtr = viewProviderPtr->GetView();
		}
	}

	QTreeWidgetItem* auxiliaryItemPtr = NULL;

	for (int messageIndex = 0; messageIndex < messagesCount; messageIndex++){
		ilog::IMessageConsumer::MessagePtr messagePtr = messagesList[messageIndex];

		QList<QVariant> shapeIndices;

		// add result shapes to view and internal shape list
		if (viewPtr != NULL){
			const ilog::CExtMessage* extMessagePtr = dynamic_cast<const ilog::CExtMessage*>(messagePtr.GetPtr());
			if (extMessagePtr != NULL){
				int objectsCount = extMessagePtr->GetAttachedObjectsCount();
				for (int i = 0; i < objectsCount; ++i){
					const iser::IObject* attachedObjectPtr = extMessagePtr->GetAttachedObject(i);
					iview::IShape* shapePtr = CreateResultShape(m_resultShapeFactoryCompPtr.GetPtr(), attachedObjectPtr, extMessagePtr, true);
					if (shapePtr != NULL){
						shapePtr->SetVisible(false);

						const QString& objectDescription = extMessagePtr->GetAttachedObjectDescription(i);
						if (!objectDescription.isEmpty()){
							shapePtr->SetDefaultDescription(objectDescription);
						}
						else{
							shapePtr->SetDefaultDescription(messagePtr->GetInformationDescription());
						}

						shapeIndices += QVariant(resultShapes.GetCount());
						resultShapes.PushBack(shapePtr);

						viewPtr->ConnectShape(shapePtr);
					}
				}
			}
			else{
				const i2d::IObject2d* object2dPtr = dynamic_cast<const i2d::IObject2d*>(messagePtr.GetPtr());
				if (object2dPtr != NULL){
					iview::IShape* shapePtr = CreateResultShape(m_resultShapeFactoryCompPtr.GetPtr(), object2dPtr, messagePtr.GetPtr(), true);
					if (shapePtr != NULL){
						shapePtr->SetVisible(false);
						shapePtr->SetDefaultDescription(messagePtr->GetInformationDescription());

						shapeIndices += QVariant(resultShapes.GetCount());
						resultShapes.PushBack(shapePtr);

						viewPtr->ConnectShape(shapePtr);
					}
				}
			}
		}

		QTreeWidgetItem* messageItemPtr = new QTreeWidgetItem;

		messageItemPtr->setData(0, DR_TASK_INDEX, taskIndex);
		messageItemPtr->setData(0, DR_SHAPE_INDICES, shapeIndices);

		QIcon messageIcon = GetCategoryIcon(messagePtr->GetInformationCategory()).pixmap(QSize(12, 12), QIcon::Normal, QIcon::On);
		messageItemPtr->setIcon(0, messageIcon);

		QString sourceName = messagePtr->GetInformationSource();
		if (sourceName.isEmpty()){
			sourceName = tabName;
		}

		messageItemPtr->setText(0, sourceName);
		messageItemPtr->setText(1, messagePtr->GetInformationDescription());

		if (isAuxiliary){
			if (auxiliaryItemPtr == NULL){
				auxiliaryItemPtr = new QTreeWidgetItem;

				QList<QVariant> auxGroupIndices;
				auxGroupIndices << -2;
				auxiliaryItemPtr->setData(0, DR_TASK_INDEX, taskIndex);
				auxiliaryItemPtr->setData(0, DR_SHAPE_INDICES, auxGroupIndices);

				auxiliaryItemPtr->setText(0, tabName);
				auxiliaryItemPtr->setText(1, tr("Auxiliary Output"));

				MessageList->addTopLevelItem(auxiliaryItemPtr);
			}

			auxiliaryItemPtr->addChild(messageItemPtr);
		}
		else{
			messageItemPtr->setData(0, DR_IS_RESULT, true);

			MessageList->addTopLevelItem(messageItemPtr);
		}
	}
}


void CInspectionTaskGuiComp::UpdateTaskMessages()
{
	if (!IsGuiCreated()){
		return;
	}

	MessageList->clear();
	m_resultShapesMap.clear();

	iinsp::IInspectionTask* taskPtr = GetObservedObject();

	if (taskPtr != NULL){
		int subtasksCount = taskPtr->GetSubtasksCount();
		for (int subTaskIndex = 0; subTaskIndex < subtasksCount; subTaskIndex++){
			iinsp::ISupplier* subTaskPtr = taskPtr->GetSubtask(subTaskIndex);
			if (subTaskPtr != NULL){
				const ilog::IMessageContainer* messageContainerPtr = subTaskPtr->GetWorkMessages(iinsp::ISupplier::MCT_RESULTS);
				if (messageContainerPtr != NULL){
					m_resultMessagesMap[subTaskIndex].CopyFrom(*messageContainerPtr, istd::IChangeable::CM_CONVERT);

					AddTaskMessagesToLog(m_resultMessagesMap[subTaskIndex], subTaskIndex, false);
				}

				messageContainerPtr = subTaskPtr->GetWorkMessages(iinsp::ISupplier::MCT_TEMP);
				if (messageContainerPtr != NULL){
					m_tempMessagesMap[subTaskIndex].CopyFrom(*messageContainerPtr, istd::IChangeable::CM_CONVERT);

					AddTaskMessagesToLog(m_tempMessagesMap[subTaskIndex], subTaskIndex, true);
				}
			}
		}
	}

	// Close message view if no messages were provided:
	QList<int> sizes = MessageListSplitter->sizes();

	if (MessageList->topLevelItemCount() == 0){
		sizes[1] = 0;
	}
	else{
		sizes[1] = sizes[0] / 4;
	}

	MessageListSplitter->setSizes(sizes);
}


void CInspectionTaskGuiComp::DoUpdateEditor(int taskIndex)
{
	int extendersCount = m_editorViewExtendersCompPtr.GetCount();
	int previewProvidersCount = m_previewSceneProvidersCompPtr.GetCount();

	iview::IShapeView* viewPtr = NULL;

	if ((m_currentGuiIndex >= 0) && (m_currentGuiIndex < extendersCount) && (m_currentGuiIndex < previewProvidersCount)){
		iqt2d::IViewExtender* extenderPtr = m_editorViewExtendersCompPtr[m_currentGuiIndex];
		iqt2d::IViewProvider* previewProviderPtr = m_previewSceneProvidersCompPtr[m_currentGuiIndex];
		if ((extenderPtr != NULL) && (previewProviderPtr != NULL)){
			extenderPtr->RemoveItemsFromScene(previewProviderPtr);

			viewPtr = previewProviderPtr->GetView();
		}
	}

	if (taskIndex >= 0){
		// add shapes from editor to view
		if ((taskIndex < extendersCount) && (taskIndex < previewProvidersCount)){
			iqt2d::IViewExtender* extenderPtr = m_editorViewExtendersCompPtr[taskIndex];
			iqt2d::IViewProvider* previewProviderPtr = m_previewSceneProvidersCompPtr[taskIndex];
			if ((extenderPtr != NULL) && (previewProviderPtr != NULL)){
				extenderPtr->AddItemsToScene(previewProviderPtr, iqt2d::IViewExtender::SF_DIRECT);

				viewPtr = previewProviderPtr->GetView();
			}
		}

		int editorsCount = m_editorsCompPtr.GetCount();
		if (taskIndex < editorsCount){
			imod::IModelEditor* editorPtr = m_editorsCompPtr[taskIndex];
			if (editorPtr != NULL){
				editorPtr->UpdateEditor(istd::IChangeable::GetAnyChange());
			}
		}
	}

	// Activate task related shapes:
	ActivateTaskShapes(taskIndex, ShapeIndices(), false);

	if (viewPtr != NULL){
		viewPtr->Update();
	}
}


void CInspectionTaskGuiComp::ActivateTaskShapes(int taskIndex, const ShapeIndices& shapeIndices, bool useAuxiliary)
{
	if (!IsGuiCreated()){
		return;
	}

	bool showAllAuxiliary = shapeIndices.contains(-2);
	bool showResults = showAllAuxiliary || !useAuxiliary;

	for (		ResultShapesMap::ConstIterator shapesContainerIter = m_resultShapesMap.constBegin();
				shapesContainerIter != m_resultShapesMap.constEnd();
				++shapesContainerIter){
		int shapeTaskIndex = shapesContainerIter.key();
		const istd::TPointerVector<iview::IShape>& resultShapes = shapesContainerIter.value();

		int shapesCount = resultShapes.GetCount();
		for (int i = 0; i < shapesCount; ++i){
			iview::IShape* shapePtr = resultShapes.GetAt(i);
			Q_ASSERT(shapePtr != NULL);	// only correct instances should be added to container

			shapePtr->SetVisible(showResults && (taskIndex == shapeTaskIndex));

			iview::IInteractiveShape* interactiveShapePtr = dynamic_cast<iview::IInteractiveShape*>(shapePtr);
			if (interactiveShapePtr != NULL){
				interactiveShapePtr->SetSelected(shapeIndices.contains(i));
			}
		}
	}

	for (		ResultShapesMap::ConstIterator shapesContainerIter = m_tempShapesMap.constBegin();
				shapesContainerIter != m_tempShapesMap.constEnd();
				++shapesContainerIter){
		int shapeTaskIndex = shapesContainerIter.key();
		const istd::TPointerVector<iview::IShape>& resultShapes = shapesContainerIter.value();

		int shapesCount = resultShapes.GetCount();
		for (int i = 0; i < shapesCount; ++i){
			iview::IShape* shapePtr = resultShapes.GetAt(i);
			Q_ASSERT(shapePtr != NULL);	// only correct instances should be added to container

			shapePtr->SetVisible((taskIndex == shapeTaskIndex) && (showAllAuxiliary || (useAuxiliary && (shapeIndices.contains(i)))));
		}
	}

	if ((taskIndex >= 0) && (taskIndex  < m_previewSceneProvidersCompPtr.GetCount())){
		iqt2d::IViewProvider* previewProviderPtr = m_previewSceneProvidersCompPtr[taskIndex];
		if (previewProviderPtr != NULL){
			iview::IShapeView* viewPtr = previewProviderPtr->GetView();
			if (viewPtr != NULL){
				viewPtr->Update();
			}
		}
	}
}


void CInspectionTaskGuiComp::CreateMenu()
{
	/*
	QMenu* actionsMenuPtr = new QMenu(MenuButton);
	MenuButton->setMenu(actionsMenuPtr);

	m_copyAllActionPtr = actionsMenuPtr->addAction(QIcon(":/Icons/Copy"), tr("Copy all"), this, SLOT(OnCopyAll()));
	m_pasteAllActionPtr = actionsMenuPtr->addAction(QIcon(":/Icons/Paste"), tr("Paste all"), this, SLOT(OnPasteAll()));
	actionsMenuPtr->addSeparator();
	m_copyCurrentTaskActionPtr = actionsMenuPtr->addAction(QIcon(":/Icons/Copy"), tr("Copy current task"), this, SLOT(OnCopyCurrent()));
	m_pasteCurrentTaskActionPtr = actionsMenuPtr->addAction(QIcon(":/Icons/Paste"), tr("Paste current task"), this, SLOT(OnPasteCurrent()));

	if (m_paramsLoaderCompPtr.IsValid()){
		actionsMenuPtr->addSeparator();
		m_loadAllActionPtr = actionsMenuPtr->addAction(QIcon(":/Icons/Open"), tr("Load..."), this, SLOT(OnLoadParams()));
		m_saveAllActionPtr = actionsMenuPtr->addAction(QIcon(":/Icons/Save"), tr("Save..."), this, SLOT(OnSaveParams()));
	}

	UpdateMenu();
	*/
}


void CInspectionTaskGuiComp::UpdateMenu()
{
	/*
	Q_ASSERT(m_pasteCurrentTaskActionPtr != NULL);
	Q_ASSERT(m_pasteAllActionPtr != NULL);

	QClipboard* clipboardPtr = QApplication::clipboard();
	const QMimeData* mimeDataPtr = clipboardPtr->mimeData();
	if (mimeDataPtr != NULL){
		m_pasteCurrentTaskActionPtr->setEnabled(mimeDataPtr->hasFormat(SupplierTaskMimeType) || mimeDataPtr->hasText());
		m_pasteAllActionPtr->setEnabled(mimeDataPtr->hasFormat(InspectionTaskMimeType) || mimeDataPtr->hasText());
	}
	else{
		m_pasteCurrentTaskActionPtr->setEnabled(false);
		m_pasteAllActionPtr->setEnabled(false);
	}
	*/
}


bool CInspectionTaskGuiComp::CopyTaskParametersToClipboard(iser::ISerializable* objectPtr, const char* mimeType) const
{
	QClipboard* clipboardPtr = QApplication::clipboard();
	Q_ASSERT(clipboardPtr != NULL);
	Q_ASSERT(objectPtr != NULL);

	iser::CCompactXmlMemWriteArchive archive;
	if (objectPtr->Serialize(archive)){
		QMimeData* mimeDataPtr = new QMimeData;
		mimeDataPtr->setData(mimeType, archive.GetString());
		clipboardPtr->setMimeData(mimeDataPtr);
		clipboardPtr->setText(archive.GetString());

		return true;
	}

	return false;
}


bool CInspectionTaskGuiComp::ReadTaskParametersFromClipboard(iser::ISerializable* objectPtr, const char* mimeType)
{
	const QClipboard* clipboardPtr = QApplication::clipboard();
	Q_ASSERT(clipboardPtr != NULL);
	Q_ASSERT(objectPtr != NULL);

	// try via mime data
	const QMimeData* mimeDataPtr = clipboardPtr->mimeData();
	if (mimeDataPtr != NULL && mimeType != NULL){
		if (mimeDataPtr->hasFormat(mimeType)){
			iser::CCompactXmlMemReadArchive archive(mimeDataPtr->data(mimeType));
			if (objectPtr->Serialize(archive)){
				UpdateGui(istd::IChangeable::GetAllChanges());

				return true;
			}
		}
	}	

	// else try via plain text
	QByteArray dataToPaste(clipboardPtr->text().toUtf8());
	iser::CCompactXmlMemReadArchive archive(dataToPaste);
	if (objectPtr->Serialize(archive)){
		UpdateGui(istd::IChangeable::GetAllChanges());

		return true;
	}

	return false;
}


QString CInspectionTaskGuiComp::GetSettingsKey() const
{
	QString settingsKey = "Splitter";
	if (m_settingsKeyAttrPtr.IsValid()){
		settingsKey = *m_settingsKeyAttrPtr + QString("/") + settingsKey;
	}

	return settingsKey;
}


// private static methods

QIcon CInspectionTaskGuiComp::GetCategoryIcon(istd::IInformationProvider::InformationCategory category)
{
	static QIcon logIcon(":/Icons/Log");
	static QIcon infoIcon(":/Icons/Info");
	static QIcon warningIcon(":/Icons/Warning");
	static QIcon errorIcon(":/Icons/Error");

	switch (category){
	case istd::IInformationProvider::IC_INFO:
		return infoIcon;

	case istd::IInformationProvider::IC_WARNING:
		return warningIcon;

	case istd::IInformationProvider::IC_ERROR:
	case istd::IInformationProvider::IC_CRITICAL:
		return errorIcon;

	default:
		return logIcon;
	}
}


} // namespace iqtinsp


