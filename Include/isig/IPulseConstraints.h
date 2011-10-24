#ifndef isig_IPulseConstraints_included
#define isig_IPulseConstraints_included


// ACF includes
#include "istd/IChangeable.h"
#include "istd/CRange.h"


namespace isig
{


/**
	Constraits of signal pulse paramters (isig::IPulseParams).
*/
class IPulseConstraints: virtual public istd::IChangeable
{
public:
	/**
		List of flags for supported options.
	*/
	enum PulseSupportedFlags
	{
		PSF_NONE = 0,
		PSF_DELAY_TIME = 1 << 0,
		PSF_PULSE_DURATION = 1 << 1,
		PSF_RELAXING_TIME = 1 << 2
	};

	/**
		Gets information about supported features.
	*/
	virtual int GetPulseSupportedFlags() const = 0;

	/**
		Get information about delay time range.
		\return	information about possible delay time in seconds
	*/
	virtual istd::CRange GetDelayTimeRange() const = 0;
	/**
		Get information about on time range.
		\return	information about possible on time in seconds
	*/
	virtual istd::CRange GetPulseDurationRange() const = 0;
	/**
		Get information about off time range.
		\return	information about possible off time in seconds
	*/
	virtual istd::CRange GetRelaxingTimeRange() const = 0;
};


} // namespace isig


#endif // !isig_IPulseConstraints_included


