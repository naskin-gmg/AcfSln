// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#ifndef isig_ITriggerController_included
#define isig_ITriggerController_included


// ACF includes
#include <istd/IPolymorphic.h>
#include <istd/ITimeStamp.h>
#include <istd/TDelPtr.h>

#include <iprm/IParamsSet.h>


namespace isig
{


class ITriggerController: virtual public istd::IPolymorphic
{
public:
	typedef istd::TDelPtr<istd::ITimeStamp> TimeStampPtr;

	/**
		Check, if software trigger is accepted and possible.
	*/
	virtual bool IsSoftwareTriggerActive() const = 0;

	/**
		Send software trigger to device.
		\param	timeStampPtr	optional pointer to time stamp object, if it is not \c NULL, will be filled with trigger time stamp.
	*/
	virtual bool SendSoftwareTrigger(TimeStampPtr* timeStampPtrPtr = NULL, const iprm::IParamsSet* paramsPtr = NULL) = 0;
};


} // namespace imeas


#endif // !isig_ITriggerController_included


