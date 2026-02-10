// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#ifndef imeas_ISampleAcquisition_included
#define imeas_ISampleAcquisition_included


// ACF includes
#include <istd/CIndex2d.h>
#include <iimg/IBitmap.h>
#include <iproc/IProcessor.h>


namespace imeas
{


/**
	Sample acquisition processor.
*/
class ISampleAcquisition: virtual public iproc::IProcessor
{
public:
	/**
		Get sampling rate.
		\return		number of samples per second.
	*/
	virtual double GetSamplingRate(const iprm::IParamsSet* paramsPtr) const = 0;
};


} // namespace imeas


#endif // !imeas_ISampleAcquisition_included


