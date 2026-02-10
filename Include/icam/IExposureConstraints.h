// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#ifndef icam_IExposureConstraints_included
#define icam_IExposureConstraints_included


#include <istd/IChangeable.h>
#include <istd/TRange.h>

#include <icam/icam.h>


namespace icam
{


class IExposureConstraints: virtual public istd::IChangeable
{
public:
	/**
		Get range of posible shutter time (exposure time) values.
		\return	range of shutter time, or empty range is this time cannot be controlled.
	*/
	virtual istd::CRange GetShutterTimeRange(int cameraIndex) const = 0;

	/**
		Get range of posible delay time values.
		Delay time is time between trigger and exposure begin.
		\return	range of delay time, or empty range is this time cannot be controlled.
	*/
	virtual istd::CRange GetDelayTimeRange(int cameraIndex) const = 0;

	/**
		Get range of posible exposure enabled (EEN) signal delay time values.
		\return	range of delay time, or empty range is this time cannot be controlled.
	*/
	virtual istd::CRange GetEenDelayRange(int cameraIndex) const = 0;

	// Gain range
	virtual istd::CIntRange GetGainRange(int cameraIndex) const = 0;
};


} // namespace icam


#endif // !icam_IExposureConstraints_included


