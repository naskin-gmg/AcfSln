// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#include <iqtinsp/CInspectionTaskGuiComp.h>


// Qt includes
#include <QtCore/QtGlobal>
#include <QtCore/QMimeData>
#include <QtCore/QVariantAnimation>
#include <QtGui/QClipboard>
#include <QtWidgets/QScrollBar>
#if QT_VERSION >= 0x050000
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QMenu>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QScrollArea>
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
#include <iview/CViewBase.h>
#include <iview/CImageShape.h>
#include <iqtgui/iqtgui.h>


namespace iqtinsp
{


// private static stuff

static const char InspectionTaskMimeType[] = "acf/iqtinsp::CInspectionTask";
static const char SupplierTaskMimeType[] = "acf/iinsp::ISupplier";


static const QSize LOG_ICON_SIZE{ 18,18 };



// public

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
	m_commands.InsertChild(&m_executeTaskCommand);
	m_commands.InsertChild(&m_continuousExecuteCommand);

	m_rootCommands.InsertChild(&m_commands);

	m_autoTestTimer.setSingleShot(true);
	m_autoTestTimer.setInterval(0);
}


// selection API

bool CInspectionTaskGuiComp::SelectTask(int index)
{
	if (index < 0 || index >= m_editorsCompPtr.GetCount()) {
		return false;
	}

	if (m_buttonGroupPtr) {
		m_buttonGroupPtr->button(index)->click();
		return true;
	}

	if (m_toolBoxPtr) {
		m_toolBoxPtr->setCurrentIndex(index);
		return true;
	}

	if (m_tabWidgetPtr) {
		m_tabWidgetPtr->setCurrentIndex(index);
		return true;
	}

	return false;
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
	int extendersCount = m_editorViewExtendersCompPtr.GetCount();
	int previewProvidersCount = m_previewSceneProvidersCompPtr.GetCount();

	iview::IShapeView* viewPtr = NULL;

	for (int i = 0; i < qMin(extendersCount, previewProvidersCount); ++i){
		iqt2d::IViewExtender* extenderPtr = m_editorViewExtendersCompPtr[i];
		iqt2d::IViewProvider* previewProviderPtr = m_previewSceneProvidersCompPtr[i];
		if ((extenderPtr != NULL) && (previewProviderPtr != NULL)){
			extenderPtr->RemoveItemsFromScene(previewProviderPtr);

			viewPtr = previewProviderPtr->GetView();
			if (viewPtr!= NULL){
				viewPtr->Update();
			}
		}
	}

	m_resultShapesMap.clear();
	m_tempShapesMap.clear();
	m_tempMessagesMap.clear();
	m_resultMessagesMap.clear();

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
				auto category = infoProviderPtr->GetInformationCategory();
				StateIconLabel->setPixmap(GetCategoryPixmap(category));
			}
			else{
				StateIconLabel->setPixmap(GetCategoryPixmap(istd::IInformationProvider::IC_INFO));
			}
			break;

		case iinsp::ISupplier::WS_FAILED:
			StateIconLabel->setPixmap(GetCategoryPixmap(istd::IInformationProvider::IC_CRITICAL));
			break;

		default:
			StateIconLabel->setPixmap(GetCategoryPixmap(istd::IInformationProvider::IC_NONE));
			break;
	}
}


void CInspectionTaskGuiComp::UpdateVisualElements()
{
	if (!IsGuiCreated()){
		return;
	}

	auto inspectionTaskPtr = GetObservedObject();
	if (inspectionTaskPtr == nullptr)
		return;

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
	
			auto taskPtr = inspectionTaskPtr->GetSubtask(tabIndex);
			Q_ASSERT(taskPtr);

			int taskStatus = taskPtr->GetWorkStatus();
			buttonPtr->setStyleSheet("");

			if (taskStatus == iinsp::ISupplier::WS_OK){

				auto infoProviderPtr = dynamic_cast<istd::IInformationProvider*>(taskPtr);
				if (infoProviderPtr){
					auto category = infoProviderPtr->GetInformationCategory();

					DecorateButton(buttonPtr, (int)category);
				}
			}
			else if (taskStatus == iinsp::ISupplier::WS_FAILED){
				DecorateButton(buttonPtr, (int)istd::IInformationProvider::IC_CRITICAL);
			}
		}
	}

	if (m_disableAutoTestAttrPtr.IsValid()){
		bool disable = m_disableAutoTestAttrPtr->GetValue();
		m_continuousExecuteCommand.SetEnabled(!disable);
		AutoTestButton->setEnabled(!disable);
		AutoTestButton->setVisible(!disable);
	}
}


void CInspectionTaskGuiComp::DecorateButton(QWidget* buttonPtr, int category) const
{
	if (m_darkMode){
		// TDB
		switch (category){
			case istd::IInformationProvider::IC_WARNING:
				buttonPtr->setStyleSheet("background: none; border-right: 4px solid #e25303;");
				return;
			case istd::IInformationProvider::IC_ERROR:
			case istd::IInformationProvider::IC_CRITICAL:
				buttonPtr->setStyleSheet("background: none; border-right: 4px solid red;");
				return;
			default:
				buttonPtr->setStyleSheet("");
				return;
		}
	}
	else {
		switch (category){
			case istd::IInformationProvider::IC_WARNING:
				buttonPtr->setStyleSheet("background: #fffff0; border-right: 4px solid #e25303;");
				return;
			case istd::IInformationProvider::IC_ERROR:
			case istd::IInformationProvider::IC_CRITICAL:
				buttonPtr->setStyleSheet("background: #fff0f0; border-right: 4px solid red;");
				return;
			default:
				buttonPtr->setStyleSheet("");
				return;
		}
	}
}


void CInspectionTaskGuiComp::UpdateParametersVisibility()
{
	QVariantAnimation *qva = new QVariantAnimation(this);
	qva->setDuration(200);

	connect(qva, &QVariantAnimation::valueChanged, [=](const QVariant &value){
		int siz = value.toInt();
		ControlFrame->setFixedWidth(siz);
		GeneralParamsFrame->setFixedWidth(siz);
		ParamsFrame->setFixedWidth(siz);
		PreviewSplitter->setSizes({ 1, value.toInt() });
	});

	connect(qva, &QVariantAnimation::finished, [=](){
		ControlFrame->setMaximumWidth(INT_MAX);
		GeneralParamsFrame->setMaximumWidth(INT_MAX);
		ParamsFrame->setMaximumWidth(INT_MAX);
		ControlFrame->setMinimumWidth(0);
		GeneralParamsFrame->setMinimumWidth(0);
		ParamsFrame->setMinimumWidth(0);
	});
	
	if (PreviewSplitter->sizes().at(1)){
		m_lastPanelWidth = PreviewSplitter->sizes().at(1);
		qva->setStartValue((double)0);
		qva->setEndValue((double)m_lastPanelWidth);
		qva->setDirection(QAbstractAnimation::Backward);
		qva->start(QAbstractAnimation::DeleteWhenStopped);
	}
	else {
		qva->setStartValue((double)0);
		if (!m_lastPanelWidth) m_lastPanelWidth = 400;	// default size
		qva->setEndValue((double)m_lastPanelWidth);
		qva->start(QAbstractAnimation::DeleteWhenStopped);
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


	// drop minimum width to make panel resizable again (a workaround)
	ControlFrame->setMinimumWidth(0);
	GeneralParamsFrame->setMinimumWidth(0);
	ParamsFrame->setMinimumWidth(0);
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


void CInspectionTaskGuiComp::OnGuiModelAttached()
{
	BaseClass::OnGuiModelAttached();

	// handle license
	if (m_licenseProviderCompPtr.IsValid() && m_licenseEnableIds.IsValid()) {
		for (int i = 0; i < m_licenseEnableIds.GetCount(); ++i) {
			if (m_licenseEnableIds[i].size()) {
				bool hasRight = (m_licenseProviderCompPtr->HasRight(m_licenseEnableIds[i], true));

				// for now for buttons only
				if (m_buttonGroupPtr && m_buttonGroupPtr->button(i)) {
					m_buttonGroupPtr->button(i)->setVisible(hasRight);
				}
			}
		}

	}
}


void CInspectionTaskGuiComp::OnGuiModelDetached()
{
	MessageList->clear();

	BaseClass::OnGuiModelDetached();
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

	layoutPtr->setContentsMargins(0,0,0,0);

	ControlFrame->setVisible(*m_showControlPanelAttrPtr);

	ParamsFrame->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::MinimumExpanding);

	bool useSpacer = *m_useVerticalSpacerAttrPtr;

	RightPanel->hide();

	MessageList->header()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
	MessageList->setIconSize(LOG_ICON_SIZE);

	// identification
	GetQtWidget()->setProperty("CInspectionTaskGuiComp", (qulonglong)this);
	RightPanel->setProperty("CInspectionTaskGuiComp", (qulonglong)this);

	// buttons
	switch (*m_designTypeAttrPtr)
	{
		case 0: // stack
		{
			ControlFrame->setTitle("");	// no group title

			RightPanel->setLayout(new QVBoxLayout);
			RightPanel->layout()->setContentsMargins(0, 4, 0, 2);
			RightPanel->layout()->setSpacing(8);
			RightPanel->show();

			m_stackedWidgetPtr = new QStackedWidget(ParamsFrame);
			m_buttonGroupPtr = new QButtonGroup(ParamsFrame);

#if QT_VERSION >= 0x050E00
			connect(m_buttonGroupPtr, &QButtonGroup::idClicked, m_stackedWidgetPtr, &QStackedWidget::setCurrentIndex);
#else
			connect(m_buttonGroupPtr, QOverload<QAbstractButton*>::of(&QButtonGroup::buttonClicked), [=](QAbstractButton* button){
				m_stackedWidgetPtr->setCurrentIndex(m_buttonGroupPtr->id(button));
			}
			);
#endif

			ControlFrame->setMinimumWidth(450);

			int subtasksCount = m_editorGuisCompPtr.GetCount();
			for (int i = 0; i < subtasksCount; ++i){
				iqtgui::IGuiObject* guiPtr = m_editorGuisCompPtr[i];

				if (guiPtr != NULL){
					QWidget* panelPtr = new QWidget(m_stackedWidgetPtr);
					QLayout* panelLayoutPtr = new QVBoxLayout(panelPtr);
					panelLayoutPtr->setContentsMargins(0, 0, 0, 0);
					
					QString name, shortName;
					if (i < m_namesAttrPtr.GetCount()){
						name = m_namesAttrPtr[i];
					}

					if (i < m_shortNamesAttrPtr.GetCount()){
						shortName = m_shortNamesAttrPtr[i];
					}
					else {
						shortName = name;
					}

					QIcon icon;
					if (i < m_iconsAttrPtr.GetCount()){
						icon = QIcon(m_iconsAttrPtr[i]);
					}

					// headers
					if (name.size()){
						QLabel* headerPtr = new QLabel(name, panelPtr);
						headerPtr->setStyleSheet(
							"border: 1px solid transparent;"
							"border-radius: 3px;"
							"background-color: #1A76E7;"
							"color: white;"
							"font-size: 12pt;"
							"padding: 8px;");
						headerPtr->setAlignment(Qt::AlignCenter);
						panelLayoutPtr->addWidget(headerPtr);
					}

					auto buttonPtr = new QToolButton(RightPanel);
					buttonPtr->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
					buttonPtr->setAutoRaise(true);
					buttonPtr->setIcon(icon);
					buttonPtr->setToolTip(name);
					buttonPtr->setText(shortName);
					buttonPtr->setCheckable(true);
					buttonPtr->setFixedSize(70, 70);
					buttonPtr->setIconSize(QSize(34, 34));
					RightPanel->layout()->addWidget(buttonPtr);
					m_buttonGroupPtr->addButton(buttonPtr, i);

					buttonPtr->installEventFilter(this);

					guiPtr->CreateGui(panelPtr);

					auto areaPtr = new QScrollArea(GetQtWidget());
					areaPtr->setWidget(panelPtr);
					areaPtr->setWidgetResizable(true);
					areaPtr->setFrameStyle(QFrame::NoFrame);

					int toolBoxIndex = m_stackedWidgetPtr->addWidget(areaPtr);

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

			QObject::connect(m_stackedWidgetPtr, SIGNAL(currentChanged(int)), this, SLOT(OnEditorChanged(int)));

			layoutPtr->addWidget(m_stackedWidgetPtr);

			// add close button
			QVBoxLayout* vbl = (QVBoxLayout*)(RightPanel->layout());
			vbl->addStretch();

			auto closeButtonPtr = new QToolButton(RightPanel);
			closeButtonPtr->setToolButtonStyle(Qt::ToolButtonIconOnly);
			closeButtonPtr->setAutoRaise(true);
			closeButtonPtr->setFixedSize(70, 24);
			closeButtonPtr->setIconSize(QSize(20, 20));
			closeButtonPtr->setIcon(GetIcon(":/Icons/Menu"));
			//closeButtonPtr->setIcon(GetIcon(":/TaskIcons/ClosePanel"));
			closeButtonPtr->setToolTip(tr("Show/Hide Parameters"));
			vbl->addWidget(closeButtonPtr);
			QObject::connect(closeButtonPtr, &QToolButton::clicked, [=](){
				UpdateParametersVisibility();
			});

			// go 1st tab
			auto* botton0 = m_buttonGroupPtr->button(0);
			Q_ASSERT(botton0 != nullptr);
			botton0->click();
		}
		break;

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

	if (m_hideRightPanelAttrPtr.IsValid())
		RightPanel->setHidden(*m_hideRightPanelAttrPtr);

	if (m_hideMessageListAttrPtr.IsValid())
		MessageList->setHidden(*m_hideMessageListAttrPtr);

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

	CreateMessagesFilter();

	UpdateTaskMessages();

	OnEditorChanged(0);

	UpdateVisualElements();

	BaseClass::OnGuiCreated();

	// establishing connections
	const Qt::ConnectionType uniqueQueued = static_cast<Qt::ConnectionType>(Qt::QueuedConnection | Qt::UniqueConnection);

	connect(&m_continuousExecuteCommand, SIGNAL(toggled(bool)), AutoTestButton, SLOT(setChecked(bool)));
	connect(AutoTestButton, SIGNAL(clicked()), this, SLOT(OnAutoTestButtonClicked()), uniqueQueued);
	connect(&m_executeTaskCommand, SIGNAL(triggered()), this, SLOT(OnAutoTest()), uniqueQueued);
	connect(TestAllButton, SIGNAL(clicked()), this, SLOT(OnTestAll()), uniqueQueued);
	connect(&m_autoTestTimer, SIGNAL(timeout()), this, SLOT(OnAutoTest()), uniqueQueued);
}


void CInspectionTaskGuiComp::OnGuiDestroyed()
{
	disconnect(&m_continuousExecuteCommand, SIGNAL(toggled(bool)), AutoTestButton, SLOT(setChecked(bool)));
	disconnect(AutoTestButton, SIGNAL(clicked()), this, SLOT(OnAutoTestButtonClicked()));
	disconnect(&m_executeTaskCommand, SIGNAL(triggered()), this, SLOT(OnAutoTest()));
	disconnect(TestAllButton, SIGNAL(clicked()), this, SLOT(OnTestAll()));
	disconnect(&m_autoTestTimer, SIGNAL(timeout()), this, SLOT(OnAutoTest()));

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

	if (m_frameNameAttrPtr.IsValid())
		ControlFrame->setTitle(*m_frameNameAttrPtr);

	if (m_buttonNameAttrPtr.IsValid())
		TestAllButton->setText(*m_buttonNameAttrPtr);

	if (m_buttonTooltipAttrPtr.IsValid())
		TestAllButton->setToolTip(*m_buttonTooltipAttrPtr);
		
	m_executeTaskCommand.SetVisuals(tr("Execute"), tr("Execute"), tr("Execute processing pipline"), GetIcon(":/Icons/Play"));
	m_continuousExecuteCommand.SetVisuals(tr("Continuous"), tr("Continuous"), tr("Enable continuous execution of the processing pipeline"), GetIcon(":/Icons/AutoUpdate"));

	int subtasksCount = m_editorGuisCompPtr.GetCount();

	for (int i = 0; i < subtasksCount; ++i){
		iqtgui::IGuiObject* guiPtr = m_editorGuisCompPtr[i];
		if (guiPtr != NULL){
			QString name, shortName;
			if (i < m_namesAttrPtr.GetCount()){
				name = m_namesAttrPtr[i];
			}

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

			default:	// stacked buttons
				if (i < m_shortNamesAttrPtr.GetCount()){
					shortName = m_shortNamesAttrPtr[i];
				}
				else {
					shortName = name;
				}

				m_buttonGroupPtr->button(i)->setText(shortName);
				m_buttonGroupPtr->button(i)->setToolTip(name);

				ControlFrame->setTitle("");	// no group title

				break;
			}
		}
	}

	UpdateVisualElements();
}


void CInspectionTaskGuiComp::OnGuiDesignChanged()
{
	BaseClass::OnGuiDesignChanged();

	auto themeId = GetCurrentThemeId();
	m_darkMode = (themeId.toLower().contains("dark"));

	if (m_darkMode){
		RightPanel->setStyleSheet(
			"QAbstractButton{font-size:10px; border:4px solid transparent; border-right:4px solid transparent; color: #ddd;} "
			"QAbstractButton:hover{border-right:4px solid #0078D7; color: #fff;} "
			"QAbstractButton:checked{border-right:4px solid #0078D7; color: #fff; "
			"border-top:4px solid #0078D7; border-bottom:4px solid #0078D7; border-left:4px solid #0078D7;} "
		);
	} 
	else {
		RightPanel->setStyleSheet(
			"QAbstractButton{font-size:10px; border:4px solid transparent; border-right:4px solid transparent; color: #335777;} "
			"QAbstractButton:hover{border-right:4px solid #0078D7; color: #0078D7; background: #f0f0ff;} "
			"QAbstractButton:checked{border-right:4px solid #0078D7; color: #0078D7; background: #f0f0ff; "
			"border-top:4px solid #0078D7; border-bottom:4px solid #0078D7; border-left:4px solid #0078D7;} "
		);
	}

	AutoTestButton->setIcon(GetIcon(":/Icons/AutoUpdate"));
	TestAllButton->setIcon(GetIcon(":/Icons/Play"));

	UpdateVisualElements();
	UpdateProcessingState();
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


// reimplemented (QObject)

bool CInspectionTaskGuiComp::eventFilter(QObject* obj, QEvent* event)
{
	if (event->type() == QEvent::MouseButtonDblClick){
		if (dynamic_cast<QToolButton*>(obj)){
			UpdateParametersVisibility();
			return true;
		}
	}

	return BaseClass::eventFilter(obj, event);
}


// reimplemented (iview::IViewEventObserver)

bool CInspectionTaskGuiComp::OnSelectChange(
			const iview::IShapeView& /*view*/,
			const istd::CIndex2d& /*position*/,
			const iview::IInteractiveShape& /*shape*/,
			bool /*state*/)
{
	return false;
}


bool CInspectionTaskGuiComp::OnViewMouseButton(
			const iview::IShapeView& /*view*/,
			const istd::CIndex2d& position,
			Qt::MouseButton buttonType,
			bool state,
			const iview::IInteractiveShape* /*shapePtr*/)
{
	if (buttonType == Qt::LeftButton && state){
		const auto& shapeVector = m_resultShapesMap[m_currentGuiIndex];

		// look for simple touched shape
		iview::IShape* touchedShapePtr = nullptr;

		for (int i = 0; i < shapeVector.GetCount(); ++i){
			auto shapePtr = shapeVector.GetElementAt(i);
			if (shapePtr->IsVisible() && shapePtr->IsTouched(position) != iview::ITouchable::TS_NONE){
				touchedShapePtr = shapePtr;
				break;
			}
		}

		// look for area touched shape
		if (!touchedShapePtr)
			for (int i = 0; i < shapeVector.GetCount(); ++i){
				auto shapePtr = shapeVector.GetElementAt(i);
				// force check area touch
				bool oldFlag = shapePtr->IsAreaTouchAllowed();
				if (!oldFlag) shapePtr->SetAreaTouchAllowed(true);

				if (shapePtr->IsVisible() && shapePtr->IsTouched(position) != iview::ITouchable::TS_NONE){
					shapePtr->SetAreaTouchAllowed(oldFlag);
					touchedShapePtr = shapePtr;
					break;
				}

				shapePtr->SetAreaTouchAllowed(oldFlag);
			}

		if (!touchedShapePtr)
			return false;

		// select the shape & message
		int msgCount = MessageList->topLevelItemCount();
		for (int msg = 0; msg < msgCount; ++msg){
			auto itemPtr = MessageList->topLevelItem(msg);
			QVariantList shapePointers = itemPtr->data(0, DR_SHAPE_POINTERS).toList();
			for (auto& it : shapePointers){
				iview::IShape* itemShapePtr = (iview::IShape*)(it.value<quintptr>());
				if (itemShapePtr == touchedShapePtr){
					MessageList->setCurrentItem(itemPtr);
					iview::IInteractiveShape* interactiveShapePtr = dynamic_cast<iview::IInteractiveShape*>(touchedShapePtr);
					if (interactiveShapePtr != NULL){
						interactiveShapePtr->SetSelected(true);
					}
					return false;
				}
			}
		}

	}

	return false;
}


bool CInspectionTaskGuiComp::OnViewMouseMove(
			const iview::IShapeView& /*view*/,
			const istd::CIndex2d& /*position*/)
{
	return false;
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

	if (m_testStarted){
		return;
	}

	m_testStarted = true;

	MessageList->clear();
	m_resultShapesMap.clear();
	m_tempShapesMap.clear();

	iinsp::ISupplier* supplierPtr = dynamic_cast<iinsp::ISupplier*>(GetObservedObject());
	if (supplierPtr != NULL){
		istd::CChangeGroup changeGroup(supplierPtr);

		supplierPtr->ClearWorkResults();
		supplierPtr->InvalidateSupplier();
		supplierPtr->EnsureWorkInitialized();
		supplierPtr->EnsureWorkFinished();
	}

	m_testStarted = false;

	QCoreApplication::removePostedEvents(this, QEvent::MetaCall); // prevents endless testing if test button is clicked continuously

	if (AutoTestButton->isChecked()){
		m_autoTestTimer.start();
	}
}


void CInspectionTaskGuiComp::OnTestAll()
{
	if (m_generalParamsEditorCompPtr.IsValid()){
		m_generalParamsEditorCompPtr->UpdateModelFromEditor();
	}

	OnAutoTest();
}


void CInspectionTaskGuiComp::OnAutoTestButtonClicked()
{
	if (AutoTestButton->isChecked())
		OnAutoTest();
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


void CInspectionTaskGuiComp::on_MessageList_itemDoubleClicked(QTreeWidgetItem* itemPtr, int /*column*/)
{
	Q_ASSERT(itemPtr != NULL);

	int taskIndex = itemPtr->data(0, DR_TASK_INDEX).toInt();

	// switch to the required page
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

			if (m_buttonGroupPtr != nullptr){
				m_buttonGroupPtr->button(tabIndex)->click();
			}

			break;
		}
	}

	// #11249
	QVariantList shapePointers = itemPtr->data(0, DR_SHAPE_POINTERS).toList();
	for (auto& it : shapePointers){
		iview::IShape* shapePtr = (iview::IShape*)(it.value<quintptr>());
		if (shapePtr){
			if (auto previewProviderPtr = m_previewSceneProvidersCompPtr[m_currentGuiIndex]){
				if (auto viewPtr = previewProviderPtr->GetView()){
					if (auto viewBasePtr = dynamic_cast<iview::CViewBase*>(viewPtr)){
						viewBasePtr->CenterTo(*shapePtr);
						return;
					}
				}
			}
		}
	}
}


// private methods

void CInspectionTaskGuiComp::CreateMessagesFilter()
{
	QHeaderView* header = MessageList->header();
	QHBoxLayout* layout = new QHBoxLayout(header);
	layout->setContentsMargins(0, 0, 0, 0);
	layout->addStretch();

	QPushButton* allButton = new QPushButton(GetCategoryLogIcon(istd::IInformationProvider::IC_NONE), tr("All Messages"), header);
	allButton->setToolTip(tr("Display all messages"));
	allButton->setCheckable(true);
	allButton->setChecked(true);
	allButton->setAutoExclusive(true);
	QPushButton* warnButton = new QPushButton(GetCategoryLogIcon(istd::IInformationProvider::IC_WARNING), tr("Warnings"), header);
	warnButton->setToolTip(tr("Display only warning and error messages"));
	warnButton->setCheckable(true);
	warnButton->setAutoExclusive(true);
	QPushButton* errorButton = new QPushButton(GetCategoryLogIcon(istd::IInformationProvider::IC_ERROR), tr("Errors"), header);
	errorButton->setToolTip(tr("Display error messages only"));
	errorButton->setCheckable(true);
	errorButton->setAutoExclusive(true);

	layout->addWidget(allButton);
	layout->addWidget(warnButton);
	layout->addWidget(errorButton);
	layout->addStretch();

	connect(allButton, &QPushButton::toggled, [=](bool down){ if (down){ m_filterStatus = 0; UpdateTaskMessages(); }});
	connect(warnButton, &QPushButton::toggled, [=](bool down){ if (down){ m_filterStatus = 2; UpdateTaskMessages(); }});
	connect(errorButton, &QPushButton::toggled, [=](bool down){ if (down){ m_filterStatus = 3; UpdateTaskMessages(); }});
}


void CInspectionTaskGuiComp::AddTaskMessagesToLog(const ilog::IMessageContainer& messageContainer, int taskIndex, bool isAuxiliary)
{
	ilog::IMessageContainer::Messages messagesList = messageContainer.GetMessages();
	if (messagesList.empty()){
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

	for (auto it = messagesList.rbegin(); it != messagesList.rend(); ++it)
	{
		ilog::IMessageConsumer::MessagePtr messagePtr = *it;
		auto category = messagePtr->GetInformationCategory();
		if (category < m_filterStatus)
			continue;

		QList<QVariant> shapeIndices;
		QList<QVariant> shapePointers;

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
						shapePtr->SetDefaultDescription(messagePtr->GetInformationDescription());

						const QString& objectDescription = extMessagePtr->GetAttachedObjectDescription(i);
						if (!objectDescription.isEmpty()){
							shapePtr->SetToolTip(objectDescription);
						}

						shapeIndices += QVariant(resultShapes.GetCount());
						resultShapes.PushBack(shapePtr);

						shapePointers << (quintptr)shapePtr;

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

						shapePointers << (quintptr)shapePtr;

						viewPtr->ConnectShape(shapePtr);
					}
				}
			}
		}

		QTreeWidgetItem* messageItemPtr = new QTreeWidgetItem;

		messageItemPtr->setData(0, DR_TASK_INDEX, taskIndex);
		messageItemPtr->setData(0, DR_SHAPE_INDICES, shapeIndices);
		messageItemPtr->setData(0, DR_SHAPE_POINTERS, shapePointers);
		messageItemPtr->setData(0, DR_CATEGORY, category);

		messageItemPtr->setIcon(0, GetCategoryLogIcon(category));

		QString sourceName = messagePtr->GetInformationSource();
		if (sourceName.isEmpty()){
			sourceName = tabName;
		}

		messageItemPtr->setText(0, "[" + sourceName + "] " + messagePtr->GetInformationDescription());

		if (isAuxiliary){
			if (auxiliaryItemPtr == NULL){
				auxiliaryItemPtr = new QTreeWidgetItem;

				QList<QVariant> auxGroupIndices;
				auxGroupIndices << -2;
				auxiliaryItemPtr->setData(0, DR_TASK_INDEX, taskIndex);
				auxiliaryItemPtr->setData(0, DR_SHAPE_INDICES, auxGroupIndices);
				auxiliaryItemPtr->setData(0, DR_CATEGORY, 0);

				auxiliaryItemPtr->setText(0, "[" + tabName + "] " + tr("Auxiliary Output"));

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

	MessageList->setUpdatesEnabled(false);

	MessageList->clear();
	m_resultShapesMap.clear();

	iinsp::IInspectionTask* taskPtr = GetObservedObject();

	if (taskPtr != NULL){
		int subtasksCount = taskPtr->GetSubtasksCount();
		for (int subTaskIndex = subtasksCount-1; subTaskIndex >= 0; subTaskIndex--){
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

	// #11236: sort the messages so the diagnostic ones go down
	QList<QTreeWidgetItem*> lowItems;
	for (int i = MessageList->topLevelItemCount() - 1; i >= 0; i--){
		auto mgs = MessageList->topLevelItem(i);
		int category = mgs->data(0, DR_CATEGORY).toInt();
		if (category <= 0){
			lowItems.append(mgs);
			MessageList->takeTopLevelItem(i);
		}
	}
	if (lowItems.size())
		MessageList->addTopLevelItems(lowItems);

	// Activate task related shapes:
	ActivateTaskShapes(m_currentGuiIndex, ShapeIndices(), false);

	// a trick: let the header remain interactive, scrollable if needed, and fitting to the full width available
	MessageList->header()->setSectionResizeMode(0, QHeaderView::Interactive);
	MessageList->header()->resizeSections(QHeaderView::ResizeToContents);
	if (MessageList->header()->sectionSize(0) < MessageList->header()->width())
		MessageList->header()->resizeSections(QHeaderView::Stretch);

	MessageList->setUpdatesEnabled(true);
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

			// remove listener
			if (viewPtr){
				viewPtr->RemoveViewEventObserver(this);
			}
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

		// add listener
		viewPtr->AddViewEventObserver(this);
	}

	// Update commands (#10853)
	if (m_taskCommandsDisplayerPtr.IsValid()){
		ibase::ICommandsProvider* commandsPtr = nullptr;
		if (m_commandsProvidersCompPtr.IsValid()){
			int commandCount = m_commandsProvidersCompPtr.GetCount();
			if (taskIndex >= 0 && taskIndex < commandCount){
				commandsPtr = m_commandsProvidersCompPtr[taskIndex];
			}
		}

		m_taskCommandsDisplayerPtr->ShowCommands(commandsPtr);
	}

	// Output current task index
	if (m_currentTaskIndexOutputPtr.IsValid()) {
		m_currentTaskIndexOutputPtr->SetSelectedOptionIndex(taskIndex);
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
	QMenu* actionsMenuPtr = new QMenu(MenuButton);
	MenuButton->setMenu(actionsMenuPtr);

	m_copyAllActionPtr = actionsMenuPtr->addAction(GetIcon(":/Icons/Copy"), tr("Copy all"), this, SLOT(OnCopyAll()));
	m_pasteAllActionPtr = actionsMenuPtr->addAction(GetIcon(":/Icons/Paste"), tr("Paste all"), this, SLOT(OnPasteAll()));
	actionsMenuPtr->addSeparator();
	m_copyCurrentTaskActionPtr = actionsMenuPtr->addAction(GetIcon(":/Icons/Copy"), tr("Copy current task"), this, SLOT(OnCopyCurrent()));
	m_pasteCurrentTaskActionPtr = actionsMenuPtr->addAction(GetIcon(":/Icons/Paste"), tr("Paste current task"), this, SLOT(OnPasteCurrent()));

	if (m_paramsLoaderCompPtr.IsValid()){
		actionsMenuPtr->addSeparator();
		m_loadAllActionPtr = actionsMenuPtr->addAction(GetIcon(":/Icons/Open"), tr("Load..."), this, SLOT(OnLoadParams()));
		m_saveAllActionPtr = actionsMenuPtr->addAction(GetIcon(":/Icons/Save"), tr("Save..."), this, SLOT(OnSaveParams()));
	}

	UpdateMenu();
}


void CInspectionTaskGuiComp::UpdateMenu()
{
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


} // namespace iqtinsp


