#ifndef imeas_INumericParams_included
#define imeas_INumericParams_included


#include "iser/ISerializable.h"

#include "imath/CVarVector.h"


namespace imeas
{


class INumericConstraints;


class INumericParams: virtual public iser::ISerializable
{
public:
	/**
		Get parameter constraints describing possible parameter values.
	*/
	virtual const INumericConstraints* GetNumericConstraints() const = 0;

	/**
		Get filter length for each dimension.
	*/
	virtual imath::CVarVector GetValues() const = 0;

	/**
		Set filter length for each dimension.
	*/
	virtual bool SetValues(const imath::CVarVector& lengths) = 0;
};


} // namespace imeas


#endif // !imeas_INumericParams_included


