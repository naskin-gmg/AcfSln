#ifndef iprop_IPropertiesManager_included
#define iprop_IPropertiesManager_included


#include "iser/IObject.h"

#include "iprop/iprop.h"


namespace iprop
{


class IProperty;


/**
	Common interface for a container of properties.
*/
class IPropertiesManager: virtual public iser::ISerializable
{
public:
	/**
		Get number of properties.
	*/
	virtual int GetPropertiesCount() const = 0;

	/**
		Get property with the given index.
	*/
	virtual iser::IObject* GetProperty(int propertyIndex) const = 0;

	/**
		Get the ID of the property with the given index \c propertyIndex.
		Property ID must be unique in the property set.
	*/
	virtual std::string GetPropertyId(int propertyIndex) const = 0;

	/**
		Get the description of the property with the given index \c propertyIndex
	*/
	virtual istd::CString GetPropertyDescription(int propertyIndex) const = 0;

	/**
		Insert an property into the object.
		\param objectPtr	Property data object.
		\param propertyId	Unique ID of the property in the attributes repository.
		\param propertyDescription	Human readable description of the property.
		\param propertyFlags	Property flags. \sa IProperty::PropertyFlags.
		\param releaseFlag	Indicate who control the property's memory.
		If \c releaseFlag is \c true, the control over the \c objectPtr will be overtaken by this object.
	*/
	virtual void InsertProperty(
				iser::IObject* objectPtr,
				const std::string& propertyId,
				const std::string& propertyDescription,
				int propertyFlags,
				bool releaseFlag) = 0;
};


} // namespace iprop


#endif // !iprop_IPropertiesManager_included


