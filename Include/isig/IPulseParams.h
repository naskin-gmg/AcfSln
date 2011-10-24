#ifndef isig_IPulseParams_included
#define isig_IPulseParams_included


// ACF includes
#include "iser/ISerializable.h"


namespace isig
{


class IPulseConstraints;


/**
	Parammeters of single signal pulse.
*/
class IPulseParams: virtual public iser::ISerializable
{
public:
	/**
		Get access to constraits of this parameters.
	*/
	virtual const IPulseConstraints* GetPulseConstraints() const = 0;

	/**
		Get delay time from begin (for example trigger edge) to pulse begin.
		\return time in seconds.
	*/
	virtual double GetDelayTime() const = 0;
	/**
		Set delay time from begin (for example trigger edge) to pulse begin.
		\param time	delay time in seconds.
	*/
	virtual void SetDelayTime(double time) = 0;

	/**
		Get pulse duration time.
		\return pulse duration time in seconds.
	*/
	virtual double GetPulseDuration() const = 0;
	/**
		Set pulse duration time.
		\param time	pulse duration time in seconds.
	*/
	virtual void SetPulseDuration(double time) = 0;

	/**
		Get relaxing time period after pulse ends needed to accept new begin.
		\return	off time in seconds.
	*/
	virtual double GetRelaxingTime() const = 0;
	/**
		Get relaxing time period after pulse ends needed to accept new begin.
		\param time	off time in seconds.
	*/
	virtual void SetRelaxingTime(double time) = 0;
};


} // namespace isig


#endif // !isig_IPulseParams_included


