#ifndef QtInspectionPck_included
#define QtInspectionPck_included


// ACF includes
#include <icomp/TModelCompWrap.h>
#include <icomp/TMakeComponentWrap.h>

#include <iqtinsp/CInspectionTaskGuiComp.h>
#include <iqtinsp/CGeneralSupplierGuiComp.h>
#include <iqtinsp/CMessageBasedViewExtenderComp.h>


/**
	Package contains UI-related components for inspection framework
*/
namespace QtInspectionPck
{


typedef iqtinsp::CInspectionTaskGuiComp InspectionTaskGui;
typedef iqtinsp::CGeneralSupplierGuiComp GeneralSupplierGui;
typedef iqtinsp::CMessageBasedViewExtenderComp MessageBasedViewExtender;


} // namespace QtInspectionPck


#endif // !QtInspectionPck_included


