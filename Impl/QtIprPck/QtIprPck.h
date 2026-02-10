// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#ifndef QtIprPck_included
#define QtIprPck_included


// ACF-Solutions
#include <iqtipr/CValueSupplierGuiComp.h>
#include <iqtipr/CCaliperParamsGuiComp.h>
#include <iqtipr/CLineProjectionSupplierGuiComp.h>
#include <iqtipr/CCircleFinderParamsGuiComp.h>
#include <iqtipr/CSearchBasedFeaturesSupplierGuiComp.h>
#include <iqtipr/CGeneralSearchParamsGuiComp.h>
#include <iqtipr/CConvolution2dParamsGuiComp.h>
#include <iqtipr/CMultiLineSupplierGuiComp.h>
#include <iqtipr/CMultiLineProjectionSupplierGuiComp.h>
#include <iqtipr/CColorPatternComparatorGuiComp.h>
#include <iqtipr/CPatternTeachingControllerGuiComp.h>
#include <iqtipr/CImageInterpolationParamsGuiComp.h>


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
typedef iqtipr::CMultiLineSupplierGuiComp MultiLineSupplierGui;
typedef iqtipr::CMultiLineProjectionSupplierGuiComp MultiLineProjectionSupplierGui;
typedef iqtipr::CPatternTeachingControllerGuiComp PatternTeachingControllerGui;
typedef iqtipr::CColorPatternComparatorGuiComp ColorPatternComparatorGui;
typedef iqtipr::CImageInterpolationParamsGuiComp ImageInterpolationParamsGui;


} // namespace QtIprPck


#endif // !QtIprPck_included


