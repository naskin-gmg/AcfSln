#ifndef iprop_IProperty_included
#define iprop_IProperty_included


// ACF includes
#include "iser/IObject.h"

// ACF-Solutions includes
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
		PF_PERSISTENT = 0x1,

		/**
			Property was added to the container at the run time.
		*/
		PF_DYNAMIC = 0x2
	};

	/**
		Set property to a default value.
		\TODO: Think about moving this method to istd::IChangeable
	*/
	virtual void ResetValue() = 0;

	/**
		Get default value for the property.
	*/
	virtual const iser::IObject* GetDefaultPropertyValue() const = 0;
};


} // namespace iprop


#endif // !iprop_IProperty_included


