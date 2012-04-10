#include "iprop/CPropertyBase.h"


#include "iprop/IPropertiesManager.h"


namespace iprop
{


// public methods

CPropertyBase::CPropertyBase()
	:m_propertyOwnerPtr(NULL),
	m_changeFlags(0)
{
}


CPropertyBase::CPropertyBase(
				IPropertiesManager* propertyOwnerPtr,
				const QByteArray& propertyId,
				const QByteArray& propertyDescription,
				int propertyFlags,
				int changeFlags)
	:m_propertyOwnerPtr(propertyOwnerPtr),
	m_changeFlags(changeFlags)
{
	if (propertyOwnerPtr != NULL){
		propertyOwnerPtr->InsertProperty(this, propertyId, propertyDescription, propertyFlags, false);
	}
}


// reimplemented (iprop::IProperty)

const iser::IObject* CPropertyBase::GetDefaultPropertyValue() const
{
	return NULL;
}


} // namespace iprop


