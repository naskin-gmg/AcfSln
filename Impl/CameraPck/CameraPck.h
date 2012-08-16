#ifndef IacfBasePck_included
#define IacfBasePck_included


// ACF includes
#include "icomp/TModelCompWrap.h"
#include "icomp/TMakeComponentWrap.h"

#include "icam/CExposureParamsComp.h"
#include "icam/CSnapBitmapSupplierComp.h"
#include "icam/CMemoryAcquisitionComp.h"
#include "icam/CAcquisitionConstraintsComp.h"
#include "icam/CSelectionParamCameraComp.h"
#include "icam/CCameraDelegatorComp.h"
#include "icam/CMultiCameraBitmapSupplierComp.h"
#include "icam/CSelectableBitmapSupplierComp.h"


/**
	Base system-independent general package.
*/
namespace CameraPck
{


typedef icomp::TModelCompWrap<icam::CExposureParamsComp> ExposureParams;
typedef icomp::TModelCompWrap<icam::CSnapBitmapSupplierComp> SnapBitmapSupplier;
typedef icam::CMemoryAcquisitionComp MemoryAcquisition;
typedef icam::CAcquisitionConstraintsComp AcquisitionConstraints;
typedef icam::CSelectionParamCameraComp SelectionParamCamera;
typedef icam::CCameraDelegatorComp CameraDelegator;
typedef icomp::TModelCompWrap<icam::CMultiCameraBitmapSupplierComp> MultiCameraBitmapSupplier;
typedef icomp::TModelCompWrap<icam::CSelectableBitmapSupplierComp> SelectableBitmapSupplier;


} // namespace CameraPck


#endif // !IacfBasePck_included


