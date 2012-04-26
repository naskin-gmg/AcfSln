#ifndef imeas_INumericParams_included
#define imeas_INumericParams_included


#include "iser/ISerializable.h"

#include "imath/CVarVector.h"


namespace imeas
{


class INumericConstraints;


/**
	General parameter set containing list of numeric values.
*/
class INumericParams: virtual public iser::ISerializable
{
public:
	/**
		Get parameter constraints describing possible parameter values.
	*/
	virtual const INumericConstraints* GetNumericConstraints() const = 0;

	/**
		Get list of numeric values.
	*/
	virtual imath::CVarVector GetValues() const = 0;

	/**
		Set list of numeric values.
	*/
	virtual bool SetValues(const imath::CVarVector& values) = 0;
};


} // namespace imeas


#endif // !imeas_INumericParams_included


