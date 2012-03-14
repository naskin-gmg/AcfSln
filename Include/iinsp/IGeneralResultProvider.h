#ifndef iinsp_IGeneralResultProvider_included
#define iinsp_IGeneralResultProvider_included


// ACF includes
#include "istd/IChangeable.h"
#include "istd/ILogger.h"


namespace iinsp
{


/**
	Provide general inspection result.
*/
class IGeneralResultProvider: virtual public istd::IChangeable
{
public:
	virtual istd::IInformation::InformationCategory GetGeneralResult() const = 0;
};


} // namespace iinsp


#endif // !iinsp_IGeneralResultProvider_included


