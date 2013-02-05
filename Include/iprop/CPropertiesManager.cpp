#include "iprop/CPropertiesManager.h"


// ACF includes
#include "iprop/TProperty.h"
#include "iprop/TMultiProperty.h"


namespace iprop
{


// public methods

CPropertiesManager::CPropertiesManager()
{
	m_itemModel.setColumnCount(2);
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

void CPropertiesManager::RemoveAllProperties()
{
	if (!m_propertiesList.IsEmpty()){
		istd::CChangeNotifier changePtr(this, CF_MODEL | CF_RESET);

		m_propertiesList.Reset();

		m_itemModel.clear();
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

		QList<QStandardItem*> row;
		QStandardItem* propertyItemPtr = new QStandardItem(QString(propertyId));
		propertyItemPtr->setToolTip(QString(propertyDescription));
		row << propertyItemPtr;
	
		QStandardItem* propertyValueItemPtr = new QStandardItem();
		row << propertyValueItemPtr;

		m_itemModel.appendRow(row);
	}
}


// reimplemented (ibase::IQtItemModelProvider)

const QAbstractItemModel* CPropertiesManager::GetItemModel() const
{
	return &m_itemModel;
}


// reimplemented (iser::ISerializable)

bool CPropertiesManager::Serialize(iser::IArchive& archive)
{
	static iser::CArchiveTag propertiesTag("Properties", "Liste of object properties");
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
						((existingPropertyPtr->propertyFlags & IProperty::PF_PERSISTENT) != 0)){
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


// private methods

CPropertiesManager::CPropertiesManager(const CPropertiesManager&)
{
}


// private static members

CPropertiesManager::PropertyFactory CPropertiesManager::s_propertyFactory;

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


