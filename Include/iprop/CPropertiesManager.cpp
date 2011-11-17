#include "iprop/CPropertiesManager.h"


// ACF includes
#include "iprop/TProperty.h"
#include "iprop/TMultiProperty.h"


namespace iprop
{


// public methods

CPropertiesManager::CPropertiesManager()
{
}


CPropertiesManager::PropertyInfo* CPropertiesManager::GetPropertyInfo(const std::string& propertyId) const
{
	int propertiesCount = m_propertiesList.GetCount();

	for (int propertyIndex = 0; propertyIndex < propertiesCount; propertyIndex++){
		CPropertiesManager::PropertyInfo* propertyInfoPtr = m_propertiesList.GetAt(propertyIndex);
		I_ASSERT(propertyInfoPtr != NULL);

		if (propertyInfoPtr->propertyId == propertyId){
			return propertyInfoPtr;
		}
	}

	return NULL;
}


// reimplemented (iprop::IPropertiesManager)

int CPropertiesManager::GetPropertiesCount() const
{
	return m_propertiesList.GetCount();
}


iser::IObject* CPropertiesManager::GetProperty(int propertyIndex) const
{
	I_ASSERT(propertyIndex >= 0);
	I_ASSERT(propertyIndex < m_propertiesList.GetCount());

	return m_propertiesList.GetAt(propertyIndex)->objectPtr.GetPtr();
}


std::string CPropertiesManager::GetPropertyId(int propertyIndex) const
{
	I_ASSERT(propertyIndex >= 0);
	I_ASSERT(propertyIndex < m_propertiesList.GetCount());

	return m_propertiesList.GetAt(propertyIndex)->propertyId;
}


istd::CString CPropertiesManager::GetPropertyDescription(int propertyIndex) const
{
	I_ASSERT(propertyIndex >= 0);
	I_ASSERT(propertyIndex < m_propertiesList.GetCount());

	return m_propertiesList.GetAt(propertyIndex)->propertyDescription;
}


void CPropertiesManager::InsertProperty(
			iser::IObject* objectPtr,
			const std::string& propertyId,
			const std::string& propertyDescription,
			int propertyFlags,
			bool releaseFlag)
{
	PropertyInfo* existingInfoPtr = GetPropertyInfo(propertyId);
	I_ASSERT(existingInfoPtr == NULL);
	if (objectPtr != NULL && existingInfoPtr == NULL){
		PropertyInfo* propertyInfoPtr = new PropertyInfo;

		propertyInfoPtr->objectPtr.SetPtr(objectPtr, releaseFlag);
		propertyInfoPtr->propertyId = propertyId;
		propertyInfoPtr->propertyDescription = propertyDescription;
		propertyInfoPtr->propertyFlags = propertyFlags;

		m_propertiesList.PushBack(propertyInfoPtr);
	}
}


// reimplemented (iser::ISerializable)

bool CPropertiesManager::Serialize(iser::IArchive& archive)
{
	iser::CArchiveTag propertiesTag("Properties", "Liste of object properties");
	iser::CArchiveTag propertyTag("Property", "Object property");

	if (archive.IsStoring()){
		return WriteProperties(archive, propertiesTag, propertyTag);
	}

	return ReadProperties(archive, propertiesTag, propertyTag);
}


bool CPropertiesManager::ReadProperties(
			iser::IArchive& archive,
			const iser::CArchiveTag& propertiesTag,
			const iser::CArchiveTag& propertyTag)
{
	bool retVal = true;

	int propertiesCount = m_propertiesList.GetCount();

	retVal = retVal && archive.BeginMultiTag(propertiesTag, propertyTag, propertiesCount);

	for (int propertyIndex = 0; propertyIndex < propertiesCount; ++propertyIndex){
		retVal = retVal && archive.BeginTag(propertyTag);

		std::string propertyId;
		std::string propertyTypeId;

		iser::CArchiveTag propertyTypeIdTag("PropertyTypeId", "ID of the property object");
		retVal = retVal && archive.BeginTag(propertyTypeIdTag);
		retVal = retVal && archive.Process(propertyTypeId);
		retVal = retVal && archive.EndTag(propertyTypeIdTag);

		iser::CArchiveTag propertyIdTag("PropertyId", "Name of the property object");
		retVal = retVal && archive.BeginTag(propertyIdTag);
		retVal = retVal && archive.Process(propertyId);
		retVal = retVal && archive.EndTag(propertyIdTag);

		if (retVal){
			PropertyInfo* existingPropertyPtr = GetPropertyInfo(propertyId);

			if (		existingPropertyPtr != NULL &&
						existingPropertyPtr->objectPtr->GetFactoryId() == propertyTypeId &&
						((existingPropertyPtr->propertyFlags & iprop::IProperty::PF_PERSISTENT) != 0)){
				retVal = retVal && existingPropertyPtr->objectPtr->Serialize(archive);
			}
			else{
				// try to serialize deprecated property:
				istd::TDelPtr<iser::IObject> objectPtr(s_propertyFactory.CreateInstance(propertyTypeId));
				if (objectPtr.IsValid()){
					retVal = retVal && objectPtr->Serialize(archive);
				}
			}
		}

		retVal = retVal && archive.EndTag(propertyTag);
	}

	retVal = retVal && archive.EndTag(propertiesTag);

	return retVal;
}


bool CPropertiesManager::WriteProperties(
			iser::IArchive& archive,
			const iser::CArchiveTag& propertiesTag,
			const iser::CArchiveTag& propertyTag) const
{

	bool retVal = true;
	int propertiesCount = m_propertiesList.GetCount();
	int persistentPropertiesCount = 0;

	for (int propertyIndex = 0; propertyIndex < propertiesCount; ++propertyIndex){
		PropertyInfo* propertyInfoPtr = m_propertiesList.GetAt(propertyIndex);

		bool isPersistent = ((propertyInfoPtr->propertyFlags & iprop::IProperty::PF_PERSISTENT) != 0);
		if (isPersistent){
			++persistentPropertiesCount;
		}
	}

	retVal = retVal && archive.BeginMultiTag(propertiesTag, propertyTag, persistentPropertiesCount);

	for (int propertyIndex = 0; propertyIndex < propertiesCount; ++propertyIndex){
		PropertyInfo* propertyInfoPtr = m_propertiesList.GetAt(propertyIndex);

		bool isPersistent = ((propertyInfoPtr->propertyFlags & iprop::IProperty::PF_PERSISTENT) != 0);
		if (!isPersistent){
			continue;
		}

		iser::IObject* objectPtr = propertyInfoPtr->objectPtr.GetPtr();
		std::string propertyId = propertyInfoPtr->propertyId;
		std::string propertyTypeId = objectPtr->GetFactoryId();

		retVal = retVal && archive.BeginTag(propertyTag);

		iser::CArchiveTag propertyTypeIdTag("PropertyTypeId", "ID of the property object");
		retVal = retVal && archive.BeginTag(propertyTypeIdTag);
		retVal = retVal && archive.Process(propertyTypeId);
		retVal = retVal && archive.EndTag(propertyTypeIdTag);

		iser::CArchiveTag propertyIdTag("PropertyId", "Name of the property object");
		retVal = retVal && archive.BeginTag(propertyIdTag);
		retVal = retVal && archive.Process(propertyId);
		retVal = retVal && archive.EndTag(propertyIdTag);

		retVal = retVal && objectPtr->Serialize(archive);

		retVal = retVal && archive.EndTag(propertyTag);
	}

	retVal = retVal && archive.EndTag(propertiesTag);

	return retVal;
}


// private static members

CPropertiesManager::PropertyFactory CPropertiesManager::s_propertyFactory;

static struct DefaultAttributeTypesRegistrator
{
	DefaultAttributeTypesRegistrator()
	{
		CPropertiesManager::RegisterPropertyType<iprop::CIntAttribute>();
		CPropertiesManager::RegisterPropertyType<iprop::CBoolAttribute>();
		CPropertiesManager::RegisterPropertyType<iprop::CDoubleAttribute>();
		CPropertiesManager::RegisterPropertyType<iprop::CStringAttribute>();
		CPropertiesManager::RegisterPropertyType<iprop::CMultiIntAttribute>();
		CPropertiesManager::RegisterPropertyType<iprop::CMultiBoolAttribute>();
		CPropertiesManager::RegisterPropertyType<iprop::CMultiDoubleAttribute>();
		CPropertiesManager::RegisterPropertyType<iprop::CMultiStringAttribute>();
	}

} s_defaultAttributeTypesRegistrator;


} // namespace iprop


