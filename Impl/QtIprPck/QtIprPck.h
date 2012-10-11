#ifndef QtIprPck_included
#define QtIprPck_included


// ACF includes
#include "icomp/TModelCompWrap.h"
#include "icomp/TMakeComponentWrap.h"

// ACF-Solutions
#include "iqtipr/CValueSupplierGuiComp.h"
#include "iqtipr/CCaliperParamsGuiComp.h"
#include "iqtipr/CLineProjectionSupplierGuiComp.h"
#include "iqtipr/CCircleFinderParamsGuiComp.h"
#include "iqtipr/CSearchBasedFeaturesSupplierGuiComp.h"
#include "iqtipr/CGeneralSearchParamsGuiComp.h"
#include "iqtipr/CConvolution2dParamsGuiComp.h"
#include "iqtipr/CMultiLineSupplierGuiComp.h"
#include "iqtipr/CMultiLineProjectionSupplierGuiComp.h"


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
typedef iqtipr::CGeneralSearchParamsGuiComp GeneralSearchParamsGui;
typedef iqtipr::CConvolution2dParamsGuiComp Convolution2dParamsGui;
typedef imod::TModelWrap<iqtipr::CMultiLineSupplierGuiComp> MultiLineSupplierGui;
typedef iqtipr::CMultiLineProjectionSupplierGuiComp MultiLineProjectionSupplierGui;


} // namespace QtIprPck


#endif // !QtIprPck_included


