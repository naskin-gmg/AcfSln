// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#ifndef iinsp_IConsumer_included
#define iinsp_IConsumer_included


// ACF includes
#include <istd/IChangeable.h>
#include <iinsp/ISupplier.h>


namespace iinsp
{


/**
	Base interface for consumers accepting suppliers.
*/
class IConsumer: virtual public istd::IChangeable
{
public:
	virtual void AcceptSupplier(const ISupplier* supplier) = 0;
};


} // namespace iinsp


#endif // !iinsp_IConsumer_included


