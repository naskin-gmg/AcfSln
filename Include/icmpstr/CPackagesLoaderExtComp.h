// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#ifndef icmpstr_CPackagesLoaderExtComp_included
#define icmpstr_CPackagesLoaderExtComp_included


// Qt includes
#include <QtCore/QMap>
#include <QtCore/QDir>

// ACF includes
#include <icmpstr/IExternalMetaInfoManager.h>
#include <ipackage/CPackagesLoaderComp.h>


namespace icmpstr
{


class CPackagesLoaderExtComp:
			public ipackage::CPackagesLoaderComp,
			virtual public IExternalMetaInfoManager
{
public:
	typedef ipackage::CPackagesLoaderComp BaseClass;

	I_BEGIN_COMPONENT(CPackagesLoaderComp);
		I_REGISTER_INTERFACE(IExternalMetaInfoManager);
	I_END_COMPONENT;

	// reimplemented (iqt::CPackagesLoaderComp)
	virtual bool RegisterPackagesDir(const QString& subDir);

	// reimplemented (icmpstr::IExternalMetaInfoManager)
	virtual QString GetPackageInfoPath(const QByteArray& packageId) const;
	virtual QString GetComponentInfoPath(const icomp::CComponentAddress& address) const;

private:
	typedef QMap<QByteArray, QDir> PackageInfosMap;
	PackageInfosMap m_packageInfosMap;
};


} // namespace icmpstr


#endif // !icmpstr_CPackagesLoaderExtComp_included


