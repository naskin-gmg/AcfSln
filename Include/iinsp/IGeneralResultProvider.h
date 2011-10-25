#ifndef iinsp_IGeneralResultProvider_included
#define iinsp_IGeneralResultProvider_included


// ACF includes
#include "istd/IChangeable.h"


namespace iinsp
{


/**
	Provide general inspection result.
*/
class IGeneralResultProvider: virtual public istd::IChangeable
{
public:
	enum GeneralResult
	{
		GR_OK,
		GR_WARNING,
		GR_FAILED,
		GR_CRITICAL
	};

	virtual GeneralResult GetGeneralResult() const = 0;
};


} // namespace iinsp


#endif // !iinsp_IGeneralResultProvider_included


