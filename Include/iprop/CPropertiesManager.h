#ifndef iprop_CPropertiesManager_included
#define iprop_CPropertiesManager_included


// ACF includes
#include "istd/TComposedFactory.h"
#include "istd/TSingleFactory.h"
#include "istd/TOptPointerVector.h"
#include "istd/TDelPtr.h"
#include "iser/IArchive.h"
#include "iser/CArchiveTag.h"

// ACF-Solutions includes
#include "iprop/TProperty.h"
#include "iprop/TMultiProperty.h"
#include "iprop/IPropertiesManager.h"


namespace iprop
{


/**
	Basic implementation of an property container.
*/
class CPropertiesManager: virtual public IPropertiesManager
{
public:
	/*
		Internal property info object.
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
	virtual void ResetProperties();
	virtual int GetPropertiesCount() const;
	virtual iser::IObject* GetProperty(int propertyIndex) const;
	virtual QByteArray GetPropertyId(int propertyIndex) const;
	virtual QString GetPropertyDescription(int propertyIndex) const;
	virtual IProperty::PropertyFlags GetPropertyFlags(int propertyIndex) const;
	virtual int FindPropertyIndex(const QByteArray& propertyId) const;
	virtual void InsertProperty(
				iser::IObject* objectPtr,
				const QByteArray& propertyId,
				const QByteArray& propertyDescription,
				int propertyFlags,
				bool releaseFlag);

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);

protected:
	/**
		Remove property from manager.
		\param	propertyId	ID of property to remove.
		\param	onlyOwned	if true, only properties owned by manager can be removed.
							Property is owned by manager if it was inserted with set \c releaseFlag.
		\return	true, if property could be removed.
	 */
	bool RemoveProperty(const QByteArray& propertyId, bool onlyOwned = true);
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

	typedef istd::TComposedFactory<iser::IObject> PropertyFactory;

	static PropertyFactory& GetPropertyFactory();

private:
	istd::TPointerVector<PropertyInfo> m_propertiesList;
};


// public static methods

template <typename PropertyImpl>
bool CPropertiesManager::RegisterPropertyType()
{
	return GetPropertyFactory().RegisterFactory(new istd::TSingleFactory<iser::IObject, PropertyImpl>(PropertyImpl::GetTypeName()), true);
}


} // namespace iprop


#endif // !iprop_CPropertiesManager_included


