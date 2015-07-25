#ifndef imeas_INumericConstraints_included
#define imeas_INumericConstraints_included


// ACF includes
#include "istd/IChangeable.h"

// ACF-Solutions includes
#include "imath/IUnitInfo.h"

#include "imeas/imeas.h"


namespace imeas
{


/**
	Describe additional meta information for set of numeric values.
	\sa imeas::INumericValue
*/
class INumericConstraints: virtual public istd::IChangeable
{
public:
	/**
		Get number of expected numeric values.
	*/
	virtual int GetNumericValuesCount() const = 0;

	/**
		Get human readable name of numeric value for specified list index.
		\param	index	index of numeric value.
	*/
	virtual QString GetNumericValueName(int index) const = 0;

	/**
		Get human readable description of numeric value for specified list index.
		\param	index	index of numeric value.
	*/
	virtual QString GetNumericValueDescription(int index) const = 0;

	/**
		Get range of possible numeric values for specified list index.
		\param	index	index of numeric value.
	*/
	virtual const imath::IUnitInfo* GetNumericValueUnitInfo(int index) const = 0;
};


} // namespace imeas


#endif // !imeas_INumericConstraints_included


