#include <icmpstr/CRegistryTreeViewComp.h>


// Qt includes
#include <QtCore/QDir>
#if QT_VERSION >= 0x050000
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QDialog>
#else
#include <QtGui/QHeaderView>
#include <QtGui/QListWidget>
#include <QtGui/QDialog>
#endif

// ACF includes
#include <icomp/CCompositeComponentStaticInfo.h>
#include <icomp/CCompositePackageStaticInfo.h>
#include <icomp/CComponentMetaDescriptionEncoder.h>
#include <ilog/CMessageContainer.h>
#include <iwidgets/CItemDelegate.h>
#include <GeneratedFiles/icmpstr/ui_CComponentStatisticsView.h>


namespace icmpstr
{


class StatisticsDialog: public QDialog, public Ui::CComponentStatisticsView
{
public:
	StatisticsDialog()
	{
		setupUi(this);
	}
};


class ComparableItem: public QTreeWidgetItem
{
public:
	virtual bool operator<(const QTreeWidgetItem& item) const override
	{
		int sortColumnIndex = item.treeWidget()->sortColumn();

		if (sortColumnIndex > 0){
			double value = item.text(sortColumnIndex).toDouble();
			double myValue = text(sortColumnIndex).toDouble();

			return myValue < value;
		}
		
		return QTreeWidgetItem::operator<(item);
	}
};


CRegistryTreeViewComp::CRegistryTreeViewComp()
	:m_environmentObserver(this)
{
	m_selectionInfo.SetParent(this);
}


void CRegistryTreeViewComp::AddSubcomponents(
			const icomp::IRegistry& registry,
			const icomp::CComponentAddress& address,
			QTreeWidgetItem* registryElementItemPtr)
{
	if (m_envManagerCompPtr.IsValid()){
		const icomp::IRegistry* componentRegistryPtr = NULL;

		if (!address.GetPackageId().isEmpty()){
			const icomp::IComponentStaticInfo* metaInfoPtr = m_envManagerCompPtr->GetComponentMetaInfo(address);

			if (metaInfoPtr != NULL &&(metaInfoPtr->GetComponentType() == icomp::IComponentStaticInfo::CT_COMPOSITE)){
				const icomp::CCompositeComponentStaticInfo* compositeMetaInfoPtr = dynamic_cast<const icomp::CCompositeComponentStaticInfo*>(metaInfoPtr);
				if (compositeMetaInfoPtr != NULL){
					componentRegistryPtr = &compositeMetaInfoPtr->GetRegistry();
				}
			}
		}
		else{
			componentRegistryPtr = registry.GetEmbeddedRegistry(address.GetComponentId());
		}

		if (componentRegistryPtr != NULL){
			CreateRegistryTree(*componentRegistryPtr, registryElementItemPtr);
		}
	}
}


void CRegistryTreeViewComp::CreateRegistryTree(const icomp::IRegistry& registry, QTreeWidgetItem* registryRootItemPtr)
{
	icomp::IRegistry::Ids elementIds = registry.GetElementIds();
	for (		icomp::IRegistry::Ids::iterator iter = elementIds.begin();
				iter != elementIds.end();
				++iter){
		const QByteArray& elementId = *iter;
		const icomp::IRegistry::ElementInfo* elementInfoPtr = registry.GetElementInfo(elementId);
		if ((elementInfoPtr != NULL) && elementInfoPtr->elementPtr.IsValid()){
			AddRegistryElementItem(registry, elementInfoPtr, elementId, registryRootItemPtr);
		}
	}
}


// protected methods

QTreeWidgetItem* CRegistryTreeViewComp::AddRegistryElementItem(
			const icomp::IRegistry& registry,
			const icomp::IRegistry::ElementInfo* elementPtr,
			const QByteArray& elementId,
			QTreeWidgetItem* parentItemPtr)
{
	icomp::CRegistryElement* registryElementPtr = dynamic_cast<icomp::CRegistryElement*>(elementPtr->elementPtr.GetPtr());
	if (registryElementPtr != NULL){
		QTreeWidgetItem* elementItemPtr = new QTreeWidgetItem();

		QByteArray componentId = elementPtr->address.GetComponentId();
		if (!elementPtr->address.GetPackageId().isEmpty()){
			const icomp::IComponentStaticInfo* metaInfoPtr = m_envManagerCompPtr->GetComponentMetaInfo(elementPtr->address);

			if (metaInfoPtr != NULL &&(metaInfoPtr->GetComponentType() == icomp::IComponentStaticInfo::CT_REAL)){
				Q_ASSERT(!componentId.isEmpty());
				m_componentHistogram[componentId]++;

				icomp::CComponentMetaDescriptionEncoder encoder(metaInfoPtr->GetKeywords());

				QStringList projectInfoList = encoder.GetValues("Project");
				for (int i = 0; i < projectInfoList.count(); ++i){
					m_projectHistogram[projectInfoList[i]]++;
				}
			}
		}

		QByteArray packageId =  elementPtr->address.GetPackageId();
		if (!elementPtr->address.GetPackageId().isEmpty()){
			const icomp::IComponentStaticInfo* metaInfoPtr = m_envManagerCompPtr->GetPackageMetaInfo(packageId);
			if (metaInfoPtr != NULL){
				const icomp::CCompositePackageStaticInfo* compositeMetaInfoPtr = dynamic_cast<const icomp::CCompositePackageStaticInfo*>(metaInfoPtr);

				if (compositeMetaInfoPtr == NULL){
					m_packageHistogram[packageId]++;
				}
			}
		}

		elementItemPtr->setText(CT_NAME, elementId);
		elementItemPtr->setData(CT_NAME, DR_ELEMENT_NAME, elementId);
		elementItemPtr->setData(CT_NAME, DR_ELEMENT_ID, componentId);
		elementItemPtr->setData(CT_NAME, DR_ELEMENT_PACKAGE_ID, packageId);
		elementItemPtr->setData(CT_NAME, DR_REGISTRY, quintptr(&registry));
		elementItemPtr->setText(CT_ID, componentId);
		elementItemPtr->setText(CT_PACKAGE, packageId);

		static QIcon okIcon(":/Icons/Ok");
		elementItemPtr->setIcon(CT_NAME, okIcon);

		if (parentItemPtr != NULL){
			parentItemPtr->addChild(elementItemPtr);
			parentItemPtr->setExpanded(true);
		}
		
		bool isConsistent = true;
		ilog::CMessageContainer messageContainer;

		if (m_consistInfoCompPtr.IsValid()){
			isConsistent = m_consistInfoCompPtr->IsElementValid(
						elementId,
						registry,
						false,
						true,
						&messageContainer);
		}

		static QIcon errorIcon(":/Icons/Warning");

		if (!isConsistent){
			elementItemPtr->setIcon(CT_NAME, errorIcon);

			ilog::CMessageContainer::Messages messages = messageContainer.GetMessages();
			QString messageText;
			for (ilog::CMessageContainer::Messages::iterator it = messages.begin(); it != messages.end(); it++) {
				messageText += (*it)->GetInformationDescription();
				messageText += '\n';
			}

			elementItemPtr->setData(CT_NAME, DR_MESSAGE_LIST, messageText);

			while (parentItemPtr != NULL){
				parentItemPtr->setIcon(CT_NAME, errorIcon);

				if (parentItemPtr->parent() == NULL){
					SetStatusIcon(errorIcon);
				}

				QString parentMessages = parentItemPtr->data(CT_NAME, DR_MESSAGE_LIST).toString();
				parentItemPtr->setData(CT_NAME, DR_MESSAGE_LIST, parentMessages + messageText);

				parentItemPtr = parentItemPtr->parent();
			}
		}

		AddSubcomponents(registry, elementPtr->address, elementItemPtr);

		return elementItemPtr;
	}

	return NULL;
}


bool CRegistryTreeViewComp::IsRegistryValid(const icomp::IRegistry& registry) const
{
	bool retVal = true;

	icomp::IRegistry::Ids elementIds = registry.GetElementIds();
	for (		icomp::IRegistry::Ids::iterator iter = elementIds.begin();
				iter != elementIds.end();
				++iter){
		const QByteArray& elementId = *iter;
		const icomp::IRegistry::ElementInfo* elementInfoPtr = registry.GetElementInfo(elementId);
		if ((elementInfoPtr != NULL) && elementInfoPtr->elementPtr.IsValid()){
			icomp::CRegistryElement* registryElementPtr = dynamic_cast<icomp::CRegistryElement*>(elementInfoPtr->elementPtr.GetPtr());
			if (registryElementPtr != NULL){
				bool isConsistent = true;

				if (m_consistInfoCompPtr.IsValid()){
					isConsistent = m_consistInfoCompPtr->IsElementValid(
								elementId,
								registry,
								false,
								true,
								NULL);
				}

				if (!isConsistent){
					return false;
				}

				if (m_envManagerCompPtr.IsValid()){
					const icomp::IComponentStaticInfo* metaInfoPtr = m_envManagerCompPtr->GetComponentMetaInfo(elementInfoPtr->address);

					if (metaInfoPtr != NULL &&(metaInfoPtr->GetComponentType() == icomp::IComponentStaticInfo::CT_COMPOSITE)){
						const icomp::CCompositeComponentStaticInfo* compositeMetaInfoPtr = dynamic_cast<const icomp::CCompositeComponentStaticInfo*>(metaInfoPtr);
						if (compositeMetaInfoPtr != NULL){
							const icomp::IRegistry& subRegistry = compositeMetaInfoPtr->GetRegistry();

							retVal = retVal && IsRegistryValid(subRegistry);
						}
					}
				}
			}
		}
	}

	return retVal;
}


void CRegistryTreeViewComp::UpdateRegistryStatus()
{
	const icomp::IRegistry* registryPtr = GetObservedObject();
	if (registryPtr != NULL){		
		bool isValid = IsRegistryValid(*registryPtr);
			
		static QIcon errorIcon(":/Icons/Warning");
		static QIcon okIcon(":/Icons/Ok");

		SetStatusIcon(isValid ? okIcon : errorIcon);
	}
}


void CRegistryTreeViewComp::UpdateTreeItemsVisibility()
{
	QString filterText = FilterEdit->text();
	bool showOnlyErrors = ShowOnlyErrorsCheck->isChecked();

	QTreeWidgetItemIterator treeIter(RegistryTree);

	while (*treeIter != NULL){
		QTreeWidgetItem* itemPtr = *treeIter;

		bool showItem = true;

		QString itemName = itemPtr->text(CT_NAME);
		QString itemId = itemPtr->text(CT_ID);
		QString packageId = itemPtr->text(CT_PACKAGE);

		if (		filterText.isEmpty() || 
					itemName.contains(filterText, Qt::CaseInsensitive) || 
					itemId.contains(filterText, Qt::CaseInsensitive) || 
					packageId.contains(filterText, Qt::CaseInsensitive)){
			showItem = true;
		}
		else{
			showItem = false;
		}

		QString messageText = itemPtr->data(CT_NAME, DR_MESSAGE_LIST).toString();
		if (messageText.isEmpty() && showOnlyErrors){
			showItem = false;
		}

		itemPtr->setHidden(!showItem);

		if (showItem){
			QTreeWidgetItem* parentItemPtr = itemPtr->parent();

			while (parentItemPtr != NULL){ 
				if (parentItemPtr->isHidden()){
					parentItemPtr->setHidden(false);
				}

				parentItemPtr = parentItemPtr->parent();
			}
		}

		treeIter++;
	}
}


// reimplemented (iqtgui::TGuiObserverWrap)

void CRegistryTreeViewComp::UpdateGui(const istd::IChangeable::ChangeSet& /*changeSet*/)
{
	RegistryTree->clear();

	// Clear statistics:
	m_componentHistogram.clear();
	m_packageHistogram.clear();
	m_projectHistogram.clear();

	QTreeWidgetItem* rootItem = new QTreeWidgetItem();
	rootItem->setText(0, "Root");

	static QIcon okIcon(":/Icons/Ok");

	rootItem->setIcon(CT_NAME, okIcon);

	SetStatusIcon(okIcon);

	RegistryTree->addTopLevelItem(rootItem);

	icomp::IRegistry* registryPtr = GetObservedObject();
	if (registryPtr != NULL){
		CreateRegistryTree(*registryPtr, rootItem);
	}

	rootItem->setSelected(true);
}


void CRegistryTreeViewComp::OnGuiModelDetached()
{
	RegistryTree->clear();

	BaseClass::OnGuiModelDetached();
}


// reimplemented (iqtgui::CGuiComponentBase)

void CRegistryTreeViewComp::OnGuiCreated()
{
	BaseClass::OnGuiCreated();

	QList<int> sizes = MainSplitter->sizes();
	sizes[0] = 0;

	MainSplitter->setSizes(sizes);

#if QT_VERSION < 0x050000
	RegistryTree->header()->setResizeMode(QHeaderView::Stretch);
#else
	RegistryTree->header()->setSectionResizeMode(QHeaderView::Stretch);
#endif
}


// reimplemented (imod::CSingleModelObserverBase)

void CRegistryTreeViewComp::AfterUpdate(imod::IModel* modelPtr, const istd::IChangeable::ChangeSet& changeSet)
{
	UpdateRegistryStatus();
	
	BaseClass::AfterUpdate(modelPtr, changeSet);
}


// reimplemented (icomp::CComponentBase)

void CRegistryTreeViewComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	if (m_envManagerModelCompPtr.IsValid()){
		m_envManagerModelCompPtr->AttachObserver(&m_environmentObserver);
	}
}


void CRegistryTreeViewComp::OnComponentDestroyed()
{
	if (m_envManagerModelCompPtr.IsValid() && m_envManagerModelCompPtr->IsAttached(&m_environmentObserver)){
		m_envManagerModelCompPtr->DetachObserver(&m_environmentObserver);
	}

	BaseClass::OnComponentDestroyed();
}


// protected slots

void CRegistryTreeViewComp::on_RegistryTree_itemSelectionChanged()
{
	istd::CChangeNotifier changePtr(&m_selectionInfo);

	m_selectedElements.selectedElementIds.clear();
	m_selectedElements.registryPtr = NULL;

	QList<QTreeWidgetItem*> selectedItems = RegistryTree->selectedItems();

	int itemsCount = selectedItems.size();
	
	for (int itemIndex = 0; itemIndex < itemsCount; itemIndex++){
		QTreeWidgetItem* itemPtr = selectedItems.at(itemIndex);
		Q_ASSERT(itemPtr != NULL);

		QString messageText = itemPtr->data(CT_NAME, DR_MESSAGE_LIST).toString();

		m_selectedElements.selectedElementIds.insert(itemPtr->data(CT_NAME, DR_ELEMENT_ID).toByteArray());

		MessagesList->setPlainText(messageText);

		m_selectedElements.selectedElementIds.insert(itemPtr->data(CT_NAME, DR_ELEMENT_ID).toByteArray());
		quintptr registryPointerAddress = itemPtr->data(CT_NAME, DR_REGISTRY).value<quintptr>();
		m_selectedElements.registryPtr = reinterpret_cast<icomp::IRegistry*>(registryPointerAddress);
	}

	// Close message view of no messages were provded:
	QList<int> sizes = MainSplitter->sizes();

	if (MessagesList->toPlainText().isEmpty()){
		sizes[1] = 0;
	}
	else{
		sizes[1] = sizes[0] / 4;
	}

	MainSplitter->setSizes(sizes);
}


void CRegistryTreeViewComp::on_RegistryTree_itemDoubleClicked(QTreeWidgetItem* itemPtr, int /*column*/)
{
	icomp::CComponentAddress componentAddress;

	componentAddress.SetComponentId(itemPtr->data(CT_NAME, DR_ELEMENT_ID).toByteArray());
	componentAddress.SetPackageId(itemPtr->data(CT_NAME, DR_ELEMENT_PACKAGE_ID).toByteArray());
	
	if (m_envManagerCompPtr.IsValid() && m_documentManagerCompPtr.IsValid()){
		const icomp::IComponentStaticInfo* metaInfoPtr = m_envManagerCompPtr->GetComponentMetaInfo(componentAddress);

		if (metaInfoPtr != NULL &&(metaInfoPtr->GetComponentType() == icomp::IComponentStaticInfo::CT_COMPOSITE)){
			QDir packageDir(m_envManagerCompPtr->GetPackagePath(componentAddress.GetPackageId()));
		
			QByteArray componentId = componentAddress.GetComponentId();

			QString filePath = packageDir.absoluteFilePath(componentAddress.GetComponentId() + ".acc");
			QString filePathOld = packageDir.absoluteFilePath(componentId + ".arx");
			if (!QFileInfo(filePath).exists()){
				if (QFileInfo(filePathOld).exists()){
					filePath = filePathOld;	
				}
			}

			m_documentManagerCompPtr->OpenDocument(NULL, &filePath);
		}
	}
}


void CRegistryTreeViewComp::on_FilterEdit_textChanged(const QString& /*filterText*/)
{
	UpdateTreeItemsVisibility();
}


void CRegistryTreeViewComp::on_ShowOnlyErrorsCheck_stateChanged(int /*state*/)
{
	UpdateTreeItemsVisibility();
}


void CRegistryTreeViewComp::on_ShowStatisticsButton_clicked()
{
	StatisticsDialog dialog;

#if QT_VERSION < 0x050000
	dialog.ProjectsList->header()->setResizeMode(QHeaderView::ResizeToContents);
	dialog.PackageList->header()->setResizeMode(QHeaderView::ResizeToContents);
	dialog.ComponentList->header()->setResizeMode(QHeaderView::ResizeToContents);
#else
	dialog.ProjectsList->header()->setSectionResizeMode(QHeaderView::ResizeToContents);
	dialog.PackageList->header()->setSectionResizeMode(QHeaderView::ResizeToContents);
	dialog.ComponentList->header()->setSectionResizeMode(QHeaderView::ResizeToContents);
#endif

	dialog.ProjectsGroup->setTitle(QString(tr("Projects (%1)")).arg(m_projectHistogram.keys().count()));
	FillStatistics(m_projectHistogram, *dialog.ProjectsList);

	dialog.PackagesGroup->setTitle(QString(tr("Packages (%1)")).arg(m_packageHistogram.keys().count()));
	FillStatistics(m_packageHistogram, *dialog.PackageList);

	dialog.ComponentsGroup->setTitle(QString(tr("Components (%1)")).arg(m_componentHistogram.keys().count()));
	FillStatistics(m_componentHistogram, *dialog.ComponentList);

	dialog.exec();
}


// private methods

void CRegistryTreeViewComp::FillStatistics(const Histogram& dataHistogram, QTreeWidget& list) const
{
	int overallCount = 0;
	for (		Histogram::ConstIterator iter = dataHistogram.constBegin();
				iter != dataHistogram.constEnd();
				++iter){
		overallCount += iter.value();
	}

	for (		Histogram::ConstIterator iter = dataHistogram.constBegin();
				iter != dataHistogram.constEnd();
				++iter){
		ComparableItem* item = new ComparableItem;
		item->setText(0, iter.key());
		item->setText(1, QString::number(iter.value()));
		item->setText(2, QString::number(iter.value() / double(overallCount) * 100, 'f', 1));

		list.addTopLevelItem(item);
	}
}

// public methods of embedded class EnvironmentObserver

CRegistryTreeViewComp::EnvironmentObserver::EnvironmentObserver(CRegistryTreeViewComp* parentPtr)
:	m_parent(*parentPtr)
{
	Q_ASSERT(parentPtr != NULL);
}


// protected methods of embedded class EnvironmentObserver

// reimplemented (imod::TSingleModelObserverBase)

void CRegistryTreeViewComp::EnvironmentObserver::OnUpdate(const istd::IChangeable::ChangeSet& changeSet)
{
	m_parent.UpdateRegistryStatus();

	if (m_parent.IsGuiCreated()){
		m_parent.UpdateGui(changeSet);
	}
}


// public methods of embedded class SelectionInfoImpl

void CRegistryTreeViewComp::SelectionInfoImpl::SetParent(CRegistryTreeViewComp* parentPtr)
{
	m_parentPtr = parentPtr;
}


// reimplemented (icmpstr::IElementSelectionInfo)

icomp::IRegistry* CRegistryTreeViewComp::SelectionInfoImpl::GetSelectedRegistry() const
{
	Q_ASSERT(m_parentPtr != NULL);	// parent should be set before any subelement can be accessed

	return m_parentPtr->m_selectedElements.registryPtr;
}


IElementSelectionInfo::Elements CRegistryTreeViewComp::SelectionInfoImpl::GetSelectedElements() const
{
	Q_ASSERT(m_parentPtr != NULL);	// parent should be set before any subelement can be accessed

	IElementSelectionInfo::Elements retVal;

	icomp::IRegistry* registryPtr = m_parentPtr->GetObservedObject();
	if (registryPtr != NULL){
		for (		ElementIds::const_iterator iter = m_parentPtr->m_selectedElements.selectedElementIds.begin();
					iter != m_parentPtr->m_selectedElements.selectedElementIds.end();
					++iter){
			const QByteArray& elementName = *iter;

			const icomp::IRegistry::ElementInfo* elementInfoPtr = registryPtr->GetElementInfo(elementName);
			if (elementInfoPtr != NULL){
				retVal[elementName] = elementInfoPtr;
			}
		}
	}

	return retVal;
}


} // namespace icmpstr


