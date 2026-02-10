// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#ifndef QtInspectionPck_included
#define QtInspectionPck_included


// ACF includes
#include <icomp/TModelCompWrap.h>
#include <icomp/TMakeComponentWrap.h>

#include <iqtinsp/CInspectionTaskGuiComp.h>
#include <iqtinsp/CGeneralSupplierGuiComp.h>
#include <iqtinsp/CMessageBasedViewExtenderComp.h>
#include <iqtinsp/CTeachableParamsDecoratorGuiComp.h>


/**
	Package contains UI-related components for inspection framework
*/
namespace QtInspectionPck
{


typedef iqtinsp::CInspectionTaskGuiComp InspectionTaskGui;
typedef iqtinsp::CGeneralSupplierGuiComp GeneralSupplierGui;
typedef iqtinsp::CMessageBasedViewExtenderComp MessageBasedViewExtender;
typedef iqtinsp::CTeachableParamsDecoratorGuiComp TeachableParamsDecoratorGui;


} // namespace QtInspectionPck


#endif // !QtInspectionPck_included


