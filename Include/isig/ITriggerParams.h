#ifndef isig_ITriggerParams_included
#define isig_ITriggerParams_included


#include "iser/ISerializable.h"

#include "isig/isig.h"


namespace isig
{


class ITriggerConstraints;


/**
	Interface for AVT camera parameters.
*/
class ITriggerParams: virtual public iser::ISerializable
{
public:
	/**
		Describe working mode of trigger signal.
	*/
	enum TriggerMode
	{
		/**
			There is no trigger, no signal acquisition will be done.
		*/
		TM_NONE,
		/**
			There is no external trigger, permanently signal acquisition will be done.
		*/
		TM_CONTINUOUS,
		/**
			There external trigger with rising edge.
		*/
		TM_RISING_EDGE,
		/**
			There external trigger with falling edge.
		*/
		TM_FALLING_EDGE,
		/**
			There external trigger with positive edge control.
		*/
		TM_POSITIVE_LEVEL,
		/**
			There external trigger with negative edge control.
		*/
		TM_NEGATIVE_LEVEL,
		TM_SOFTWARE
	};

	/**
		Get constrints describing supported parameter states.
	*/
	virtual const ITriggerConstraints* GetTriggerConstraints() const = 0;

	/**
		Get trigger mode.
		Normal trigger modes are defined in \c isig::ITriggerParams::TriggerMode.
	*/
	virtual int GetTriggerMode() const = 0;

	/**
		Set trigger mode.
		Normal trigger modes are defined in \c isig::ITriggerParams::TriggerMode.
	*/
	virtual void SetTriggerMode(int triggerMode) = 0;
};


} // namespace isig


#endif // !isig_ITriggerParams_included


