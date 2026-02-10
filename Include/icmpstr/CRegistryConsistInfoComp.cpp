// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#include <icmpstr/CRegistryConsistInfoComp.h>


// Qt includes
#include <QtCore/QDir>


// ACF includes
#include <icomp/CCompositeComponentStaticInfo.h>

#include <ilog/CMessage.h>


namespace icmpstr
{


// reimplemented (icmpstr::IRegistryConsistInfo)

icomp::IRegistry::Ids CRegistryConsistInfoComp::GetCompatibleElements(
			const icomp::IElementStaticInfo::Ids& interfaceNames,
			const icomp::IRegistry& registry,
			int queryFlags) const
{
	icomp::IRegistry::Ids retVal;

	bool includeUndefined = (queryFlags & QF_INCLUDE_UNDEFINED) != 0;

	if (m_envManagerCompPtr.IsValid()){
		bool isDetachedQuery = ((queryFlags & QF_DETACHED_FROM_CONTAINER) != 0);

		icomp::IRegistry::Ids elementIds = registry.GetElementIds();
		for (		icomp::IRegistry::Ids::const_iterator index = elementIds.begin();
					index != elementIds.end();
					++index){
			const QByteArray& elementId = *index;
			const icomp::IRegistry::ElementInfo* elementInfoPtr = registry.GetElementInfo(elementId);
			Q_ASSERT(elementInfoPtr != NULL);	// element ID was taken from this registry, it must exist
			Q_ASSERT(elementInfoPtr->elementPtr.IsValid());	// element ID was taken from this registry, it must exist

			int elementFlags = elementInfoPtr->elementPtr->GetElementFlags();
			bool isElementDetached = ((elementFlags & icomp::IRegistryElement::EF_IS_DETACHED) != 0);
			if (isElementDetached != isDetachedQuery){
				continue;
			}

			icomp::IRegistry::Ids subIds;

			const icomp::CComponentAddress& elementAddress = elementInfoPtr->address;
			if (!elementAddress.GetPackageId().isEmpty()){
				const icomp::IComponentStaticInfo* infoPtr = m_envManagerCompPtr->GetComponentMetaInfo(elementAddress);

				if (infoPtr != NULL){
					subIds = GetCompatibleIds(
								elementId,
								*infoPtr,
								interfaceNames,
								queryFlags);
				}
				else if (includeUndefined){
					retVal.insert(elementId);
				}
			}
			else{
				const icomp::IRegistry* embeddedRegistryPtr = registry.GetEmbeddedRegistry(elementAddress.GetComponentId());
				if (embeddedRegistryPtr == NULL){
					if (includeUndefined){
						retVal.insert(elementId);
					}

					continue;
				}

				icomp::CCompositeComponentStaticInfo info(*embeddedRegistryPtr, *m_envManagerCompPtr);
				subIds = GetCompatibleIds(
							elementId,
							info,
							interfaceNames,
							queryFlags);
			}

			retVal += subIds;
		}
	}

	return retVal;
}


bool CRegistryConsistInfoComp::IsRegistryValid(
			const icomp::IRegistry& registry,
			bool ignoreUndef,
			bool allReasons,
			ilog::IMessageConsumer* reasonConsumerPtr) const
{
	bool retVal = true;

	icomp::IRegistry::Ids ids = registry.GetElementIds();
	for (		icomp::IRegistry::Ids::const_iterator iter = ids.begin();
				iter != ids.end();
				++iter){
		const QByteArray& elementId = *iter;

		bool status = IsElementValid(elementId, registry, ignoreUndef, allReasons, reasonConsumerPtr);

		if (!status && !allReasons){
			return false;
		}

		retVal = retVal && status;
	}

	return retVal;
}


bool CRegistryConsistInfoComp::IsElementValid(
			const QByteArray& elementName,
			const icomp::IRegistry& registry,
			bool ignoreUndef,
			bool allReasons,
			ilog::IMessageConsumer* reasonConsumerPtr) const
{
	Q_ASSERT(m_envManagerCompPtr.IsValid());

	bool retVal = true;

	const icomp::IRegistry::ElementInfo* infoPtr = registry.GetElementInfo(elementName);
	if (infoPtr != NULL){
		const icomp::CComponentAddress& elementAddress = infoPtr->address;
		if (!elementAddress.GetPackageId().isEmpty()){
			retVal = IsElementWithInfoValid(
						elementName,
						*infoPtr,
						m_envManagerCompPtr->GetComponentMetaInfo(elementAddress),
						registry,
						ignoreUndef,
						allReasons,
						reasonConsumerPtr) && retVal;
		}
		else{
			icomp::IRegistry* embeddedRegistryPtr = registry.GetEmbeddedRegistry(elementAddress.GetComponentId());
			if (embeddedRegistryPtr != NULL){
				icomp::CCompositeComponentStaticInfo embeddedInfo(*embeddedRegistryPtr, *m_envManagerCompPtr);
				retVal = IsElementWithInfoValid(
							elementName,
							*infoPtr,
							&embeddedInfo,
							registry,
							ignoreUndef,
							allReasons,
							reasonConsumerPtr) && retVal;
			}
			else{
				if (reasonConsumerPtr != NULL){
					reasonConsumerPtr->AddMessage(ilog::IMessageConsumer::MessagePtr(new ilog::CMessage(
								istd::IInformationProvider::IC_WARNING,
								MI_COMPONENT_INACTIVE,
								tr("Element %1 uses unknown embedded composite component %2").arg(QString(elementName)).arg(QString(elementAddress.GetComponentId())),
								tr("Element Consistency Check"),
								0)));
				}

				retVal = false;
			}
		}

		if (!retVal && !allReasons){
			return false;
		}
	}
	else{
		if (reasonConsumerPtr != NULL){
			reasonConsumerPtr->AddMessage(ilog::IMessageConsumer::MessagePtr(new ilog::CMessage(
						istd::IInformationProvider::IC_ERROR,
						MI_NO_ELEMENT_INFO,
						tr("No element info available for %1").arg(QString(elementName)),
						tr("Element Consistency Check"),
						0)));
		}

		retVal = false;
	}

	return retVal;
}


bool CRegistryConsistInfoComp::IsElementWithInfoValid(
			const QByteArray& elementName,
			const icomp::IRegistry::ElementInfo& elementInfo,
			const icomp::IComponentStaticInfo* metaInfoPtr,
			const icomp::IRegistry& registry,
			bool ignoreUndef,
			bool allReasons,
			ilog::IMessageConsumer* reasonConsumerPtr) const
{
	bool retVal = true;

	if (metaInfoPtr != NULL){
		int componentType = metaInfoPtr->GetComponentType();
		if ((componentType != icomp::IComponentStaticInfo::CT_REAL) && (componentType != icomp::IComponentStaticInfo::CT_COMPOSITE)){
			if (reasonConsumerPtr != NULL){
				reasonConsumerPtr->AddMessage(ilog::IMessageConsumer::MessagePtr(new ilog::CMessage(
							istd::IInformationProvider::IC_WARNING,
							MI_COMPONENT_INACTIVE,
							tr("Element %1 uses wrong defined component %2").arg(QString(elementName)).arg(elementInfo.address.ToString()),
							tr("Element Consistency Check"),
							0)));
			}

			retVal = false;

			if (!allReasons){
				return false;
			}
		}

		iattr::IAttributesProvider::AttributeIds attributeIds = metaInfoPtr->GetAttributeMetaIds();
		for (		icomp::IElementStaticInfo::Ids::const_iterator attrIter = attributeIds.begin();
					attrIter != attributeIds.end();
					++attrIter){
			const QByteArray& attributeId = *attrIter;

			retVal = IsAttributeValid(attributeId, elementName, registry, ignoreUndef, allReasons, reasonConsumerPtr, metaInfoPtr) && retVal;

			if (!retVal && !allReasons){
				return false;
			}
		}
	}
	else{
		if (!ignoreUndef){
			if (reasonConsumerPtr != NULL){
				reasonConsumerPtr->AddMessage(ilog::IMessageConsumer::MessagePtr(new ilog::CMessage(
							istd::IInformationProvider::IC_WARNING,
							MI_COMPONENT_INACTIVE,
							tr("Element %1 uses inactive component %2").arg(QString(elementName)).arg(elementInfo.address.ToString()),
							tr("Element Consistency Check"),
							0)));
			}

			retVal = false;

			if (!allReasons){
				return false;
			}
		}
	}

	const icomp::IRegistryElement* elementPtr = elementInfo.elementPtr.GetPtr();
	if (elementPtr != NULL){
		if (metaInfoPtr == NULL){	// if no meta info, we try to check attributes from existing attributes list
			iattr::IAttributesProvider::AttributeIds ids = elementPtr->GetAttributeIds();
			for (		iattr::IAttributesProvider::AttributeIds::ConstIterator iter = ids.constBegin();
						iter != ids.constEnd();
						++iter){
				const QByteArray& attributeId = *iter;

				retVal = IsAttributeValid(attributeId, elementName, registry, ignoreUndef, allReasons, reasonConsumerPtr) && retVal;

				if (!retVal && !allReasons){
					return false;
				}
			}
		}
	}
	else{
		if (reasonConsumerPtr != NULL){
			reasonConsumerPtr->AddMessage(ilog::IMessageConsumer::MessagePtr(new ilog::CMessage(
						istd::IInformationProvider::IC_WARNING,
						MI_COMPONENT_INACTIVE,
						tr("Element %1 is not loaded").arg(QString(elementName)),
						tr("Element Consistency Check"),
						0)));
		}

		return false;
	}

	return retVal;
}


bool CRegistryConsistInfoComp::IsAttributeValid(
			const QByteArray& attributeName,
			const QByteArray& elementName,
			const icomp::IRegistry& registry,
			bool ignoreUndef,
			bool allReasons,
			ilog::IMessageConsumer* reasonConsumerPtr,
			const icomp::IComponentStaticInfo* componentMetaInfoPtr) const
{
	const icomp::IRegistry::ElementInfo* infoPtr = registry.GetElementInfo(elementName);
	if (infoPtr != NULL){
		const icomp::IComponentStaticInfo* compInfoPtr = componentMetaInfoPtr;
		if ((compInfoPtr == NULL) && m_envManagerCompPtr.IsValid()){
			compInfoPtr = m_envManagerCompPtr->GetComponentMetaInfo(infoPtr->address);
		}

		if (infoPtr->elementPtr.IsValid() && (compInfoPtr != NULL)){
			const icomp::IAttributeStaticInfo* attrMetaInfoPtr = compInfoPtr->GetAttributeInfo(attributeName);
			if (attrMetaInfoPtr != NULL){
				const icomp::IRegistryElement::AttributeInfo* attrInfoPtr = infoPtr->elementPtr->GetAttributeInfo(attributeName);
				if (attrInfoPtr != NULL){
					if (!AreTypesCompatible(attrMetaInfoPtr->GetAttributeTypeId(), attrInfoPtr->attributeTypeName)){
						if (reasonConsumerPtr != NULL){
							reasonConsumerPtr->AddMessage(ilog::IMessageConsumer::MessagePtr(new ilog::CMessage(
										istd::IInformationProvider::IC_ERROR,
										MI_BAD_ATTRIBUTE_TYPE,
										tr("Attribute %1 in %2 is defined as %3, but in registry it has type %4")
													.arg(QString(attributeName))
													.arg(QString(elementName))
													.arg(QString(attrMetaInfoPtr->GetAttributeTypeId()))
													.arg(QString(attrInfoPtr->attributeTypeName)),
										tr("Attribute Consistency Check"),
										0)));
						}

						return false;
					}

					if (attrInfoPtr->attributePtr.IsValid()){
						if (!CheckAttributeCompatibility(
									*attrInfoPtr->attributePtr,
									*attrMetaInfoPtr,
									attributeName,
									elementName,
									registry,
									ignoreUndef,
									allReasons,
									reasonConsumerPtr)){
							return false;
						}
					}
					else if (	attrInfoPtr->exportId.isEmpty() &&
								((attrMetaInfoPtr->GetAttributeFlags() & icomp::IAttributeStaticInfo::AF_NULLABLE) == 0)){
						if (reasonConsumerPtr != NULL){
							reasonConsumerPtr->AddMessage(ilog::IMessageConsumer::MessagePtr(new ilog::CMessage(
										istd::IInformationProvider::IC_ERROR,
										MI_REF_NOT_RESOLVED,
										tr("Reference or factory %1 in %2 cannot be undefined")
													.arg(QString(attributeName))
													.arg(QString(elementName)),
										tr("Attribute Consistency Check"),
										0)));
						}

						return false;
					}
				}
				else if ((attrMetaInfoPtr->GetAttributeFlags() & icomp::IAttributeStaticInfo::AF_NULLABLE) == 0){
					if (reasonConsumerPtr != NULL){
						reasonConsumerPtr->AddMessage(ilog::IMessageConsumer::MessagePtr(new ilog::CMessage(
									istd::IInformationProvider::IC_ERROR,
									MI_REF_NOT_RESOLVED,
									tr("Reference or factory %1 in %2 cannot be undefined")
												.arg(QString(attributeName))
												.arg(QString(elementName)),
									tr("Attribute Consistency Check"),
									0)));
					}

					return false;
				}
			}
			else if (!ignoreUndef){
				if (reasonConsumerPtr != NULL){
					reasonConsumerPtr->AddMessage(ilog::IMessageConsumer::MessagePtr(new ilog::CMessage(
								istd::IInformationProvider::IC_ERROR,
								MI_UNDEF_ATTRIBUTE,
								tr("Attribute %1 in %2 not exists in component specification")
											.arg(QString(attributeName))
											.arg(QString(elementName)),
								tr("Attribute Consistency Check"),
								0)));
				}

				return false;
			}
		}
	}
	else{
		if (reasonConsumerPtr != NULL){
			reasonConsumerPtr->AddMessage(ilog::IMessageConsumer::MessagePtr(new ilog::CMessage(
						istd::IInformationProvider::IC_ERROR,
						MI_NO_ELEMENT_INFO,
						tr("No element %1 found").arg(QString(elementName)),
						tr("Attribute Consistency Check"),
						0)));
		}

		return false;
	}

	return true;
}


bool CRegistryConsistInfoComp::IsAttributeValueValid(
			int attributeFlags,
			const QByteArray& attributeText,
			const icomp::IElementStaticInfo::Ids& interfaceNames,
			const icomp::IElementStaticInfo::Ids& optionalInterfaceNames,
			const icomp::IRegistry& registry,
			bool ignoreUndef,
			ilog::IMessageConsumer* reasonConsumerPtr) const
{
	if ((attributeFlags & (icomp::IAttributeStaticInfo::AF_FACTORY | icomp::IAttributeStaticInfo::AF_REFERENCE)) == 0){
		return true;
	}

	return CheckPointedElementCompatibility(
				attributeFlags,
				attributeText,
				interfaceNames,
				optionalInterfaceNames,
				registry,
				ignoreUndef,
				"",
				reasonConsumerPtr);
}


QIcon CRegistryConsistInfoComp::GetComponentIcon(const icomp::CComponentAddress& address) const
{
	if (m_externalMetaInfoManagerCompPtr.IsValid()){
		QString infoPath = m_externalMetaInfoManagerCompPtr->GetComponentInfoPath(address);
		if (!infoPath.isEmpty()){
			IconCache::ConstIterator cachedIter = m_iconCache.constFind(address);
			if (cachedIter != m_iconCache.constEnd()){
				return cachedIter.value();
			}
			else{
				QIcon& icon = m_iconCache[address];

				QDir packageDir(infoPath);
				if (packageDir.exists("Image.png")){
					icon = QIcon(packageDir.filePath("Image.png"));
				}
				else if (packageDir.exists("Image")){
					icon = QIcon(packageDir.filePath("Image"));
				}

				return icon;
			}
		}
	}

	return QIcon();
}


// static methods

bool CRegistryConsistInfoComp::AreTypesCompatible(const QByteArray& metaTypeId, const QByteArray& typeId)
{
	if (metaTypeId == typeId){
		return true;
	}

	if ((metaTypeId == iattr::CStringAttribute::GetTypeName()) && (typeId == icomp::CTextAttribute::GetTypeName())){
		return true;
	}

	if ((metaTypeId == iattr::CStringListAttribute::GetTypeName()) && (typeId == icomp::CMultiTextAttribute::GetTypeName())){
		return true;
	}

	return false;
}


// protected methods

icomp::IRegistry::Ids CRegistryConsistInfoComp::GetCompatibleIds(
			const QByteArray& elementId,
			const icomp::IElementStaticInfo& elementStaticInfo,
			const icomp::IElementStaticInfo::Ids& interfaceNames,
			int queryFlags) const
{
	icomp::IRegistry::Ids retVal;

	bool areInterfacesSupported = ((queryFlags & QF_ANY_INTERFACE) != 0)? false: true;

	for (		icomp::IElementStaticInfo::Ids::const_iterator interfaceIter = interfaceNames.begin();
				interfaceIter != interfaceNames.end();
				++interfaceIter){
		const QByteArray& interfaceName = *interfaceIter;
		istd::CClassInfo interfaceType(interfaceName);

		if (		interfaceType.IsVoid() ||
					(interfaceName == "icomp::IComponent") ||
					(interfaceName == "const icomp::IComponent") ||
					(interfaceName == "void") ||
					(interfaceName == "const void")){
			continue;
		}

		icomp::IElementStaticInfo::Ids supportedInterfaces = elementStaticInfo.GetMetaIds(icomp::IComponentStaticInfo::MGI_INTERFACES);

		QByteArray nonConstInterfaceName = interfaceType.GetConstCasted(false).GetName();
		bool isInterfaceCompatible =
					(supportedInterfaces.find(interfaceName) != supportedInterfaces.end()) ||
					(supportedInterfaces.find(nonConstInterfaceName) != supportedInterfaces.end());

		if (isInterfaceCompatible){
			if ((queryFlags & QF_ANY_INTERFACE) != 0){
				areInterfacesSupported = true;

				break;
			}
		}
		else{
			if ((queryFlags & QF_ANY_INTERFACE) == 0){
				areInterfacesSupported = false;

				break;
			}
		}
	}

	if (areInterfacesSupported){
		retVal.insert(elementId);
	}

	if ((queryFlags & QF_INCLUDE_SUBELEMENTS) != 0){
		const icomp::IElementStaticInfo::Ids subcomponentIds = elementStaticInfo.GetMetaIds(icomp::IComponentStaticInfo::MGI_SUBELEMENTS);

		for (		icomp::IElementStaticInfo::Ids::const_iterator subIter = subcomponentIds.begin();
					subIter != subcomponentIds.end();
					++subIter){
			const QByteArray& subcomponentId = *subIter;

			const icomp::IElementStaticInfo* subcomponentInfoPtr = elementStaticInfo.GetSubelementInfo(subcomponentId);
			if (subcomponentInfoPtr != NULL){
				icomp::IRegistry::Ids subIds = GetCompatibleIds(
							istd::CIdManipBase::JoinId(elementId, subcomponentId),
							*subcomponentInfoPtr,
							interfaceNames,
							queryFlags & ~QF_INCLUDE_SUBELEMENTS);

				retVal += subIds;
			}
		}
	}

	return retVal;
}


bool CRegistryConsistInfoComp::CheckAttributeCompatibility(
			const iser::IObject& attribute,
			const icomp::IAttributeStaticInfo& attributeMetaInfo,
			const QByteArray& attributeName,
			const QByteArray& elementName,
			const icomp::IRegistry& registry,
			bool ignoreUndef,
			bool allReasons,
			ilog::IMessageConsumer* reasonConsumerPtr) const
{
	int attributeFlags = attributeMetaInfo.GetAttributeFlags();

	QString messagePrefix = tr("%1 in %2: ").arg(QString(attributeName)).arg(QString(elementName));

	const icomp::CReferenceAttribute* idPtr = dynamic_cast<const icomp::CReferenceAttribute*>(&attribute);
	if (idPtr != NULL){
		icomp::IElementStaticInfo::Ids interfaceNames = attributeMetaInfo.GetRelatedMetaIds(
					icomp::IComponentStaticInfo::MGI_INTERFACES,
					0,
					icomp::IAttributeStaticInfo::AF_NULLABLE);	// Names of the interfaces which must be set
		icomp::IElementStaticInfo::Ids optionalInterfaceNames = attributeMetaInfo.GetRelatedMetaIds(
					icomp::IComponentStaticInfo::MGI_INTERFACES,
					icomp::IAttributeStaticInfo::AF_NULLABLE,
					icomp::IAttributeStaticInfo::AF_NULLABLE);	// Names of the rest interfaces
		const QByteArray& componentId = idPtr->GetValue();

		if (!CheckPointedElementCompatibility(
					attributeFlags,
					componentId,
					interfaceNames,
					optionalInterfaceNames,
					registry,
					ignoreUndef,
					messagePrefix,
					reasonConsumerPtr)){
			return false;
		}
	}

	bool retVal = true;

	const icomp::CMultiReferenceAttribute* multiIdPtr = dynamic_cast<const icomp::CMultiReferenceAttribute*>(&attribute);
	if (multiIdPtr != NULL){
		icomp::IElementStaticInfo::Ids interfaceNames = attributeMetaInfo.GetRelatedMetaIds(
					icomp::IComponentStaticInfo::MGI_INTERFACES,
					0,
					icomp::IAttributeStaticInfo::AF_NULLABLE);	// Names of the interfaces which must be set
		icomp::IElementStaticInfo::Ids optionalInterfaceNames = attributeMetaInfo.GetRelatedMetaIds(
					icomp::IComponentStaticInfo::MGI_INTERFACES,
					icomp::IAttributeStaticInfo::AF_NULLABLE,
					icomp::IAttributeStaticInfo::AF_NULLABLE);	// Names of the rest interfaces
		int idsCount = multiIdPtr->GetValuesCount();
		for (int idIndex = 0; idIndex < idsCount; idIndex++){
			const QByteArray& componentId = multiIdPtr->GetValueAt(idIndex);

			if (!CheckPointedElementCompatibility(
						attributeFlags,
						componentId,
						interfaceNames,
						optionalInterfaceNames,
						registry,
						ignoreUndef,
						messagePrefix,
						reasonConsumerPtr)){
				if (!allReasons){
					return false;
				}
				else{
					retVal = false;
				}
			}
		}
	}

	return retVal;
}


bool CRegistryConsistInfoComp::CheckPointedElementCompatibility(
			int attributeFlags,
			const QByteArray& pointedElementName,
			const icomp::IElementStaticInfo::Ids& interfaceNames,
			const icomp::IElementStaticInfo::Ids& optionalInterfaceNames,
			const icomp::IRegistry& registry,
			bool ignoreUndef,
			const QString& messagePrefix,
			ilog::IMessageConsumer* reasonConsumerPtr) const
{
	if ((attributeFlags & (icomp::IAttributeStaticInfo::AF_FACTORY | icomp::IAttributeStaticInfo::AF_REFERENCE)) == 0){
		if (!ignoreUndef && (reasonConsumerPtr != NULL)){
			reasonConsumerPtr->AddMessage(ilog::IMessageConsumer::MessagePtr(new ilog::CMessage(
						istd::IInformationProvider::IC_ERROR,
						MI_BAD_ATTRIBUTE_TYPE,
						messagePrefix + tr("Attribute has incompatible registry entry"),
						tr("Attribute Consistency Check"),
						0)));
		}

		return false;
	}

	QByteArray baseId;
	QByteArray subId = pointedElementName;
	istd::CIdManipBase::SplitId(pointedElementName, baseId, subId);

	if (!subId.isEmpty() && ((attributeFlags & icomp::IAttributeStaticInfo::AF_FACTORY) != 0)){
		if (!ignoreUndef && (reasonConsumerPtr != NULL)){
			reasonConsumerPtr->AddMessage(ilog::IMessageConsumer::MessagePtr(new ilog::CMessage(
						istd::IInformationProvider::IC_ERROR,
						MI_BAD_ATTRIBUTE_TYPE,
						messagePrefix + tr("Factory try to access subcomponents '%1', which is allowed only for references").arg(QString(pointedElementName)),
						tr("Attribute Consistency Check"),
						0)));
		}

		return false;
	}

	const icomp::IRegistry::ElementInfo* pointedInfoPtr = registry.GetElementInfo(baseId);
	if (pointedInfoPtr != NULL){
		Q_ASSERT(pointedInfoPtr->elementPtr.IsValid());
		int pointedElementFlags = pointedInfoPtr->elementPtr->GetElementFlags();

		if ((pointedElementFlags & icomp::IRegistryElement::EF_IS_DETACHED) != 0){
			if ((attributeFlags & icomp::IAttributeStaticInfo::AF_REFERENCE) != 0){
				if (!ignoreUndef && (reasonConsumerPtr != NULL)){
					reasonConsumerPtr->AddMessage(ilog::IMessageConsumer::MessagePtr(new ilog::CMessage(
								istd::IInformationProvider::IC_ERROR,
								MI_BAD_ATTRIBUTE_TYPE,
								messagePrefix + tr("Reference try to access container-detached component '%1', please attach it").arg(QString(pointedElementName)),
								tr("Attribute Consistency Check"),
								0)));
				}

				return false;
			}
		}
		else{
			if ((attributeFlags & icomp::IAttributeStaticInfo::AF_FACTORY) != 0){
				if (!ignoreUndef && (reasonConsumerPtr != NULL)){
					reasonConsumerPtr->AddMessage(ilog::IMessageConsumer::MessagePtr(new ilog::CMessage(
								istd::IInformationProvider::IC_ERROR,
								MI_BAD_ATTRIBUTE_TYPE,
								messagePrefix + tr("Factory try to access container-attached component '%1', please detach it").arg(QString(pointedElementName)),
								tr("Attribute Consistency Check"),
								0)));
				}

				return false;
			}
		}

		const icomp::CComponentAddress& pointedElementAddress = pointedInfoPtr->address;
		if (!pointedElementAddress.GetPackageId().isEmpty()){
			const icomp::IElementStaticInfo* pointedMetaInfoPtr = m_envManagerCompPtr->GetComponentMetaInfo(pointedElementAddress);
			if (pointedMetaInfoPtr == NULL){
				if (!ignoreUndef && (reasonConsumerPtr != NULL)){
					reasonConsumerPtr->AddMessage(ilog::IMessageConsumer::MessagePtr(new ilog::CMessage(
								istd::IInformationProvider::IC_ERROR,
								MI_COMPONENT_NOT_FOUND,
								messagePrefix + tr("Reference or factory is set to %1, but it cannot be resolved").arg(QString(pointedElementName)),
								tr("Attribute Consistency Check"),
								0)));
				}

				return false;
			}

			if (!subId.isEmpty()){
				pointedMetaInfoPtr = pointedMetaInfoPtr->GetSubelementInfo(subId);

				if (pointedMetaInfoPtr == NULL){
					if (reasonConsumerPtr != NULL){
						reasonConsumerPtr->AddMessage(ilog::IMessageConsumer::MessagePtr(new ilog::CMessage(
									istd::IInformationProvider::IC_ERROR,
									MI_COMPONENT_NOT_FOUND,
									messagePrefix + tr("Reference or factory is set to %1, but its subelement cannot be found").arg(QString(pointedElementName)),
									tr("Attribute Consistency Check"),
									0)));
					}

					return false;
				}
			}

			return CheckPointedElementInfoCompatibility(
						baseId,
						pointedMetaInfoPtr,
						interfaceNames,
						optionalInterfaceNames,
						ignoreUndef,
						messagePrefix,
						reasonConsumerPtr);
		}
		else{
			icomp::IRegistry* embeddedRegistryPtr = registry.GetEmbeddedRegistry(pointedElementAddress.GetComponentId());
			if (embeddedRegistryPtr != NULL){
				icomp::CCompositeComponentStaticInfo info(*embeddedRegistryPtr, *m_envManagerCompPtr);

				const icomp::IElementStaticInfo* pointedMetaInfoPtr = &info;

				if (!subId.isEmpty()){
					pointedMetaInfoPtr = pointedMetaInfoPtr->GetSubelementInfo(subId);

					if (pointedMetaInfoPtr == NULL){
						if (reasonConsumerPtr != NULL){
							reasonConsumerPtr->AddMessage(ilog::IMessageConsumer::MessagePtr(new ilog::CMessage(
										istd::IInformationProvider::IC_ERROR,
										MI_COMPONENT_NOT_FOUND,
										messagePrefix + tr("Reference or factory is set to %1, but its subelement cannot be found").arg(QString(pointedElementName)),
										tr("Attribute Consistency Check"),
										0)));
						}

						return false;
					}
				}

				return CheckPointedElementInfoCompatibility(
							pointedElementName,
							pointedMetaInfoPtr,
							interfaceNames,
							optionalInterfaceNames,
							ignoreUndef,
							messagePrefix,
							reasonConsumerPtr);
			}
			else{
				if (reasonConsumerPtr != NULL){
					reasonConsumerPtr->AddMessage(ilog::IMessageConsumer::MessagePtr(new ilog::CMessage(
								istd::IInformationProvider::IC_ERROR,
								MI_COMPONENT_NOT_FOUND,
								messagePrefix + tr("Reference or factory uses embedded type '%1', but this type is undefined").arg(QString(pointedElementAddress.GetComponentId())),
								tr("Attribute Consistency Check"),
								0)));
				}

				return false;
			}
		}
	}
	else{
		if (reasonConsumerPtr != NULL){
			reasonConsumerPtr->AddMessage(ilog::IMessageConsumer::MessagePtr(new ilog::CMessage(
						istd::IInformationProvider::IC_ERROR,
						MI_COMPONENT_NOT_FOUND,
						messagePrefix + tr("Reference or factory contains '%1', but this element doesn't exist").arg(QString(pointedElementName)),
						tr("Attribute Consistency Check"),
						0)));
		}

		return false;
	}

	return true;
}


bool CRegistryConsistInfoComp::CheckPointedElementInfoCompatibility(
			const QByteArray& pointedElementName,
			const icomp::IElementStaticInfo* pointedMetaInfoPtr,
			const icomp::IElementStaticInfo::Ids& interfaceNames,
			const icomp::IElementStaticInfo::Ids& optionalInterfaceNames,
			bool ignoreUndef,
			const QString& messagePrefix,
			ilog::IMessageConsumer* reasonConsumerPtr) const
{
	if (pointedMetaInfoPtr != NULL){
		const icomp::IElementStaticInfo::Ids& supportedInterfaces = pointedMetaInfoPtr->GetMetaIds(icomp::IComponentStaticInfo::MGI_INTERFACES);

		// check obligatory attributes
		for (		icomp::IElementStaticInfo::Ids::const_iterator interfaceIter = interfaceNames.begin();
					interfaceIter != interfaceNames.end();
					++interfaceIter){
			const QByteArray& interfaceName = *interfaceIter;
			istd::CClassInfo interfaceType(interfaceName);

			if (		interfaceType.IsVoid() ||
						(interfaceName == "icomp::IComponent") ||
						(interfaceName == "const icomp::IComponent") ||
						(interfaceName == "void") ||
						(interfaceName == "const void")){
				continue;
			}

			QByteArray nonConstInterfaceName = interfaceType.GetConstCasted(false).GetName();
			bool isInterfaceCompatible =
						(supportedInterfaces.find(interfaceName) != supportedInterfaces.end()) ||
						(supportedInterfaces.find(nonConstInterfaceName) != supportedInterfaces.end());
			if (!isInterfaceCompatible){
				if (reasonConsumerPtr != NULL){
					reasonConsumerPtr->AddMessage(ilog::IMessageConsumer::MessagePtr(new ilog::CMessage(
								istd::IInformationProvider::IC_ERROR,
								MI_WRONG_INTERFACE,
								messagePrefix + tr("Reference or factory points at '%1', but it doesn't implement interface %2")
											.arg(QString(pointedElementName))
											.arg(QString(interfaceName)),
								tr("Attribute Consistency Check"),
								0)));
				}

				return false;
			}
		}

		// check optional attributes
		if (interfaceNames.isEmpty()){	// if there is at least one obligatory interface, none of optional must be found
			bool isCompatibleInterfaceFound = false;

			for (		icomp::IElementStaticInfo::Ids::const_iterator interfaceIter = optionalInterfaceNames.begin();
						interfaceIter != optionalInterfaceNames.end();
						++interfaceIter){
				const QByteArray& interfaceName = *interfaceIter;
				istd::CClassInfo interfaceType(interfaceName);

				if (		interfaceType.IsVoid() ||
							(interfaceName == "icomp::IComponent") ||
							(interfaceName == "const icomp::IComponent") ||
							(interfaceName == "void") ||
							(interfaceName == "const void")){
					isCompatibleInterfaceFound = true;

					break;
				}

				QByteArray nonConstInterfaceName = interfaceType.GetConstCasted(false).GetName();
				bool isInterfaceCompatible =
							(supportedInterfaces.find(interfaceName) != supportedInterfaces.end()) ||
							(supportedInterfaces.find(nonConstInterfaceName) != supportedInterfaces.end());
				if (isInterfaceCompatible){
					isCompatibleInterfaceFound = true;

					break;
				}
			}

			if (!isCompatibleInterfaceFound){
				if (reasonConsumerPtr != NULL){
					reasonConsumerPtr->AddMessage(ilog::IMessageConsumer::MessagePtr(new ilog::CMessage(
								istd::IInformationProvider::IC_ERROR,
								MI_WRONG_INTERFACE,
								messagePrefix + tr("Reference or factory points at '%1', but it doesn't implement any optional interface").arg(QString(pointedElementName)),
								tr("Attribute Consistency Check"),
								0)));
				}

				return false;
			}
		}
	}
	else if (!ignoreUndef){
		if (reasonConsumerPtr != NULL){
			reasonConsumerPtr->AddMessage(ilog::IMessageConsumer::MessagePtr(new ilog::CMessage(
						istd::IInformationProvider::IC_WARNING,
						MI_COMPOSITE_FOUND,
						messagePrefix + tr("Reference or factory '%1' in '%2' points at '%3', but it is not accessible in actual configuration").arg(QString(pointedElementName)),
						tr("Attribute Consistency Check"),
						0)));
		}

		return false;
	}

	return true;
}


} // namespace icmpstr


