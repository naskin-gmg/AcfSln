#include "icmpstr/CElementSelectionInfoManagerBase.h"


	
namespace icmpstr
{


// public methods

CElementSelectionInfoManagerBase::CElementSelectionInfoManagerBase()
{
}


icomp::IRegistry* CElementSelectionInfoManagerBase::GetRegistry() const
{
	const IElementSelectionInfo* selectionInfoPtr = GetObjectPtr();
	if (selectionInfoPtr == NULL){
		return NULL;
	}

	return selectionInfoPtr->GetSelectedRegistry();
}


QStringList CElementSelectionInfoManagerBase::GetExportAliases(const QByteArray& attributeName) const
{
	const IElementSelectionInfo* selectionInfoPtr = GetObjectPtr();
	if (selectionInfoPtr == NULL){
		return QStringList();
	}

	QStringList exportedAliases;

	const icomp::IRegistry* registryPtr = selectionInfoPtr->GetSelectedRegistry();
	if (registryPtr != NULL){
		const icomp::IRegistry::ExportedElementsMap& exportedMap = registryPtr->GetExportedElementsMap();
		for (		icomp::IRegistry::ExportedElementsMap::const_iterator iter = exportedMap.begin();
					iter != exportedMap.end();
					++iter){
			if (iter.value() == attributeName){
				exportedAliases.append(iter.key());
			}
		}
	}

	return exportedAliases;
}


const iser::IObject* CElementSelectionInfoManagerBase::GetAttributeObject(
			const QByteArray& attributeId,
			const icomp::IRegistry::ElementInfo& elementInfo) const
{
	const icomp::IMetaInfoManager* metaInfoManagerPtr = GetMetaInfoManagerPtr();
	if (metaInfoManagerPtr == NULL){
		return NULL;
	}

	const icomp::IComponentStaticInfo* componentInfoPtr = metaInfoManagerPtr->GetComponentMetaInfo(elementInfo.address);
	if (componentInfoPtr == NULL){
		return NULL;
	}

	const icomp::IAttributeStaticInfo* staticInfoPtr = componentInfoPtr->GetAttributeInfo(attributeId);

	const iser::IObject* attributePtr = NULL;
	const icomp::IRegistryElement::AttributeInfo* attributeInfoPtr = elementInfo.elementPtr->GetAttributeInfo(attributeId);
	if ((attributeInfoPtr != NULL) && attributeInfoPtr->attributePtr.IsValid()){
		attributePtr = attributeInfoPtr->attributePtr.GetPtr();
	}
	else if (staticInfoPtr != NULL){
		attributePtr = staticInfoPtr->GetAttributeDefaultValue();
	}

	return attributePtr;
}


const icomp::IAttributeStaticInfo* CElementSelectionInfoManagerBase::GetAttributeStaticInfo(
			const QByteArray& attributeId,
			const icomp::IRegistry::ElementInfo& elementInfo) const
{
	const icomp::IMetaInfoManager* metaInfoManagerPtr = GetMetaInfoManagerPtr();
	if (metaInfoManagerPtr == NULL){
		return NULL;
	}

	const icomp::IComponentStaticInfo* componentInfoPtr = metaInfoManagerPtr->GetComponentMetaInfo(elementInfo.address);
	if (componentInfoPtr == NULL){
		return NULL;
	}

	return componentInfoPtr->GetAttributeInfo(attributeId);
}


} // namespace icmpstr


