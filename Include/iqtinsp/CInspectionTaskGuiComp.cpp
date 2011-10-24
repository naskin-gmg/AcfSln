#include "iqtinsp/CInspectionTaskGuiComp.h"


// Qt includes
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSpacerItem>
#include <QToolBox>
#include <QTabWidget>
#include <QGroupBox>


// ACF includes
#include "imod/IModel.h"
#include "imod/IObserver.h"


namespace iqtinsp
{


CInspectionTaskGuiComp::CInspectionTaskGuiComp()
:	m_tasksObserver(this),
	m_currentGuiIndex(-1)
{
	connect(this, SIGNAL(DoAutoTest()), SLOT(OnAutoTest()), Qt::QueuedConnection);
}


// reimplemented (imod::IModelEditor)

void CInspectionTaskGuiComp::UpdateModel() const
{
	I_ASSERT(IsGuiCreated() && (GetObjectPtr() != NULL));

	for (		EditorsList::const_iterator iter = m_editorsList.begin();
				iter != m_editorsList.end();
				++iter){
		const imod::IModelEditor* editorPtr = *iter;
		I_ASSERT(editorPtr != NULL);
		
		editorPtr->UpdateModel();
	}
}


void CInspectionTaskGuiComp::UpdateEditor(int updateFlags)
{
	I_ASSERT(IsGuiCreated());

	for (		EditorsList::const_iterator iter = m_editorsList.begin();
				iter != m_editorsList.end();
				++iter){
		imod::IModelEditor* editorPtr = *iter;
		I_ASSERT(editorPtr != NULL);
		
		editorPtr->UpdateEditor(updateFlags);
	}

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
	I_ASSERT(inspectionTaskPtr != NULL);

	int subtasksCount = inspectionTaskPtr->GetSubtasksCount();
	for (int i = 0; i < subtasksCount; ++i){
		imod::IModel* parameterModelPtr = dynamic_cast<imod::IModel*>(inspectionTaskPtr->GetSubtask(i));
		if (parameterModelPtr == NULL){
			continue;
		}

		parameterModelPtr->AttachObserver(&m_tasksObserver);

		if (i < m_observersCompPtr.GetCount()){
			imod::IObserver* observerPtr = m_observersCompPtr[i];

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
		imod::IModel* generalParamsModelPtr = dynamic_cast<imod::IModel*>(inspectionTaskPtr->GetTaskParams());
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
	I_ASSERT(inspectionTaskPtr != NULL);

	int subtasksCount = inspectionTaskPtr->GetSubtasksCount();
	for (int i = 0; i < subtasksCount; ++i){
		imod::IModel* parameterModelPtr = dynamic_cast<imod::IModel*>(inspectionTaskPtr->GetSubtask(i));
		if (parameterModelPtr == NULL){
			continue;
		}

		parameterModelPtr->DetachObserver(&m_tasksObserver);

		if (i < m_observersCompPtr.GetCount()){
			imod::IObserver* observerPtr = m_observersCompPtr[i];

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
		imod::IModel* generalParamsModelPtr = dynamic_cast<imod::IModel*>(inspectionTaskPtr->GetTaskParams());
		if ((generalParamsModelPtr != NULL) && generalParamsModelPtr->IsAttached(m_generalParamsObserverCompPtr.GetPtr())){
			generalParamsModelPtr->DetachObserver(m_generalParamsObserverCompPtr.GetPtr());
		}
	}

	return BaseClass::OnDetached(modelPtr);
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

	if (*m_designTypeAttrPtr == 1){
		QToolBox* toolBoxPtr = new QToolBox(ParamsFrame);
		int subtasksCount = m_guisCompPtr.GetCount();
		for (int i = 0; i < subtasksCount; ++i){
			iqtgui::IGuiObject* guiPtr = m_guisCompPtr[i];

			if (guiPtr != NULL){
				QWidget* panelPtr = new QWidget(ParamsFrame);
				QLayout* panelLayoutPtr = new QVBoxLayout(panelPtr);
				panelLayoutPtr->setContentsMargins(6, 0, 6, 0);
				QString name;
				if (i < m_namesAttrPtr.GetCount()){
					name = iqt::GetQString(m_namesAttrPtr[i]);
				}

				guiPtr->CreateGui(panelPtr);

				toolBoxPtr->addItem(panelPtr, name);

				QSpacerItem* spacerPtr = new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding);

				panelLayoutPtr->addItem(spacerPtr);
			}
		}

		QObject::connect(toolBoxPtr, SIGNAL(currentChanged(int)), this, SLOT(OnEditorChanged(int)));

		layoutPtr->addWidget(toolBoxPtr);
	}
	else{
		QTabWidget* tabWidgetPtr = new QTabWidget(ParamsFrame);
		tabWidgetPtr->setTabPosition(QTabWidget::TabPosition(*m_tabOrientationAttrPtr));

		int subtasksCount = m_guisCompPtr.GetCount();
		for (int i = 0; i < subtasksCount; ++i){
			iqtgui::IGuiObject* guiPtr = m_guisCompPtr[i];

			if (guiPtr != NULL){
				QWidget* panelPtr = new QWidget(tabWidgetPtr);
				QLayout* panelLayoutPtr = new QVBoxLayout(panelPtr);
				QString name;
				if (i < m_namesAttrPtr.GetCount()){
					name = iqt::GetQString(m_namesAttrPtr[i]);
				}

				guiPtr->CreateGui(panelPtr);

				tabWidgetPtr->addTab(panelPtr, name);

				QSpacerItem* spacerPtr = new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding);

				panelLayoutPtr->addItem(spacerPtr);
			}
		}

		QObject::connect(tabWidgetPtr, SIGNAL(currentChanged(int)), this, SLOT(OnEditorChanged(int)));

		layoutPtr->addWidget(tabWidgetPtr);
	}

	std::map<iqtgui::IGuiObject*, int> guiToStackIndexMap;

	int previewGuisCount = m_previewGuisCompPtr.GetCount();
	for (int previewIndex = 0; previewIndex < previewGuisCount; ++previewIndex){
		iqtgui::IGuiObject* guiObjectPtr = m_previewGuisCompPtr[previewIndex];
		if (guiObjectPtr == NULL){
			continue;
		}

		int stackIndex;
		std::map<iqtgui::IGuiObject*, int>::const_iterator stackRepeatIter = guiToStackIndexMap.find(guiObjectPtr);
		if (stackRepeatIter == guiToStackIndexMap.end()){
			istd::TDelPtr<QWidget> newPreviewPagePtr(new QWidget());
			if (!newPreviewPagePtr.IsValid()){
				continue;
			}

			new QVBoxLayout(newPreviewPagePtr.GetPtr());
			newPreviewPagePtr->setContentsMargins(0, 0, 0, 0);

			if (!guiObjectPtr->CreateGui(newPreviewPagePtr.GetPtr())){
				continue;
			}

			stackIndex = PreviewStack->addWidget(newPreviewPagePtr.PopPtr());
			m_stackIndexToTabMap[stackIndex] = previewIndex;

			guiToStackIndexMap[guiObjectPtr] = stackIndex;
		}
		else{
			stackIndex = stackRepeatIter->second;
		}

		m_tabToStackIndexMap[previewIndex] = stackIndex;
	}

	if (m_generalParamsGuiCompPtr.IsValid()){
		m_generalParamsGuiCompPtr->CreateGui(GeneralParamsFrame);
	}

	OnEditorChanged(0);

	BaseClass::OnGuiCreated();
}


void CInspectionTaskGuiComp::OnGuiDestroyed()
{
	m_editorsList.clear();

	int subtasksCount = m_guisCompPtr.GetCount();
	for (int i = 0; i < subtasksCount; ++i){
		iqtgui::IGuiObject* guiPtr = m_guisCompPtr[i];

		if ((guiPtr != NULL) && guiPtr->IsGuiCreated()){
			guiPtr->DestroyGui();
		}
	}

	for (		GuiMap::const_iterator iter = m_stackIndexToTabMap.begin();
				iter != m_stackIndexToTabMap.end();
				++iter){
		int guiIndex = iter->second;
		I_ASSERT(guiIndex >= 0);
		I_ASSERT(guiIndex < m_previewGuisCompPtr.GetCount());

		iqtgui::IGuiObject* guiPtr = m_previewGuisCompPtr[guiIndex];
		I_ASSERT(guiPtr != NULL);

		guiPtr->DestroyGui();
	}

	if (m_generalParamsGuiCompPtr.IsValid() && m_generalParamsGuiCompPtr->IsGuiCreated()){
		m_generalParamsGuiCompPtr->DestroyGui();
	}

	BaseClass::OnGuiDestroyed();
}


// protected slots

void CInspectionTaskGuiComp::OnEditorChanged(int index)
{
	if (index != m_currentGuiIndex){
		int extendersCount = m_extendersCompPtr.GetCount();
		int previewProvidersCount = m_previewSceneProvidersCompPtr.GetCount();

		if ((m_currentGuiIndex >= 0) && (m_currentGuiIndex < extendersCount) && (m_currentGuiIndex < previewProvidersCount)){
			iqt2d::ISceneExtender* extenderPtr = m_extendersCompPtr[m_currentGuiIndex];
			iqt2d::ISceneProvider* previewProviderPtr = m_previewSceneProvidersCompPtr[m_currentGuiIndex];
			if ((extenderPtr != NULL) && (previewProviderPtr != NULL)){
				extenderPtr->RemoveItemsFromScene(previewProviderPtr);
			}
		}

		if ((index >= 0) && (index < extendersCount) && (index < previewProvidersCount)){
			iqt2d::ISceneExtender* extenderPtr = m_extendersCompPtr[index];
			iqt2d::ISceneProvider* previewProviderPtr = m_previewSceneProvidersCompPtr[index];
			if ((extenderPtr != NULL) && (previewProviderPtr != NULL)){
				extenderPtr->AddItemsToScene(previewProviderPtr, iqt2d::ISceneExtender::SF_DIRECT);
			}
		}

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
	iinsp::IInspectionTask* objectPtr = GetObjectPtr();
	if (objectPtr != NULL){
		int subtasksCount = objectPtr->GetSubtasksCount();

		if ((m_currentGuiIndex >= 0) && (m_currentGuiIndex < subtasksCount)){
			iproc::ISupplier* subtaskPtr = objectPtr->GetSubtask(m_currentGuiIndex);
			if (subtaskPtr != NULL){
				subtaskPtr->EnsureWorkFinished();
			}
		}

		UpdateProcessingState();
	}
}


void CInspectionTaskGuiComp::on_TestAllButton_clicked()
{
	if (m_generalParamsEditorCompPtr.IsValid()){
		m_generalParamsEditorCompPtr->UpdateModel();
	}

	iinsp::IInspectionTask* objectPtr = GetObjectPtr();
	if (objectPtr != NULL){
		int subtasksCount = objectPtr->GetSubtasksCount();

		for (int invalidateIndex = 0; invalidateIndex < subtasksCount; ++invalidateIndex){
			iproc::ISupplier* subtaskPtr = objectPtr->GetSubtask(invalidateIndex);
			if (subtaskPtr != NULL){
				subtaskPtr->InvalidateSupplier();
			}
		}

		for (int finishIndex = 0; finishIndex < subtasksCount; ++finishIndex){
			iproc::ISupplier* subtaskPtr = objectPtr->GetSubtask(finishIndex);
			if (subtaskPtr != NULL){
				subtaskPtr->EnsureWorkFinished();
			}
		}

		UpdateProcessingState();
	}
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


// private methods

void CInspectionTaskGuiComp::UpdateProcessingState()
{
	iinsp::IInspectionTask* objectPtr = GetObjectPtr();
	if (objectPtr != NULL){
		int subtasksCount = objectPtr->GetSubtasksCount();

		int workStatus = iproc::ISupplier::WS_NONE;
		for (int invalidateIndex = 0; invalidateIndex < subtasksCount; ++invalidateIndex){
			iproc::ISupplier* subtaskPtr = objectPtr->GetSubtask(invalidateIndex);
			if (subtaskPtr != NULL){
				workStatus = istd::Max(workStatus, subtaskPtr->GetWorkStatus());
			}
		}

		switch (workStatus){
			case iproc::ISupplier::WS_OK:
				StateIconLabel->setPixmap(QPixmap(":/Icons/StateOk.svg"));
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
}


// public methods of embedded class TasksObserver

CInspectionTaskGuiComp::TasksObserver::TasksObserver(CInspectionTaskGuiComp* parentPtr)
:	m_parent(*parentPtr)
{
	I_ASSERT(parentPtr != NULL);
}


// protected methods of embedded class TasksObserver

void CInspectionTaskGuiComp::TasksObserver::OnUpdate(imod::IModel* /*modelPtr*/, int updateFlags, istd::IPolymorphic* /*updateParamsPtr*/)
{
	if (		m_parent.IsGuiCreated() &&
				m_parent.AutoTestButton->isChecked() &&
				((updateFlags & iproc::ISupplier::CF_SUPPLIER_RESULTS) != 0)){
		emit m_parent.DoAutoTest();
	}
}


} // namespace iqtinsp


