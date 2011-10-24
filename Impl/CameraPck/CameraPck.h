#ifndef IacfBasePck_included
#define IacfBasePck_included


// ACF includes
#include "icomp/TModelCompWrap.h"
#include "icomp/TMakeComponentWrap.h"

#include "icam/CExposureParamsComp.h"
#include "icam/CSnapBitmapSupplierComp.h"
#include "icam/CMemoryAcquisitionComp.h"
#include "icam/CAcquisitionConstraintsComp.h"


/**
	Base system-undependent general package.
*/
namespace CameraPck
{


typedef icomp::TModelCompWrap<icam::CExposureParamsComp> ExposureParams;
typedef icomp::TModelCompWrap<icam::CSnapBitmapSupplierComp> SnapBitmapSupplier;
typedef icam::CMemoryAcquisitionComp MemoryAcquisition;
typedef icam::CAcquisitionConstraintsComp AcquisitionConstraints;


} // namespace CameraPck


#endif // !IacfBasePck_included


