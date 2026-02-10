// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#ifndef iinsp_ITimeStampProvider_included
#define iinsp_ITimeStampProvider_included


// ACF includes
#include <istd/IChangeable.h>
#include <istd/ITimeStamp.h>


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
	virtual const istd::ITimeStamp* GetCurrentTimeStamp() const = 0;
};


} // namespace iinsp


#endif // !iinsp_ITimeStampProvider_included


