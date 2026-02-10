// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#ifndef InspectionPck_included
#define InspectionPck_included


// ACF includes
#include <icomp/TModelCompWrap.h>
#include <icomp/TMakeComponentWrap.h>

#include <iinsp/CInspectionTaskComp.h>
#include <iinsp/CInformationProviderCacheComp.h>
#include <iinsp/CIdProviderCacheComp.h>
#include <iinsp/CComposedInformationProviderComp.h>
#include <iinsp/CSupplierActivationProxyComp.h>


/**
	General inspection-related package.
*/
namespace InspectionPck
{


typedef icomp::TModelCompWrap<iinsp::CInspectionTaskComp> InspectionTask;
typedef icomp::TModelCompWrap<iinsp::CInformationProviderCacheComp> InformationProviderCache;
typedef icomp::TModelCompWrap<iinsp::CIdProviderCacheComp> IdProviderCache;
typedef icomp::TModelCompWrap<iinsp::CComposedInformationProviderComp> ComposedInformationProvider;
typedef icomp::TModelCompWrap<iinsp::CSupplierActivationProxyComp> SupplierActivationProxy;


} // namespace InspectionPck


#endif // !InspectionPck_included


