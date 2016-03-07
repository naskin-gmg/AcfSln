#ifndef IacfBasePck_included
#define IacfBasePck_included


// ACF includes
#include "iimg/CBitmap.h"
#include "iimg/CGeneralBitmap.h"

#include "icomp/TModelCompWrap.h"
#include "icomp/TMakeComponentWrap.h"

#include "icam/TSnapBitmapSupplierComp.h"
#include "icam/CExposureParamsComp.h"
#include "icam/CMemoryAcquisitionComp.h"
#include "icam/CAcquisitionConstraintsComp.h"
#include "icam/CSelectionParamCameraComp.h"
#include "icam/CCameraDelegatorComp.h"
#include "icam/CMultiCameraBitmapSupplierComp.h"
#include "icam/CSelectableBitmapSupplierComp.h"
#include "icam/CSnapMultiPageBitmapSupplierComp.h"
#include "icam/CSnapBitmapSupplierComp.h"
#include "icam/CCalibratedCameraComp.h"


/**
	Base system-independent general package.
*/
namespace CameraPck
{


typedef icomp::TModelCompWrap<icam::CExposureParamsComp> ExposureParams;
typedef icam::CMemoryAcquisitionComp MemoryAcquisition;
typedef icam::CAcquisitionConstraintsComp AcquisitionConstraints;
typedef icam::CSelectionParamCameraComp SelectionParamCamera;
typedef icam::CCameraDelegatorComp CameraDelegator;
typedef icomp::TModelCompWrap<icam::CMultiCameraBitmapSupplierComp> MultiCameraBitmapSupplier;
typedef icomp::TModelCompWrap<icam::CSelectableBitmapSupplierComp> SelectableBitmapSupplier;
typedef icomp::TModelCompWrap<icam::CSnapMultiPageBitmapSupplierComp> SnapMultiPageBitmapSupplier;

typedef icomp::TModelCompWrap<icam::CSnapBitmapSupplierComp> SnapBitmapSupplier;
typedef icomp::TModelCompWrap<icam::TSnapBitmapSupplierComp<iimg::CBitmap> > StandardQtBitmapSnapSupplier;
typedef icam::CCalibratedCameraComp CalibratedCamera;


} // namespace CameraPck


#endif // !IacfBasePck_included


