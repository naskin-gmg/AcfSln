#ifndef imeas_INumericValueProvider_included
#define imeas_INumericValueProvider_included


// ACF includes
#include <istd/IChangeable.h>
#include <imath/CVarVector.h>

// ACF-Solutions includes
#include <imeas/INumericValue.h>


namespace imeas
{


/**
	Provide access to the list of the numeric values.
*/
class INumericValueProvider: virtual public istd::IChangeable
{
public:
	/**
		Get number of numeric values in the list.
	*/
	virtual int GetValuesCount() const = 0;

	/**
		Returns a single numeric value from the list.
	*/
	virtual const imeas::INumericValue& GetNumericValue(int index) const = 0;
};


} // namespace imeas


#endif // !imeas_INumericValueProvider_included


