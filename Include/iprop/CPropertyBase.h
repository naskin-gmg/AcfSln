#ifndef iprop_CPropertyBase_included
#define iprop_CPropertyBase_included


// ACF includes
#include "iser/TCopySerializedWrap.h"

// ACF-Solutions includes
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
				const QByteArray& propertyId,
				const QByteArray& propertyDescription,
				int propertyFlags,
				int changeFlags = 0);

	// reimplemented (iprop::IProperty)
	virtual void ResetValue();
	virtual const iser::IObject* GetDefaultPropertyValue() const;

protected:
	IPropertiesManager* m_propertyOwnerPtr;
	int m_changeFlags;
};


} // namespace iprop


#endif // !iprop_CPropertyBase_included


