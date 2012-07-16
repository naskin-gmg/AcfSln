#ifndef iipr_IISearchParamsConstraints_included
#define iipr_IISearchParamsConstraints_included


#include "istd/IChangeable.h"
#include "istd/TRange.h"


namespace iipr
{


/**	
	Interface for search algorithm limitations.
*/
class ISearchConstraints: virtual public istd::IChangeable
{
public:
	virtual const istd::CRange& GetRotationRangeConstraints() const = 0;
	virtual bool IsRotationRangeSupported() const = 0;
	virtual const istd::CRange& GetScaleRangeConstraints() const = 0;
	virtual bool IsScaleRangeSupported() const = 0;
};


} // namespace iipr


#endif // !iipr_IISearchParamsConstraints_included

