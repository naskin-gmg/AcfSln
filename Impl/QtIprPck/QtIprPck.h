#ifndef QtIprPck_included
#define QtIprPck_included


// ACF includes
#include "icomp/TModelCompWrap.h"
#include "icomp/TMakeComponentWrap.h"

#include "iqtipr/CValueSupplierGuiComp.h"
#include "iqtipr/CCaliperParamsGuiComp.h"
#include "iqtipr/CLineProjectionSupplierGuiComp.h"
#include "iqtipr/CCircleFinderParamsGuiComp.h"
#include "iqtipr/CSearchBasedFeaturesSupplierGuiComp.h"


/**
	Standard package of Qt related implementations for image processing.
*/
namespace QtIprPck
{


typedef iqtipr::CValueSupplierGuiComp ValueSupplierGui;
typedef iqtipr::CCaliperParamsGuiComp CaliperParamsGui;
typedef iqtipr::CLineProjectionSupplierGuiComp LineProjectionSupplierGui;
typedef iqtipr::CCircleFinderParamsGuiComp CircleFinderParamsGui;
typedef iqtipr::CSearchBasedFeaturesSupplierGuiComp SearchBasedFeaturesSupplierGui;


} // namespace QtIprPck


#endif // !QtIprPck_included


