#include "icmpstr/CElementSelectionInfoManagerBase.h"


	
namespace icmpstr
{


// public methods

CElementSelectionInfoManagerBase::CElementSelectionInfoManagerBase()
{
}


icomp::IRegistry* CElementSelectionInfoManagerBase::GetRegistry() const
{
	const IElementSelectionInfo* selectionInfoPtr = GetObservedObject();
	if (selectionInfoPtr == NULL){
		return NULL;
	}

	return selectionInfoPtr->GetSelectedRegistry();
}


QStringList CElementSelectionInfoManagerBase::GetExportAliases(const QByteArray& attributeName) const
{
	const IElementSelectionInfo* selectionInfoPtr = GetObservedObject();
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
	const iser::IObject* attributePtr = elementInfo.elementPtr->GetAttribute(attributeId);
	if (attributePtr != NULL){
		return attributePtr;
	}

	const icomp::IMetaInfoManager* metaInfoManagerPtr = GetMetaInfoManagerPtr();
	if (metaInfoManagerPtr == NULL){
		return NULL;
	}

	const icomp::IComponentStaticInfo* componentInfoPtr = metaInfoManagerPtr->GetComponentMetaInfo(elementInfo.address);
	if (componentInfoPtr == NULL){
		return NULL;
	}

	const icomp::IAttributeStaticInfo* staticInfoPtr = componentInfoPtr->GetAttributeInfo(attributeId);
	if (staticInfoPtr != NULL){
		return staticInfoPtr->GetAttributeDefaultValue();
	}

	return NULL;
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


