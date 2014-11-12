#include "icmpstr/CComponentPromotorDialogComp.h"


// STL includes
#include <set>

// Qt includes
#include<QtCore/QtGlobal>
#if QT_VERSION >= 0x050000
#include <QtWidgets/QPushButton>
#else
#include <QtGui/QPushButton>
#endif

// ACF includes
#include "iser/CMemoryReadArchive.h"
#include "iser/CMemoryWriteArchive.h"

// ACF-Solutions includes
#include "icmpstr/CVisualRegistryElement.h"


namespace icmpstr
{


CComponentPromotorDialogComp::CComponentPromotorDialogComp()
{
	setupUi(this);

	DoRetranslate();

	m_promoteComponentCommand.setEnabled(false);
	m_promoteComponentCommand.SetGroupId(GI_COMPONENT);
	m_promoteComponentCommand.setShortcut(QKeySequence(Qt::CTRL + Qt::Key_F2));

	m_registryMenu.InsertChild(&m_promoteComponentCommand);
	m_rootMenuCommand.InsertChild(&m_registryMenu);

	connect(&m_promoteComponentCommand, SIGNAL(triggered()), this, SLOT(OnPromoteCommand()));
}


// reimpemented (ibase::ICommandsProvider)

const ibase::IHierarchicalCommand* CComponentPromotorDialogComp::GetCommands() const
{
	return &m_rootMenuCommand;
}


// protected methods

void CComponentPromotorDialogComp::InsertElementToFilters(
			const icomp::IRegistry& registry,
			const QByteArray& elementName,
			const icomp::IRegistry::ElementInfo& elementInfo)
{
	if (!m_metaInfoManagerCompPtr.IsValid()){
		return;
	}

	icomp::IRegistry::Ids elementIds = registry.GetElementIds();
	const icomp::IRegistry::ExportedInterfacesMap& exportedInterfacesMap = registry.GetExportedInterfacesMap();
	const icomp::IRegistry::ExportedElementsMap& exportedSubelementsMap = registry.GetExportedElementsMap();

	icomp::IElementStaticInfo::Ids elementInterfaces;

	const icomp::IComponentStaticInfo* staticInfoPtr = m_metaInfoManagerCompPtr->GetComponentMetaInfo(elementInfo.address);
	if (staticInfoPtr != NULL){
		elementInterfaces = staticInfoPtr->GetMetaIds(icomp::IElementStaticInfo::MGI_INTERFACES);
		m_supportedFilter.interfaces |= elementInterfaces;
		m_supportedFilter.attributes |= staticInfoPtr->GetAttributeMetaIds();
		m_supportedFilter.subcomponents |= staticInfoPtr->GetMetaIds(icomp::IComponentStaticInfo::MGI_SUBELEMENTS);

		m_supportedFilter.elements.insert(elementName);
	}

	if (elementInfo.elementPtr.IsValid()){
		// find set of exported interfaces
		for (		icomp::IRegistry::ExportedInterfacesMap::ConstIterator exportedInterfaceIter = exportedInterfacesMap.constBegin();
					exportedInterfaceIter != exportedInterfacesMap.constEnd();
					++exportedInterfaceIter){
			QByteArray baseId;
			QByteArray subId;
			istd::CIdManipBase::SplitId(exportedInterfaceIter.value(), baseId, subId);

			if (baseId == elementName){
				if (subId.isEmpty()){
					m_usedFilter.interfaces.insert(exportedInterfaceIter.key());
				}
				else{
					m_usedFilter.subcomponents.insert(subId);
				}
			}
		}

		// find interfaces used by connections in current registry
		for (		icomp::IRegistry::Ids::ConstIterator elementIter = elementIds.constBegin();
					elementIter != elementIds.constEnd();
					++elementIter){
			const QByteArray& checkedId = *elementIter;

			const icomp::IRegistry::ElementInfo* checkedInfoPtr = registry.GetElementInfo(checkedId);
			Q_ASSERT(checkedInfoPtr != NULL);
			Q_ASSERT(checkedInfoPtr->elementPtr.IsValid());

			const icomp::IComponentStaticInfo* checkedStaticInfoPtr = m_metaInfoManagerCompPtr->GetComponentMetaInfo(checkedInfoPtr->address);
			if (checkedStaticInfoPtr != NULL){
				iattr::IAttributesProvider::AttributeIds checkedAttributes = checkedInfoPtr->elementPtr->GetAttributeIds();
				for (		iattr::IAttributesProvider::AttributeIds::ConstIterator checkedAttributeIter = checkedAttributes.constBegin();
							checkedAttributeIter != checkedAttributes.constEnd();
							++checkedAttributeIter){
					const QByteArray& checkedAttributeId = *checkedAttributeIter;

					const iser::IObject* checkedAttributePtr = checkedInfoPtr->elementPtr->GetAttribute(checkedAttributeId);
					const icomp::IAttributeStaticInfo* checkedAttrStaticInfoPtr = checkedStaticInfoPtr->GetAttributeInfo(checkedAttributeId);
					if ((checkedAttributePtr != NULL) && (checkedAttrStaticInfoPtr != NULL)){
						icomp::IElementStaticInfo::Ids attrRelatedInterfaces = checkedAttrStaticInfoPtr->GetRelatedMetaIds(icomp::IComponentStaticInfo::MGI_INTERFACES, 0, 0);

						const iattr::CIdAttribute* referenceAttrPtr = dynamic_cast<const iattr::CIdAttribute*>(checkedAttributePtr);
						if (referenceAttrPtr != NULL){
							const QByteArray connectedElementName = referenceAttrPtr->GetValue();
							QByteArray baseId;
							QByteArray subId;
							istd::CIdManipBase::SplitId(connectedElementName, baseId, subId);
							if (baseId == elementName){
								if (subId.isEmpty()){
									m_usedFilter.interfaces |= attrRelatedInterfaces & elementInterfaces;
								}
								else{
									m_usedFilter.subcomponents.insert(subId);
								}
							}
						}

						const iattr::CIdListAttribute* multiReferenceAttrPtr = dynamic_cast<const iattr::CIdListAttribute*>(checkedAttributePtr);
						if (multiReferenceAttrPtr != NULL){
							int referencesCount = multiReferenceAttrPtr->GetValuesCount();
							for (int i = 0; i < referencesCount; ++i){
								const QByteArray connectedElementName = multiReferenceAttrPtr->GetValueAt(i);
								QByteArray baseId;
								QByteArray subId;
								istd::CIdManipBase::SplitId(connectedElementName, baseId, subId);
								if (baseId == elementName){
									if (subId.isEmpty()){
										m_usedFilter.interfaces |= attrRelatedInterfaces & elementInterfaces;
									}
									else{
										m_usedFilter.subcomponents.insert(subId);
									}
								}
							}
						}
					}
				}
			}
		}

		// get names of all used attributes
		m_usedFilter.attributes |= elementInfo.elementPtr->GetAttributeIds();

		// find set of exported subcomponents
		for (		icomp::IRegistry::ExportedElementsMap::ConstIterator exportedElementIter = exportedSubelementsMap.constBegin();
					exportedElementIter != exportedSubelementsMap.constEnd();
					++exportedElementIter){
			QByteArray baseId;
			QByteArray subId;
			istd::CIdManipBase::SplitId(exportedElementIter.value(), baseId, subId);

			if ((baseId == elementName) && !subId.isEmpty()){
				m_usedFilter.subcomponents.insert(subId);
			}
		}

		m_usedFilter.elements.insert(elementName);
	}
}


void CComponentPromotorDialogComp::CreatePackagesList()
{
	QString packageName = PackageNameCB->currentText();

	PackageNameCB->clear();
	if (!m_metaInfoManagerCompPtr.IsValid()){
		return;
	}

	std::set<QByteArray> ids;	// STL used due of sorting

	for (		icomp::IMetaInfoManager::ComponentAddresses::ConstIterator iter = m_filteredComponents.constBegin();
				iter != m_filteredComponents.constEnd();
				++iter){
		const icomp::CComponentAddress& address = *iter;

		ids.insert(address.GetPackageId());
	}

	for (		std::set<QByteArray>::const_iterator iter = ids.begin();
				iter != ids.end();
				++iter){
		PackageNameCB->addItem(*iter);
	}

	int packageIndex = PackageNameCB->findText(packageName);
	if (packageIndex >= 0){
		PackageNameCB->setCurrentIndex(packageIndex);
	}

	CreateComponentsList();
}


void CComponentPromotorDialogComp::CreateComponentsList()
{
	QString componentName = ComponentNameCB->currentText();

	ComponentNameCB->clear();

	if (!m_metaInfoManagerCompPtr.IsValid()){
		return;
	}

	QByteArray packageId = PackageNameCB->currentText().toUtf8();
	std::set<QByteArray> ids;	// STL used due of sorting

	for (		icomp::IMetaInfoManager::ComponentAddresses::ConstIterator iter = m_filteredComponents.constBegin();
				iter != m_filteredComponents.constEnd();
				++iter){
		const icomp::CComponentAddress& address = *iter;

		if (address.GetPackageId() == packageId){
			ids.insert(address.GetComponentId());
		}
	}

	for (		std::set<QByteArray>::const_iterator iter = ids.begin();
				iter != ids.end();
				++iter){
		ComponentNameCB->addItem(*iter);
	}

	int componentIndex = ComponentNameCB->findText(componentName);
	if (componentIndex >= 0){
		ComponentNameCB->setCurrentIndex(componentIndex);
	}
}


void CComponentPromotorDialogComp::CalcFilteredComponents()
{
	m_filteredComponents.clear();

	if (!m_metaInfoManagerCompPtr.IsValid()){
		return;
	}

	icomp::IMetaInfoManager::ComponentAddresses addresses = m_metaInfoManagerCompPtr->GetComponentAddresses();
	for (		icomp::IMetaInfoManager::ComponentAddresses::ConstIterator addressIter = addresses.constBegin();
				addressIter != addresses.constEnd();
				++addressIter){
		const icomp::CComponentAddress& address = *addressIter;

		const icomp::IComponentStaticInfo* staticInfoPtr = m_metaInfoManagerCompPtr->GetComponentMetaInfo(address);
		if (staticInfoPtr != NULL){
			if (InterfacesPolicyCB->isEnabled() && (InterfacesPolicyCB->currentIndex() > FT_IGNORED)){
				icomp::IElementStaticInfo::Ids interfaces = staticInfoPtr->GetMetaIds(icomp::IElementStaticInfo::MGI_INTERFACES);

				switch (InterfacesPolicyCB->currentIndex()){
				case FT_USED:
					if (!interfaces.contains(m_usedFilter.interfaces)){
						continue;
					}
					break;

				default:
					if (!interfaces.contains(m_supportedFilter.interfaces)){
						continue;
					}
					break;
				}
			}

			if (AttributesPolicyCB->isEnabled() && (AttributesPolicyCB->currentIndex() > FT_IGNORED)){
				iattr::IAttributesProvider::AttributeIds attributes = staticInfoPtr->GetAttributeMetaIds();

				switch (AttributesPolicyCB->currentIndex()){
				case FT_USED:
					if (!attributes.contains(m_usedFilter.attributes)){
						continue;
					}
					break;

				default:
					if (!attributes.contains(m_supportedFilter.attributes)){
						continue;
					}
					break;
				}
			}

			if (SubcomponentsPolicyCB->isEnabled() && (SubcomponentsPolicyCB->currentIndex() > FT_IGNORED)){
				icomp::IElementStaticInfo::Ids subcomponents = staticInfoPtr->GetMetaIds(icomp::IComponentStaticInfo::MGI_SUBELEMENTS);

				switch (SubcomponentsPolicyCB->currentIndex()){
				case FT_USED:
					if (!subcomponents.contains(m_usedFilter.subcomponents)){
						continue;
					}
					break;

				default:
					if (!subcomponents.contains(m_supportedFilter.subcomponents)){
						continue;
					}
					break;
				}
			}

			m_filteredComponents.insert(address);
		}
	}
}


void CComponentPromotorDialogComp::DoRetranslate()
{
	m_registryMenu.SetVisuals(
				tr("&Registry"),
				tr("Registry"),
				tr("Set of commands manipulating registry"));
	m_promoteComponentCommand.SetVisuals(
				tr("&Promote component"), 
				tr("Promote"), 
				tr("Promote component to another one"));
}


// reimplemented (imod::CSingleModelObserverBase)

void CComponentPromotorDialogComp::OnUpdate(const istd::IChangeable::ChangeSet& changeSet)
{
	BaseClass::OnUpdate(changeSet);

	const IElementSelectionInfo* objectPtr = GetObjectPtr();

	m_promoteComponentCommand.SetEnabled(
				(objectPtr != NULL) &&
				!objectPtr->GetSelectedElements().isEmpty());
}


// protected slots

void CComponentPromotorDialogComp::on_InterfacesPolicyCB_currentIndexChanged(int /*index*/)
{
	CalcFilteredComponents();
	CreatePackagesList();
}


void CComponentPromotorDialogComp::on_AttributesPolicyCB_currentIndexChanged(int /*index*/)
{
	CalcFilteredComponents();
	CreatePackagesList();
}


void CComponentPromotorDialogComp::on_SubcomponentsPolicyCB_currentIndexChanged(int /*index*/)
{
	CalcFilteredComponents();
	CreatePackagesList();
}


void CComponentPromotorDialogComp::on_PackageNameCB_currentIndexChanged(int /*index*/)
{
	CreateComponentsList();
}


void CComponentPromotorDialogComp::on_ComponentNameCB_currentIndexChanged(int index)
{
	ButtonBox->button(QDialogButtonBox::Ok)->setEnabled(index >= 0);
}


void CComponentPromotorDialogComp::OnPromoteCommand()
{
	IElementSelectionInfo* selectionInfoPtr = GetObjectPtr();
	if (!m_metaInfoManagerCompPtr.IsValid() || (selectionInfoPtr == NULL)){
		return;
	}

	icomp::IRegistry* registryPtr = selectionInfoPtr->GetSelectedRegistry();
	if (registryPtr == NULL){
		return;
	}

	m_supportedFilter = Filter();
	m_usedFilter = Filter();

	IElementSelectionInfo::Elements elements = selectionInfoPtr->GetSelectedElements();
	for (		IElementSelectionInfo::Elements::ConstIterator iter = elements.constBegin();
				iter != elements.constEnd();
				++iter){
		const QByteArray& elementName = iter.key();
		const icomp::IRegistry::ElementInfo* elementInfoPtr = iter.value();
		Q_ASSERT(elementInfoPtr != NULL);

		InsertElementToFilters(*registryPtr, elementName, *elementInfoPtr);
	}

	InterfacesPolicyCB->setEnabled(!m_supportedFilter.interfaces.isEmpty() || !m_usedFilter.interfaces.isEmpty());
	AttributesPolicyCB->setEnabled(!m_supportedFilter.attributes.isEmpty() || !m_usedFilter.attributes.isEmpty());
	SubcomponentsPolicyCB->setEnabled(!m_supportedFilter.subcomponents.isEmpty() || !m_usedFilter.subcomponents.isEmpty());

	ButtonBox->button(QDialogButtonBox::Ok)->setEnabled(false);

	CalcFilteredComponents();
	CreatePackagesList();

	if (BaseClass3::exec() == QDialog::Accepted){
		icomp::CComponentAddress newAddress(
					PackageNameCB->currentText().toLatin1(),
					ComponentNameCB->currentText().toLatin1());

		static istd::IChangeable::ChangeSet registryChangeSet(icomp::IRegistry::CF_ELEMENT_REMOVED, icomp::IRegistry::CF_ELEMENT_ADDED);
		istd::CChangeNotifier registryNotifier(registryPtr, registryChangeSet);

		for (		IElementSelectionInfo::Elements::Iterator iter = elements.begin();
					iter != elements.end();
					++iter){
			const QByteArray& elementId = iter.key();
			const icomp::IRegistry::ElementInfo* elementInfoPtr = iter.value();
			Q_ASSERT(elementInfoPtr != NULL);
			Q_ASSERT(elementInfoPtr->elementPtr.IsValid());

			iser::CMemoryWriteArchive stateStoreArchive;
			if (!elementInfoPtr->elementPtr->Serialize(stateStoreArchive)){
				continue;
			}

			i2d::CVector2d storedPosition(0, 0);
			const icmpstr::CVisualRegistryElement* prevVisualElementPtr = dynamic_cast<const icmpstr::CVisualRegistryElement*>(elementInfoPtr->elementPtr.GetPtr());
			if (prevVisualElementPtr != NULL){
				storedPosition = prevVisualElementPtr->GetCenter();
			}

			registryPtr->RemoveElementInfo(elementId);
			icomp::IRegistry::ElementInfo* newElementInfoPtr = registryPtr->InsertElementInfo(elementId, newAddress, true);
			if ((newElementInfoPtr == NULL) || !newElementInfoPtr->elementPtr.IsValid()){
				continue;
			}

			iser::CMemoryReadArchive stateRestoreArchive(stateStoreArchive);

			newElementInfoPtr->elementPtr->Serialize(stateRestoreArchive);

			icmpstr::CVisualRegistryElement* newVisualElementPtr = dynamic_cast<icmpstr::CVisualRegistryElement*>(newElementInfoPtr->elementPtr.GetPtr());
			if ((newVisualElementPtr != NULL) && !storedPosition.IsNull()){
				newVisualElementPtr->MoveCenterTo(storedPosition);
			}
		}
	}
}


} // namespace icmpstr


