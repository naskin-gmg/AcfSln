#ifndef isig_ITriggerConstraints_included
#define isig_ITriggerConstraints_included


#include "istd/IPolymorphic.h"


namespace isig
{


/**
	Interface for AVT camera parameters.
*/
class ITriggerConstraints: virtual public istd::IPolymorphic
{
public:
	/**
		Check if specified trigger mode is supported.
		Default mode should be always supported.
		Normal trigger modes are defined in \c isig::ITriggerParams::TriggerMode.
	*/
	virtual bool IsTriggerModeSupported(int triggerMode) const = 0;
};


} // namespace isig


#endif // !isig_ITriggerConstraints_included


