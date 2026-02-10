// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#ifndef iproc_IElapsedTimeProvider_included
#define iproc_IElapsedTimeProvider_included


#include <istd/IChangeable.h>


namespace iproc
{


/**
	Provider of the elapsed time
	E.g. used for processing time measuring.
*/
class IElapsedTimeProvider: virtual public istd::IChangeable
{
public:
	/**
		Get the elapsed time.
	*/
	virtual double GetElapsedTime() const = 0;
};


} // namespace iproc


#endif // !iproc_IElapsedTimeProvider_included


