#ifndef iprop_CPropertiesManager_included
#define iprop_CPropertiesManager_included


#include "istd/TComposedFactory.h"
#include "istd/TSingleFactory.h"
#include "istd/TOptPointerVector.h"
#include "istd/TDelPtr.h"

#include "iser/IArchive.h"
#include "iser/CArchiveTag.h"

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
		Internal property info item.
	*/
	struct PropertyInfo
	{
		std::string propertyId;
		std::string propertyDescription;
		int propertyFlags;

		istd::TOptDelPtr<iser::IObject> objectPtr; 
	};

	CPropertiesManager();

	/**
		Get property info object for a given property ID.
	*/
	PropertyInfo* GetPropertyInfo(const std::string& propertyId) const;

	/**
		Register a new property type.
	*/
	template <typename PropertyType>
	static bool RegisterPropertyType();

	// reimplemented (iprop::IPropertiesManager)
	virtual int GetPropertiesCount() const;
	virtual iser::IObject* GetProperty(int propertyIndex) const;
	virtual std::string GetPropertyId(int propertyIndex) const;
	virtual QString GetPropertyDescription(int propertyIndex) const;
	virtual void InsertProperty(
				iser::IObject* objectPtr,
				const std::string& propertyId,
				const std::string& propertyDescription,
				int propertyFlags,
				bool releaseFlag);

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
	typedef istd::TComposedFactory<iser::IObject> PropertyFactory;

	istd::TPointerVector<PropertyInfo> m_propertiesList;

	static PropertyFactory s_propertyFactory;
};


// public static methods

template <typename PropertyImpl>
bool CPropertiesManager::RegisterPropertyType()
{
	return s_propertyFactory.RegisterFactory(new istd::TSingleFactory<iser::IObject, PropertyImpl>(PropertyImpl::GetTypeName()), true);
}


} // namespace iprop


#endif // !iprop_CPropertiesManager_included


