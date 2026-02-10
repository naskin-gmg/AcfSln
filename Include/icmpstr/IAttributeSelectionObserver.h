// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#ifndef icmpstr_IAttributeSelectionObserver_included
#define icmpstr_IAttributeSelectionObserver_included


#include <icomp/IAttributeStaticInfo.h>

#include <icmpstr/icmpstr.h>


namespace icmpstr
{


class IAttributeSelectionObserver: virtual public istd::IPolymorphic
{
public:
	virtual void OnAttributeSelected(const icomp::IAttributeStaticInfo* attributeStaticInfoPtr) = 0;
};


} // namespace icmpstr


#endif // !IAttributeSelectionObserver_included


