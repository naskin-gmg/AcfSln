// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#ifndef imeas_ILinearAdjustConstraints_included
#define imeas_ILinearAdjustConstraints_included


#include <istd/IChangeable.h>
#include <istd/TRange.h>

#include <imeas/imeas.h>


namespace imeas
{


/**
	Describes constraints of possible values of imeas::ILinearAdjust.
*/
class ILinearAdjustConstraints: virtual public istd::IChangeable
{
public:
	/**
		Get range of possible scale factor values.
	*/
	virtual istd::CRange GetScaleFactorRange() const = 0;

	/**
		Get range of possible offset factor values.
	*/
	virtual istd::CRange GetOffsetFactorRange() const = 0;
};


} // namespace imeas


#endif // !imeas_ILinearAdjustConstraints_included


