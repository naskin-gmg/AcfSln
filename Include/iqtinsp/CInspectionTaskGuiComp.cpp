#include "iqtinsp/CInspectionTaskGuiComp.h"


// Qt includes
#include <QtGui/QHBoxLayout>
#include <QtGui/QVBoxLayout>
#include <QtGui/QSpacerItem>
#include <QtGui/QGroupBox>

// ACF includes
#include "imod/IModel.h"
#include "imod/IObserver.h"
#include "iview/IShapeView.h"
#include "iview/IInteractiveShape.h"
#include "iview/CShapeBase.h"


namespace iqtinsp
{


CInspectionTaskGuiComp::CInspectionTaskGuiComp()
:	m_currentGuiIndex(-1),
	m_toolBoxPtr(NULL),
	m_tabWidgetPtr(NULL)
{
	connect(this, SIGNAL(DoAutoTest()), SLOT(OnAutoTest()), Qt::QueuedConnection);
}


// reimplemented (imod::IModelEditor)

void CInspectionTaskGuiComp::UpdateModel() const
{
	Q_ASSERT(IsGuiCreated() && (GetObjectPtr() != NULL));

	for (		EditorsList::const_iterator iter = m_editorsList.constBegin();
				iter != m_editorsList.constEnd();
				++iter){
		const imod::IModelEditor* editorPtr = *iter;
		Q_ASSERT(editorPtr != NULL);
		
		editorPtr->UpdateModel();
	}
}


void CInspectionTaskGuiComp::UpdateEditor(int /*updateFlags*/)
{
	Q_ASSERT(IsGuiCreated());

	if (AutoTestButton->isChecked()){
		emit DoAutoTest();
	}
}


// reimplemented (imod::IObserver)

bool CInspectionTaskGuiComp::OnAttached(imod::IModel* modelPtr)
{
	m_editorsList.clear();

	if (!BaseClass::OnAttached(modelPtr)){
		return false;
	}

	iinsp::IInspectionTask* inspectionTaskPtr = GetObjectPtr();
	Q_ASSERT(inspectionTaskPtr != NULL);

	int subtasksCount = inspectionTaskPtr->GetSubtasksCount();
	for (int i = 0; i < subtasksCount; ++i){
		imod::IModel* parameterModelPtr = dynamic_cast<imod::IModel*>(inspectionTaskPtr->GetSubtask(i));
		if (parameterModelPtr == NULL){
			continue;
		}

		if (i < m_editorObserversCompPtr.GetCount()){
			imod::IObserver* observerPtr = m_editorObserversCompPtr[i];

			if (observerPtr != NULL){
				if (parameterModelPtr->AttachObserver(observerPtr)){
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
				parameterModelPtr->AttachObserver(observerPtr);
			}
		}
	}

	if (m_generalParamsObserverCompPtr.IsValid()){
		imod::IModel* generalParamsModelPtr = dynamic_cast<imod::IModel*>(inspectionTaskPtr->GetGeneralParameters());
		if (generalParamsModelPtr != NULL){
			generalParamsModelPtr->AttachObserver(m_generalParamsObserverCompPtr.GetPtr());
		}
	}

	return true;
}


bool CInspectionTaskGuiComp::OnDetached(imod::IModel* modelPtr)
{
	m_editorsList.clear();

	iinsp::IInspectionTask* inspectionTaskPtr = GetObjectPtr();
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

	return BaseClass::OnDetached(modelPtr);
}


// protected methods

void CInspectionTaskGuiComp::UpdateProcessingState()
{
	int workStatus = iproc::ISupplier::WS_INVALID;

	const iproc::ISupplier* supplierPtr = dynamic_cast<const iproc::ISupplier*>(GetObjectPtr());
	if (supplierPtr != NULL){
		workStatus = supplierPtr->GetWorkStatus();
	}

	const istd::IInformationProvider* infoProviderPtr = dynamic_cast<const istd::IInformationProvider*>(supplierPtr);

	switch (workStatus){
		case iproc::ISupplier::WS_OK:
			if (infoProviderPtr != NULL){
				switch (infoProviderPtr->GetInformationCategory()){
					case istd::IInformationProvider::IC_NONE:
						StateIconLabel->setPixmap(QPixmap(":/Icons/StateUnknown.svg"));
						break;

					case istd::IInformationProvider::IC_WARNING:
						StateIconLabel->setPixmap(QPixmap(":/Icons/StateWarning.svg"));
						break;

					case istd::IInformationProvider::IC_ERROR:
					case istd::IInformationProvider::IC_CRITICAL:
						StateIconLabel->setPixmap(QPixmap(":/Icons/StateInvalid.svg"));
						break;

					default:
						StateIconLabel->setPixmap(QPixmap(":/Icons/StateOk.svg"));
						break;
				}
			}
			else{
				StateIconLabel->setPixmap(QPixmap(":/Icons/StateOk.svg"));
			}
			break;

		case iproc::ISupplier::WS_ERROR:
			StateIconLabel->setPixmap(QPixmap(":/Icons/StateInvalid.svg"));
			break;

		case iproc::ISupplier::WS_CRITICAL:
			StateIconLabel->setPixmap(QPixmap(":/Icons/Error.svg"));
			break;

		default:
			StateIconLabel->setPixmap(QPixmap(":/Icons/StateUnknown.svg"));
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
			const iqtgui::IVisualStatusProvider* visualProviderPtr = m_editorVisualInfosCompPtr[guiIndex];
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

	if (!m_paramsLoaderCompPtr.IsValid()){
		LoaderFrame->hide();
	}

	layoutPtr->setMargin(0);

	ParamsFrame->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::MinimumExpanding);

	bool useSpacer = *m_useVerticalSpacerAttrPtr;

	if (*m_designTypeAttrPtr == 1){
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
						RegisterModel(modelPtr, i);
					}
				}
			}
		}

		QObject::connect(m_toolBoxPtr, SIGNAL(currentChanged(int)), this, SLOT(OnEditorChanged(int)));

		layoutPtr->addWidget(m_toolBoxPtr);
	}
	else{
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
						RegisterModel(modelPtr, i);
					}
				}
			}
		}

		QObject::connect(m_tabWidgetPtr, SIGNAL(currentChanged(int)), this, SLOT(OnEditorChanged(int)));

		layoutPtr->addWidget(m_tabWidgetPtr);
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

	UpdateTaskMessages();

	OnEditorChanged(0);

	UpdateVisualElements();

	BaseClass::OnGuiCreated();
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


// reimplemented (imod::CMultiModelDispatcherBase)

void CInspectionTaskGuiComp::OnModelChanged(int modelId, int /*changeFlags*/, istd::IPolymorphic* /*updateParamsPtr*/)
{
	if (modelId == m_currentGuiIndex && !m_testStarted){
		UpdateVisualElements();

		UpdateTaskMessages();

		DoUpdateEditor(m_currentGuiIndex);
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
			DoAutoTest();
		}
	}
}


void CInspectionTaskGuiComp::OnAutoTest()
{
	m_testStarted = true;

	MessageList->clear();
	m_resultShapesMap.clear();

	iproc::ISupplier* supplierPtr = dynamic_cast<iproc::ISupplier*>(GetObjectPtr());
	if (supplierPtr != NULL){
		supplierPtr->InvalidateSupplier();
		supplierPtr->EnsureWorkInitialized();
		supplierPtr->EnsureWorkFinished();

		UpdateVisualElements();

		UpdateTaskMessages();

		DoUpdateEditor(m_currentGuiIndex);

		UpdateProcessingState();
	}

	m_testStarted = false;
}


void CInspectionTaskGuiComp::on_TestAllButton_clicked()
{
	if (m_generalParamsEditorCompPtr.IsValid()){
		m_generalParamsEditorCompPtr->UpdateModel();
	}

	OnAutoTest();
}


void CInspectionTaskGuiComp::on_AutoTestButton_clicked()
{
	if (AutoTestButton->isChecked()){
		OnAutoTest();
	}
}


void CInspectionTaskGuiComp::on_LoadParamsButton_clicked()
{
	iinsp::IInspectionTask* objectPtr = GetObjectPtr();
	if (objectPtr != NULL){
		m_paramsLoaderCompPtr->LoadFromFile(*objectPtr);
	}
}


void CInspectionTaskGuiComp::on_SaveParamsButton_clicked()
{
	iinsp::IInspectionTask* objectPtr = GetObjectPtr();
	if (objectPtr != NULL){
		m_paramsLoaderCompPtr->SaveToFile(*objectPtr);
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
	int shapeIndex = itemPtr->data(0, DR_SHAPE_INDEX).toInt();

	if ((taskIndex >= 0) && m_resultShapesMap.contains(taskIndex)){
		if (taskIndex < m_previewSceneProvidersCompPtr.GetCount()){
			const iqt2d::IViewProvider* viewProviderPtr = m_previewSceneProvidersCompPtr[taskIndex];
			if (viewProviderPtr != NULL){
				iview::IShapeView* viewPtr = viewProviderPtr->GetView();
				if (viewPtr != NULL){
					viewPtr->DeselectAllShapes();
				}
			}
		}

		const istd::TPointerVector<iview::IShape>& resultShapes = m_resultShapesMap[taskIndex];
		if ((shapeIndex >= 0) && (shapeIndex < resultShapes.GetCount())){
			iview::IInteractiveShape* shapePtr = dynamic_cast<iview::IInteractiveShape*>(resultShapes.GetAt(shapeIndex));
			if (shapePtr != NULL){
				shapePtr->SetSelected();
			}
		}
	}
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

void CInspectionTaskGuiComp::AddTaskMessagesToLog(const ibase::IMessageContainer& messageContainer, int taskIndex)
{
	m_resultMessagesMap[taskIndex].CopyFrom(messageContainer, istd::IChangeable::CM_CONVERT);

	ibase::IMessageContainer::Messages messagesList = m_resultMessagesMap[taskIndex].GetMessages();

	int messagesCount = messagesList.count();
	if (messagesCount == 0){
		return;
	}

	QString tabName;
	if (taskIndex < m_namesAttrPtr.GetCount()){
		tabName = m_namesAttrPtr[taskIndex];
	}

	istd::TPointerVector<iview::IShape>& resultShapes = m_resultShapesMap[taskIndex];

	// find view associated with this task
	iview::IShapeView* viewPtr = NULL;
	if (taskIndex < m_previewSceneProvidersCompPtr.GetCount()){
		const iqt2d::IViewProvider* viewProviderPtr = m_previewSceneProvidersCompPtr[taskIndex];
		if (viewProviderPtr != NULL){
			viewPtr = viewProviderPtr->GetView();
		}
	}

	for (int messageIndex = 0; messageIndex < messagesCount; messageIndex++){
		ibase::IMessageConsumer::MessagePtr messagePtr = messagesList[messageIndex];

		int shapeIndex = -1;

		// add result shapes to view and internal shape list
		if (viewPtr != NULL){
			if (m_resultShapeFactoryCompPtr.IsValid()){
				const i2d::IObject2d* object2dPtr = dynamic_cast<const i2d::IObject2d*>(messagePtr.GetPtr());
				if (object2dPtr != NULL){
					iview::IShape* shapePtr = m_resultShapeFactoryCompPtr->CreateShape(*object2dPtr, true);
					if (shapePtr != NULL){
						iview::CShapeBase* shapeBasePtr = dynamic_cast<iview::CShapeBase*>(shapePtr);
						if (shapeBasePtr != NULL){
							shapeBasePtr->SetVisible(false);
						}

						shapeIndex = resultShapes.GetCount();
						resultShapes.PushBack(shapePtr);

						viewPtr->ConnectShape(shapePtr);
					}
				}
			}
		}

		QTreeWidgetItem* messageItemPtr = new QTreeWidgetItem;

		messageItemPtr->setData(0, DR_TASK_INDEX, taskIndex);
		messageItemPtr->setData(0, DR_SHAPE_INDEX, shapeIndex);

		QIcon messageIcon = GetCategoryIcon(messagePtr->GetInformationCategory()).pixmap(QSize(12, 12), QIcon::Normal, QIcon::On);
		messageItemPtr->setIcon(0, messageIcon);

		QString sourceName = messagePtr->GetInformationSource();
		if (sourceName.isEmpty()){
			sourceName = tabName;
		}

		messageItemPtr->setText(0, sourceName);
		messageItemPtr->setText(1, messagePtr->GetInformationDescription());

		MessageList->addTopLevelItem(messageItemPtr);
	}
}


void CInspectionTaskGuiComp::UpdateTaskMessages()
{
	if (!IsGuiCreated()){
		return;
	}

	MessageList->clear();
	m_resultShapesMap.clear();

	iinsp::IInspectionTask* taskPtr = GetObjectPtr();

	if (taskPtr != NULL){
		int subtasksCount = taskPtr->GetSubtasksCount();
		for (int subTaskIndex = 0; subTaskIndex < subtasksCount; subTaskIndex++){
			iproc::ISupplier* subTaskPtr = taskPtr->GetSubtask(subTaskIndex);
			if (subTaskPtr != NULL){
				const ibase::IMessageContainer* messageContainerPtr = subTaskPtr->GetWorkMessages();
				if (messageContainerPtr != NULL){
					AddTaskMessagesToLog(*messageContainerPtr, subTaskIndex);
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
				editorPtr->UpdateEditor();
			}
		}
	}

	// Activate task related shapes:
	ActivateTaskShapes(taskIndex);

	if (viewPtr != NULL){
		viewPtr->Update();
	}
}


void CInspectionTaskGuiComp::ActivateTaskShapes(int taskIndex)
{
	if (!IsGuiCreated()){
		return;
	}

	for (		ResultShapesMap::ConstIterator shapesContainerIter = m_resultShapesMap.constBegin();
				shapesContainerIter != m_resultShapesMap.constEnd();
				++shapesContainerIter){
		int shapeTaskIndex = shapesContainerIter.key();
		const istd::TPointerVector<iview::IShape>& resultShapes = shapesContainerIter.value();

		int shapesCount = resultShapes.GetCount();
		for (int i = 0; i < shapesCount; ++i){
			iview::IShape* shapePtr = resultShapes.GetAt(i);
			Q_ASSERT(shapePtr != NULL);	// only correct instances should be added to container

			iview::CShapeBase* shapeBasePtr = dynamic_cast<iview::CShapeBase*>(shapePtr);
			if (shapeBasePtr != NULL){
				shapeBasePtr->SetVisible(taskIndex == shapeTaskIndex);
			}
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


// private static methods

QIcon CInspectionTaskGuiComp::GetCategoryIcon(istd::IInformationProvider::InformationCategory category)
{
	static QIcon logIcon(":/Icons/Log");
	static QIcon infoIcon(":/Icons/Info.svg");
	static QIcon warningIcon(":/Icons/Warning.svg");
	static QIcon errorIcon(":/Icons/Error.svg");

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


