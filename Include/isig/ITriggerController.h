#ifndef isig_ITriggerController_included
#define isig_ITriggerController_included


// ACF includes
#include "istd/IPolymorphic.h"


namespace isig
{


class ITriggerController: virtual public istd::IPolymorphic
{
public:
	/**
		Check, if software trigger is accepted and possible.
	*/
	virtual bool IsSoftwareTriggerActive() const = 0;

	/**
		Send software trigger to device.
	*/
	virtual bool SendSoftwareTrigger() = 0;
};


} // namespace imeas


#endif // !isig_ITriggerController_included


