// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#ifndef iinsp_IIdProvider_included
#define iinsp_IIdProvider_included


// ACF includes
#include <istd/IChangeable.h>


namespace iinsp
{


/**
	Allow access to some ID.
	The meaning of the ID is depend on use case and implementation.
	Typically ID is used to unique identify inspection objects.
*/
class IIdProvider: virtual public istd::IChangeable
{
public:
	/**
		Get current ID value.
	*/
	virtual quint32 GetCurrentId() const = 0;
};


} // namespace iinsp


#endif // !iinsp_IIdProvider_included


