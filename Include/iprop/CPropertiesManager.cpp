#include "iprop/CPropertiesManager.h"


// Qt includes
#include <QtCore/QMap>

// ACF includes
#include "iprop/TProperty.h"
#include "iprop/TMultiProperty.h"


namespace iprop
{


// public methods

CPropertiesManager::CPropertiesManager()
{
}


CPropertiesManager::PropertyInfo* CPropertiesManager::GetPropertyInfo(const QByteArray& propertyId) const
{
	int propertiesCount = m_propertiesList.GetCount();

	for (int propertyIndex = 0; propertyIndex < propertiesCount; propertyIndex++){
		CPropertiesManager::PropertyInfo* propertyInfoPtr = m_propertiesList.GetAt(propertyIndex);
		Q_ASSERT(propertyInfoPtr != NULL);

		if (propertyInfoPtr->propertyId == propertyId){
			return propertyInfoPtr;
		}
	}

	return NULL;
}


// reimplemented (iprop::IPropertiesManager)

void CPropertiesManager::ResetProperties()
{
	if (!m_propertiesList.IsEmpty()){
		istd::CChangeNotifier changePtr(this, CF_MODEL | CF_RESET);

		bool workDone = false;
		while (!workDone){
			workDone = true;
			int propertiesCount = m_propertiesList.GetCount();

			for (int propertyIndex = 0; propertyIndex < propertiesCount; ++propertyIndex){
				IProperty::PropertyFlags flags = GetPropertyFlags(propertyIndex);

				if ((flags & IProperty::PF_DYNAMIC)){
					m_propertiesList.RemoveAt(propertyIndex);

					workDone = false;

					break;
				}
				else
				{
					PropertyInfo* propertyInfoPtr = m_propertiesList.GetAt(propertyIndex);
					Q_ASSERT(propertyInfoPtr != NULL);

					IProperty* propertyPtr = dynamic_cast<IProperty*>(propertyInfoPtr->objectPtr.GetPtr());
					if (propertyPtr != NULL){
						propertyPtr->ResetValue();
					}
				}
			}
		}
	}
}


int CPropertiesManager::GetPropertiesCount() const
{
	return m_propertiesList.GetCount();
}


iser::IObject* CPropertiesManager::GetProperty(int propertyIndex) const
{
	Q_ASSERT(propertyIndex >= 0);
	Q_ASSERT(propertyIndex < m_propertiesList.GetCount());

	return m_propertiesList.GetAt(propertyIndex)->objectPtr.GetPtr();
}


QByteArray CPropertiesManager::GetPropertyId(int propertyIndex) const
{
	Q_ASSERT(propertyIndex >= 0);
	Q_ASSERT(propertyIndex < m_propertiesList.GetCount());

	return m_propertiesList.GetAt(propertyIndex)->propertyId;
}


QString CPropertiesManager::GetPropertyDescription(int propertyIndex) const
{
	Q_ASSERT(propertyIndex >= 0);
	Q_ASSERT(propertyIndex < m_propertiesList.GetCount());

	return m_propertiesList.GetAt(propertyIndex)->propertyDescription;
}


IProperty::PropertyFlags CPropertiesManager::GetPropertyFlags(int propertyIndex) const
{
	Q_ASSERT(propertyIndex >= 0);
	Q_ASSERT(propertyIndex < m_propertiesList.GetCount());

	return IProperty::PropertyFlags(m_propertiesList.GetAt(propertyIndex)->propertyFlags);
}


void CPropertiesManager::InsertProperty(
			iser::IObject* objectPtr,
			const QByteArray& propertyId,
			const QByteArray& propertyDescription,
			int propertyFlags,
			bool releaseFlag)
{
	istd::CChangeNotifier notifier(this, CF_MODEL | CF_ADD_PROPERTY);

	PropertyInfo* existingInfoPtr = GetPropertyInfo(propertyId);
	Q_ASSERT(existingInfoPtr == NULL);
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
	static iser::CArchiveTag propertiesTag("Properties", "List of object properties");
	static iser::CArchiveTag propertyTag("Property", "Object property");

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
	istd::CChangeNotifier notifier(this, CF_MODEL | CF_RESET);

	bool retVal = true;

	int propertiesCount = m_propertiesList.GetCount();

	retVal = retVal && archive.BeginMultiTag(propertiesTag, propertyTag, propertiesCount);

	for (int propertyIndex = 0; propertyIndex < propertiesCount; ++propertyIndex){
		retVal = retVal && archive.BeginTag(propertyTag);

		QByteArray propertyId;
		QByteArray propertyTypeId;
		int propertyFlags = 0;
		QByteArray propertyDescription;

		static iser::CArchiveTag propertyTypeIdTag("PropertyTypeId", "ID of the property object");
		retVal = retVal && archive.BeginTag(propertyTypeIdTag);
		retVal = retVal && archive.Process(propertyTypeId);
		retVal = retVal && archive.EndTag(propertyTypeIdTag);

		static iser::CArchiveTag propertyIdTag("PropertyId", "Name of the property object");
		retVal = retVal && archive.BeginTag(propertyIdTag);
		retVal = retVal && archive.Process(propertyId);
		retVal = retVal && archive.EndTag(propertyIdTag);

		const iser::IVersionInfo& versionInfo = archive.GetVersionInfo();
		quint32 versionNumber = 0;

		if (versionInfo.GetVersionNumber(1, versionNumber)){
			if (versionNumber > 931){
				static iser::CArchiveTag propertyFlagsTag("PropertyFlags", "Property flags");
				retVal = retVal && archive.BeginTag(propertyFlagsTag);
				retVal = retVal && archive.Process(propertyFlags);
				retVal = retVal && archive.EndTag(propertyFlagsTag);

				static iser::CArchiveTag propertyDescriptionTag("PropertyDescription", "Property description");
				retVal = retVal && archive.BeginTag(propertyDescriptionTag);
				retVal = retVal && archive.Process(propertyDescription);
				retVal = retVal && archive.EndTag(propertyDescriptionTag);

				static iser::CArchiveTag propertyObjectTag("PropertyObject", "Property object");
				retVal = retVal && archive.BeginTag(propertyFlagsTag);
			}
		}

		if (retVal){
			bool isStatic = false;

			PropertyInfo* existingPropertyPtr = GetPropertyInfo(propertyId);
			if (existingPropertyPtr != NULL){
				bool isEqual = AreIdsEqual(propertyTypeId, existingPropertyPtr->objectPtr->GetFactoryId());
				if (isEqual && ((existingPropertyPtr->propertyFlags & IProperty::PF_PERSISTENT) != 0)){
					isStatic = true;
					retVal = retVal && existingPropertyPtr->objectPtr->Serialize(archive);
				}
			}

			if (!isStatic){
				// Support for old property type format:
				QMap<QByteArray, QByteArray> oldPropertyTypeMap;

				oldPropertyTypeMap[istd::CClassInfo::GetName<TProperty<int> >()] = TProperty<int>::GetTypeName();
				oldPropertyTypeMap[istd::CClassInfo::GetName<TProperty<double> >()] = TProperty<double>::GetTypeName();
				oldPropertyTypeMap[istd::CClassInfo::GetName<TProperty<bool> >()] = TProperty<bool>::GetTypeName();
				oldPropertyTypeMap[istd::CClassInfo::GetName<TProperty<QString> >()] = TProperty<QString>::GetTypeName();
				oldPropertyTypeMap[istd::CClassInfo::GetName<TProperty<QByteArray> >()] = TProperty<QByteArray>::GetTypeName();

				oldPropertyTypeMap[istd::CClassInfo::GetName<TMultiProperty<int> >()] = TMultiProperty<int>::GetTypeName();
				oldPropertyTypeMap[istd::CClassInfo::GetName<TMultiProperty<double> >()] = TMultiProperty<double>::GetTypeName();
				oldPropertyTypeMap[istd::CClassInfo::GetName<TMultiProperty<bool> >()] = TMultiProperty<bool>::GetTypeName();
				oldPropertyTypeMap[istd::CClassInfo::GetName<TMultiProperty<QString> >()] = TMultiProperty<QString>::GetTypeName();
				oldPropertyTypeMap[istd::CClassInfo::GetName<TMultiProperty<QByteArray> >()] = TMultiProperty<QByteArray>::GetTypeName();

				if (oldPropertyTypeMap.contains(propertyTypeId)){
					propertyTypeId = oldPropertyTypeMap.value(propertyTypeId);
				}

				// try to serialize a deprecated or dynamic property:
				istd::TDelPtr<iser::IObject> objectPtr(GetPropertyFactory().CreateInstance(propertyTypeId));
				if (objectPtr.IsValid()){
					retVal = retVal && objectPtr->Serialize(archive);

					if (retVal && (propertyFlags & iprop::IProperty::PF_DYNAMIC)){
						InsertProperty(objectPtr.PopPtr(), propertyId, propertyDescription, propertyFlags, true);
					}
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

		bool isPersistent = ((propertyInfoPtr->propertyFlags & IProperty::PF_PERSISTENT) != 0);
		if (isPersistent){
			++persistentPropertiesCount;
		}
	}

	retVal = retVal && archive.BeginMultiTag(propertiesTag, propertyTag, persistentPropertiesCount);

	for (int propertyIndex = 0; propertyIndex < propertiesCount; ++propertyIndex){
		PropertyInfo* propertyInfoPtr = m_propertiesList.GetAt(propertyIndex);

		bool isPersistent = ((propertyInfoPtr->propertyFlags & IProperty::PF_PERSISTENT) != 0);
		if (!isPersistent){
			continue;
		}

		iser::IObject* objectPtr = propertyInfoPtr->objectPtr.GetPtr();
		QByteArray propertyId = propertyInfoPtr->propertyId;
		QByteArray propertyTypeId = objectPtr->GetFactoryId();
		int propertyFlags = propertyInfoPtr->propertyFlags;
		QByteArray propertyDescription = propertyInfoPtr->propertyDescription;

		retVal = retVal && archive.BeginTag(propertyTag);

		static iser::CArchiveTag propertyTypeIdTag("PropertyTypeId", "ID of the property object");
		retVal = retVal && archive.BeginTag(propertyTypeIdTag);
		retVal = retVal && archive.Process(propertyTypeId);
		retVal = retVal && archive.EndTag(propertyTypeIdTag);

		static iser::CArchiveTag propertyIdTag("PropertyId", "Name of the property object");
		retVal = retVal && archive.BeginTag(propertyIdTag);
		retVal = retVal && archive.Process(propertyId);
		retVal = retVal && archive.EndTag(propertyIdTag);

		static iser::CArchiveTag propertyFlagsTag("PropertyFlags", "Property flags");
		retVal = retVal && archive.BeginTag(propertyFlagsTag);
		retVal = retVal && archive.Process(propertyFlags);
		retVal = retVal && archive.EndTag(propertyFlagsTag);

		static iser::CArchiveTag propertyDescriptionTag("PropertyDescription", "Property description");
		retVal = retVal && archive.BeginTag(propertyDescriptionTag);
		retVal = retVal && archive.Process(propertyDescription);
		retVal = retVal && archive.EndTag(propertyDescriptionTag);

		static iser::CArchiveTag propertyObjectTag("PropertyObject", "Property object");
		retVal = retVal && archive.BeginTag(propertyObjectTag);
		retVal = retVal && objectPtr->Serialize(archive);
		retVal = retVal && archive.EndTag(propertyObjectTag);

		retVal = retVal && archive.EndTag(propertyTag);
	}

	retVal = retVal && archive.EndTag(propertiesTag);

	return retVal;
}


// private methods

bool CPropertiesManager::AreIdsEqual(QByteArray firstId, QByteArray secondId) const
{
	if (firstId == secondId){
		return true;
	}

	firstId.replace(QByteArray(" "), QByteArray(""));
	secondId.replace(QByteArray(" "), QByteArray(""));

	return (firstId == secondId);
}


// private static methods

CPropertiesManager::PropertyFactory& CPropertiesManager::GetPropertyFactory()
{
	static PropertyFactory propertyFactory;

	return propertyFactory;
}


static struct DefaultPropertyTypesRegistrator
{
	DefaultPropertyTypesRegistrator()
	{
		CPropertiesManager::RegisterPropertyType<CIntProperty>();
		CPropertiesManager::RegisterPropertyType<CBoolProperty>();
		CPropertiesManager::RegisterPropertyType<CDoubleProperty>();
		CPropertiesManager::RegisterPropertyType<CStringProperty>();
		CPropertiesManager::RegisterPropertyType<CStdStringProperty>();
		CPropertiesManager::RegisterPropertyType<CMultiIntProperty>();
		CPropertiesManager::RegisterPropertyType<CMultiBoolProperty>();
		CPropertiesManager::RegisterPropertyType<CMultiDoubleProperty>();
		CPropertiesManager::RegisterPropertyType<CMultiStringProperty>();
		CPropertiesManager::RegisterPropertyType<CMultiStdStringProperty>();
	}

} s_defaultPropertyTypesRegistrator;


} // namespace iprop


