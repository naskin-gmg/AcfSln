#ifndef QtIprPck_included
#define QtIprPck_included


// ACF includes
#include "icomp/TModelCompWrap.h"
#include "icomp/TMakeComponentWrap.h"

#include "iqtipr/CSearchGuiComp.h"
#include "iqtipr/CSearchParamsGuiComp.h"
#include "iqtipr/CValueSupplierGuiComp.h"
#include "iqtipr/CCaliperParamsGuiComp.h"
#include "iqtipr/CLineProjectionSupplierGuiComp.h"
#include "iqtipr/CCircleFinderParamsGuiComp.h"


/**
	Standard package of Qt related implementations for image processing.
*/
namespace QtIprPck
{


typedef iqtipr::CSearchGuiComp SearchGui;
typedef iqtipr::CSearchParamsGuiComp SearchParamsGui;
typedef iqtipr::CValueSupplierGuiComp ValueSupplierGui;
typedef iqtipr::CCaliperParamsGuiComp CaliperParamsGui;
typedef iqtipr::CLineProjectionSupplierGuiComp LineProjectionSupplierGui;
typedef iqtipr::CCircleFinderParamsGuiComp CircleFinderParamsGui;



} // namespace QtIprPck


#endif // !QtIprPck_included


