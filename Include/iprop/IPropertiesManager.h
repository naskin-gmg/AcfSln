#ifndef iprop_IPropertiesManager_included
#define iprop_IPropertiesManager_included


// ACF includes
#include "iser/IObject.h"

// ACF-Solutions includes
#include "iprop/IProperty.h"


namespace iprop
{


/**
	Common interface for a container of properties.
*/
class IPropertiesManager: virtual public iser::ISerializable
{
public:
	/**
		Data model change notification flags.
	*/
	enum ChangeFlags
	{
		/**
			The property manager was initialized
		*/
		CF_RESET = 0xb812936,

		/**
			A new property was added to the manager
		*/
		CF_ADD_PROPERTY
	};

	/**
		Reset properties.
		The method set all static properties to their default values 
		and removes all dynamically added properties from the manager.
	*/
	virtual void ResetProperties() = 0;

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
	virtual QByteArray GetPropertyId(int propertyIndex) const = 0;

	/**
		Get the description of the property with the given index \c propertyIndex
	*/
	virtual QString GetPropertyDescription(int propertyIndex) const = 0;

	/**
		Get the property flags.
		\sa IProperty::PropertyFlags
	*/
	virtual IProperty::PropertyFlags GetPropertyFlags(int propertyIndex) const = 0;

	/**
		Insert an property into the object.
		\param objectPtr	Property data object.
		\param propertyId	Unique ID of the property in the properties repository.
		\param propertyDescription	Human readable description of the property.
		\param propertyFlags	Property flags. \sa IProperty::PropertyFlags.
		\param releaseFlag	Indicate who control the property's memory.
		If \c releaseFlag is \c true, the control over the \c objectPtr will be overtaken by this object.
	*/
	virtual void InsertProperty(
				iser::IObject* objectPtr,
				const QByteArray& propertyId,
				const QByteArray& propertyDescription,
				int propertyFlags,
				bool releaseFlag) = 0;
};


} // namespace iprop


#endif // !iprop_IPropertiesManager_included


