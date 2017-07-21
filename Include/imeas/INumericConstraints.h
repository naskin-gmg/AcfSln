#ifndef imeas_INumericConstraints_included
#define imeas_INumericConstraints_included


// ACF includes
#include <iprm/IOptionsList.h>

// ACF-Solutions includes
#include <imath/IUnitInfo.h>
#include <imeas/imeas.h>


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
		Get general information about the "content" of the numeric value.
	*/
	virtual const iprm::IOptionsList& GetValueListInfo() const = 0;

	/**
		Get range of possible numeric values for specified list index.
		\param	index	index of numeric value.
	*/
	virtual const imath::IUnitInfo* GetNumericValueUnitInfo(int index) const = 0;
};


} // namespace imeas


#endif // !imeas_INumericConstraints_included


