// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#ifndef icmpstr_IExternalMetaInfoManager_included
#define icmpstr_IExternalMetaInfoManager_included


#include <istd/IPolymorphic.h>
#include <QtCore/QString>

#include <icomp/CComponentAddress.h>


namespace icmpstr
{


class IExternalMetaInfoManager: virtual public istd::IPolymorphic
{
public:
	/**
		Get path of package info directory.
	*/
	virtual QString GetPackageInfoPath(const QByteArray& packageId) const = 0;

	/**
		Get path of component info directory.
	*/
	virtual QString GetComponentInfoPath(const icomp::CComponentAddress& address) const = 0;
};


} // namespace icmpstr


#endif // !icmpstr_IExternalMetaInfoManager_included


