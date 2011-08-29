#ifndef imeas_INumericConstraints_included
#define imeas_INumericConstraints_included


// ACF includes
#include "istd/IPolymorphic.h"

// ACF-Solutions includes
#include "imeas/IUnitInfo.h"

#include "imeas/imeas.h"


namespace imeas
{


class INumericConstraints: virtual public istd::IPolymorphic
{
public:
	/**
		Get number of expected numeric values.
	*/
	virtual int GetNumericValuesCount() const = 0;

	/**
		Get description of some filter element.
	*/
	virtual istd::CString GetNumericValueDescription(int index) const = 0;

	/**
		Get range of possible filter value for specified dimension.
	*/
	virtual const imeas::IUnitInfo& GetNumericValueUnitInfo(int index) const = 0;
};


} // namespace imeas


#endif // !imeas_INumericConstraints_included


