#ifndef imeas_INumericConstraints_included
#define imeas_INumericConstraints_included


// ACF includes
#include "istd/IChangeable.h"

// ACF-Solutions includes
#include "imeas/IUnitInfo.h"

#include "imeas/imeas.h"


namespace imeas
{


class INumericConstraints: virtual public istd::IChangeable
{
public:
	/**
		Get number of expected numeric values.
	*/
	virtual int GetNumericValuesCount() const = 0;

	/**
		Get description of numeric value for specified list index.
	*/
	virtual QString GetNumericValueDescription(int index) const = 0;

	/**
		Get range of possible numeric values for specified list index.
	*/
	virtual const imeas::IUnitInfo& GetNumericValueUnitInfo(int index) const = 0;
};


} // namespace imeas


#endif // !imeas_INumericConstraints_included


