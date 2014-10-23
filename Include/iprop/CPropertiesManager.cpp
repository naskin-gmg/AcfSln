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
		static ChangeSet changeSet(CF_RESET);
		istd::CChangeNotifier notifier(this, changeSet);

		int propertiesCount = m_propertiesList.GetCount();

		for (int propertyIndex = propertiesCount - 1; propertyIndex >= 0; --propertyIndex){
			PropertyInfo* propertyInfoPtr = m_propertiesList.GetAt(propertyIndex);
			Q_ASSERT(propertyInfoPtr != NULL);

			if (propertyInfoPtr->objectPtr.IsToRelase()){
				m_propertiesList.RemoveAt(propertyIndex);
			}
			else{
				IProperty* propertyPtr = dynamic_cast<IProperty*>(propertyInfoPtr->objectPtr.GetPtr());
				if (propertyPtr != NULL){
					propertyPtr->ResetValue();
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


int CPropertiesManager::FindPropertyIndex(const QByteArray& propertyId) const
{
	int propertiesCount = m_propertiesList.GetCount();

	for (int propertyIndex = 0; propertyIndex < propertiesCount; propertyIndex++){
		CPropertiesManager::PropertyInfo* propertyInfoPtr = m_propertiesList.GetAt(propertyIndex);
		Q_ASSERT(propertyInfoPtr != NULL);

		if (propertyInfoPtr->propertyId == propertyId){
			return propertyIndex;
		}
	}

	return -1;
}


void CPropertiesManager::InsertProperty(
			iser::IObject* objectPtr,
			const QByteArray& propertyId,
			const QByteArray& propertyDescription,
			int propertyFlags,
			bool releaseFlag)
{
	static ChangeSet changeSet(CF_ADD_PROPERTY);
	istd::CChangeNotifier notifier(this, changeSet);

	PropertyInfo* infoPtr = GetPropertyInfo(propertyId);
	if ((objectPtr != NULL) && (infoPtr == NULL)){
		infoPtr = new PropertyInfo;

		infoPtr->propertyId = propertyId;

		m_propertiesList.PushBack(infoPtr);
	}

	infoPtr->objectPtr.SetPtr(objectPtr, releaseFlag);
	infoPtr->propertyDescription = propertyDescription;
	infoPtr->propertyFlags = propertyFlags;
}


static iser::CArchiveTag s_propertiesTag("Properties", "List of object properties", iser::CArchiveTag::TT_MULTIPLE);
static iser::CArchiveTag s_propertyTag("Property", "Object property", iser::CArchiveTag::TT_GROUP, &s_propertiesTag);
static iser::CArchiveTag s_propertyTypeIdTag("PropertyTypeId", "ID of the property object", iser::CArchiveTag::TT_LEAF, &s_propertyTag);
static iser::CArchiveTag s_propertyIdTag("PropertyId", "Name of the property object", iser::CArchiveTag::TT_LEAF, &s_propertyTag);
static iser::CArchiveTag s_propertyFlagsTag("PropertyFlags", "Property flags", iser::CArchiveTag::TT_LEAF, &s_propertyTag);
static iser::CArchiveTag s_propertyDescriptionTag("PropertyDescription", "Property description", iser::CArchiveTag::TT_LEAF, &s_propertyTag);
static iser::CArchiveTag s_propertyObjectTag("PropertyObject", "Property object", iser::CArchiveTag::TT_GROUP, &s_propertyTag);

// reimplemented (iser::ISerializable)

bool CPropertiesManager::Serialize(iser::IArchive& archive)
{
	if (archive.IsStoring()){
		return WriteProperties(archive, s_propertiesTag, s_propertyTag);
	}

	return ReadProperties(archive, s_propertiesTag, s_propertyTag);
}


// protected methods

bool CPropertiesManager::RemoveProperty(const QByteArray& propertyId, bool onlyOwned)
{
	int propertiesCount = m_propertiesList.GetCount();

	for (int propertyIndex = 0; propertyIndex < propertiesCount; ++propertyIndex){
		PropertyInfo* propertyInfoPtr = m_propertiesList.GetAt(propertyIndex);
		Q_ASSERT(propertyInfoPtr != NULL);

		if (AreIdsEqual(propertyInfoPtr->propertyId, propertyId)){
			if (onlyOwned && !propertyInfoPtr->objectPtr.IsToRelase()){
				return false;
			}

			m_propertiesList.RemoveAt(propertyIndex);

			return true;
		}
	}

	return false;
}


bool CPropertiesManager::ReadProperties(
			iser::IArchive& archive,
			const iser::CArchiveTag& propertiesTag,
			const iser::CArchiveTag& propertyTag)
{
	static ChangeSet changeSet(CF_RESET);
	istd::CChangeNotifier notifier(this, changeSet);
	Q_UNUSED(notifier);

	bool retVal = true;

	int propertiesCount = m_propertiesList.GetCount();

	retVal = retVal && archive.BeginMultiTag(propertiesTag, propertyTag, propertiesCount);

	for (int propertyIndex = 0; propertyIndex < propertiesCount; ++propertyIndex){
		retVal = retVal && archive.BeginTag(propertyTag);

		QByteArray propertyId;
		QByteArray propertyTypeId;
		int propertyFlags = 0;
		QByteArray propertyDescription;

		retVal = retVal && archive.BeginTag(s_propertyTypeIdTag);
		retVal = retVal && archive.Process(propertyTypeId);
		retVal = retVal && archive.EndTag(s_propertyTypeIdTag);

		retVal = retVal && archive.BeginTag(s_propertyIdTag);
		retVal = retVal && archive.Process(propertyId);
		retVal = retVal && archive.EndTag(s_propertyIdTag);

		const iser::IVersionInfo& versionInfo = archive.GetVersionInfo();

		quint32 versionNumber = 0;
		if (!versionInfo.GetVersionNumber(1, versionNumber) || (versionNumber > 931)){
			retVal = retVal && archive.BeginTag(s_propertyFlagsTag);
			retVal = retVal && archive.Process(propertyFlags);
			retVal = retVal && archive.EndTag(s_propertyFlagsTag);

			retVal = retVal && archive.BeginTag(s_propertyDescriptionTag);
			retVal = retVal && archive.Process(propertyDescription);
			retVal = retVal && archive.EndTag(s_propertyDescriptionTag);
		}

		if (retVal){
			bool isToSerialize = true;

			istd::TDelPtr<iser::IObject> newObjectPtr;

			PropertyInfo* existingPropertyPtr = GetPropertyInfo(propertyId);
			if (existingPropertyPtr != NULL){
				bool isEqual = AreIdsEqual(propertyTypeId, existingPropertyPtr->objectPtr->GetFactoryId());
				if (isEqual){
					isToSerialize = ((existingPropertyPtr->propertyFlags & IProperty::PF_PERSISTENT) != 0);
				}
				else{
					if (existingPropertyPtr->objectPtr.IsToRelase()){
						RemoveProperty(propertyId);
						existingPropertyPtr = NULL;
					}
					else{
						return false;
					}
				}
			}

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

			iser::IObject* propertyPtr = NULL;
			if (isToSerialize && (existingPropertyPtr != NULL)){
				Q_ASSERT(existingPropertyPtr->objectPtr.IsValid());
				propertyPtr = existingPropertyPtr->objectPtr.GetPtr();
			}
			else{
				newObjectPtr.SetPtr(GetPropertyFactory().CreateInstance(propertyTypeId));
				propertyPtr = newObjectPtr.GetPtr();
			}


			if (propertyPtr == NULL){
				qDebug("Property could not be restored from the media");

				return false;
			}
		
			if (versionNumber > 931){
				retVal = retVal && archive.BeginTag(s_propertyObjectTag);
			}
			retVal = retVal && propertyPtr->Serialize(archive);

			if (versionNumber > 931){
				retVal = retVal && archive.EndTag(s_propertyObjectTag);
			}

			if (isToSerialize){
				if (retVal && newObjectPtr.IsValid()){
					InsertProperty(newObjectPtr.PopPtr(), propertyId, propertyDescription, propertyFlags, true);
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

		retVal = retVal && archive.BeginTag(s_propertyTypeIdTag);
		retVal = retVal && archive.Process(propertyTypeId);
		retVal = retVal && archive.EndTag(s_propertyTypeIdTag);

		retVal = retVal && archive.BeginTag(s_propertyIdTag);
		retVal = retVal && archive.Process(propertyId);
		retVal = retVal && archive.EndTag(s_propertyIdTag);

		const iser::IVersionInfo& versionInfo = archive.GetVersionInfo();

		quint32 versionNumber = 0;
		if (!versionInfo.GetVersionNumber(1, versionNumber) || (versionNumber > 931)){
			retVal = retVal && archive.BeginTag(s_propertyFlagsTag);
			retVal = retVal && archive.Process(propertyFlags);
			retVal = retVal && archive.EndTag(s_propertyFlagsTag);

			retVal = retVal && archive.BeginTag(s_propertyDescriptionTag);
			retVal = retVal && archive.Process(propertyDescription);
			retVal = retVal && archive.EndTag(s_propertyDescriptionTag);
		}

		retVal = retVal && archive.BeginTag(s_propertyObjectTag);
		retVal = retVal && objectPtr->Serialize(archive);
		retVal = retVal && archive.EndTag(s_propertyObjectTag);

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

	if (firstId == secondId){
		return true;
	}

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

	if (oldPropertyTypeMap.contains(firstId)){
		firstId = oldPropertyTypeMap.value(firstId);
	}

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


