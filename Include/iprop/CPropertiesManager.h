#ifndef iprop_CPropertiesManager_included
#define iprop_CPropertiesManager_included


// Qt includes
#include <QtGui/QStandardItemModel>

// ACF includes
#include "istd/TComposedFactory.h"
#include "istd/TSingleFactory.h"
#include "istd/TOptPointerVector.h"
#include "istd/TDelPtr.h"

#include "iser/IArchive.h"
#include "iser/CArchiveTag.h"

#include "ibase/IQtItemModelProvider.h"

#include "iprop/TProperty.h"
#include "iprop/TMultiProperty.h"
#include "iprop/IPropertiesManager.h"


namespace iprop
{


/**
	Basic implementation of an property container.
*/
class CPropertiesManager:
			virtual public IPropertiesManager,
			virtual public ibase::IQtItemModelProvider
{
public:
	/*
		Internal property info item.
	*/
	struct PropertyInfo
	{
		QByteArray propertyId;
		QByteArray propertyDescription;
		int propertyFlags;

		istd::TOptDelPtr<iser::IObject> objectPtr; 
	};

	CPropertiesManager();

	/**
		Get property info object for a given property ID.
	*/
	PropertyInfo* GetPropertyInfo(const QByteArray& propertyId) const;

	/**
		Register a new property type.
	*/
	template <typename PropertyType>
	static bool RegisterPropertyType();

	// reimplemented (iprop::IPropertiesManager)
	virtual void RemoveAllProperties();
	virtual int GetPropertiesCount() const;
	virtual iser::IObject* GetProperty(int propertyIndex) const;
	virtual QByteArray GetPropertyId(int propertyIndex) const;
	virtual QString GetPropertyDescription(int propertyIndex) const;
	virtual void InsertProperty(
				iser::IObject* objectPtr,
				const QByteArray& propertyId,
				const QByteArray& propertyDescription,
				int propertyFlags,
				bool releaseFlag);

	// reimplemented (ibase::IQtItemModelProvider)
	virtual const QAbstractItemModel* GetItemModel() const;

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);

protected:
	bool ReadProperties(
				iser::IArchive& archive,
				const iser::CArchiveTag& propertiesTag,
				const iser::CArchiveTag& propertyTag);

	bool WriteProperties(
				iser::IArchive& archive,
				const iser::CArchiveTag& propertiesTag,
				const iser::CArchiveTag& propertyTag) const;
private:
	/**
		Compare two property ids.
		NOTE: Mac add extra space.
	*/
	bool AreIdsEqual(QByteArray firstId, QByteArray secondId) const;

private:
	typedef istd::TComposedFactory<iser::IObject> PropertyFactory;

	istd::TPointerVector<PropertyInfo> m_propertiesList;

	static PropertyFactory s_propertyFactory;

	QStandardItemModel m_itemModel;
};


// public static methods

template <typename PropertyImpl>
bool CPropertiesManager::RegisterPropertyType()
{
	return s_propertyFactory.RegisterFactory(new istd::TSingleFactory<iser::IObject, PropertyImpl>(PropertyImpl::GetTypeName()), true);
}


} // namespace iprop


#endif // !iprop_CPropertiesManager_included


