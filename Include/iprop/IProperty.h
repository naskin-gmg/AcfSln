#ifndef iprop_IProperty_included
#define iprop_IProperty_included


#include "iser/IObject.h"

#include "iprop/iprop.h"


namespace iprop
{


/**
	Common interface for an abstract property.
*/
class IProperty: virtual public iser::IObject
{
public:
	enum PropertyFlags
	{
		/**
			Property is persistent.
		*/
		PF_PERSISTENT = 0x1
	};

	/**
		Get default value for the property.
	*/
	virtual const iser::IObject* GetDefaultPropertyValue() const = 0;
};


} // namespace iprop


#endif // !iprop_IProperty_included


