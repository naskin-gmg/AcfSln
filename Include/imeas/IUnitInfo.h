#ifndef imeas_IUnitInfo_included
#define imeas_IUnitInfo_included


// ACF includes
#include "istd/IPolymorphic.h"
#include "istd/CRange.h"
#include "istd/CString.h"
#include "imath/TIValueManip.h"


namespace imeas
{


class IUnitInfo: virtual public istd::IPolymorphic
{
public:
	/**
		General type of unit.
	*/
	enum UnitType
	{
		UT_UNKNOWN,
		UT_PHYSICAL,
		UT_TECHNICAL,
		UT_RELATIVE
	};

	/**
		Get type of unit.
	*/
	virtual int GetUnitType() const = 0;

	/**
		Get name of unit.
	*/
	virtual istd::CString GetUnitName() const = 0;

	/**
		Get multiplication factor used to calculate user input values.
	*/
	virtual double GetDisplayMultiplicationFactor() const = 0;

	/**
		Get range of possible filter value for specified dimension.
	*/
	virtual istd::CRange GetValueRange() const = 0;

	/**
		Return number manipulator object for filter length value.
	*/
	virtual const imath::IDoubleManip& GetValueManip() const = 0;
};


} // namespace imeas


#endif // !imeas_IUnitInfo_included


