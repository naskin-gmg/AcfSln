#ifndef iprop_CPropertyBase_included
#define iprop_CPropertyBase_included


#include "iser/TCopySerializedWrap.h"

#include "iprop/IProperty.h"


namespace iprop
{


class IPropertiesManager;


/**
	Basic implementation of an property.
*/
class CPropertyBase: public iser::TCopySerializedWrap<IProperty>
{
public:
	CPropertyBase();
	CPropertyBase(
				IPropertiesManager* propertyOwnerPtr,
				const std::string& propertyId,
				const std::string& propertyDescription,
				int propertyFlags,
				int changeFlags = 0);

	// reimplemented (iprop::IProperty)
	virtual const iser::IObject* GetDefaultPropertyValue() const;

protected:
	IPropertiesManager* m_propertyOwnerPtr;
	int m_changeFlags;
};


} // namespace iprop


#endif // !iprop_CPropertyBase_included


