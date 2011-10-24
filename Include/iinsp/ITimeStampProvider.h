#ifndef iinsp_ITimeStampProvider_included
#define iinsp_ITimeStampProvider_included


// ACF includes
#include "istd/IChangeable.h"
#include "isys/ITimer.h"


namespace iinsp
{


/**
	Interface allowing to retrieve a current time stamp.
*/
class ITimeStampProvider: virtual public istd::IChangeable
{
public:
	/**
		Get current processing the time stamp.
		\return	timer representing time stamp or NULL if it is impossible to retrive it.
	*/
	virtual isys::ITimer* GetCurrentTimeStamp() const = 0;
};


} // namespace iinsp


#endif // !iinsp_ITimeStampProvider_included


