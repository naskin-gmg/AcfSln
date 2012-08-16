#ifndef QtCameraPck_included
#define QtCameraPck_included


#include "iqtcam/CFileAcquisitionComp.h"
#include "iqtcam/CSnapImageGuiComp.h"
#include "iqtcam/CExposureParamsGuiComp.h"
#include "iqtcam/CBitmapSupplierGuiComp.h"
#include "iqtcam/CMultiBitmapViewComp.h"


/**
	Qt camera package.
*/
namespace QtCameraPck
{


typedef iqtcam::CFileAcquisitionComp FileAcquisition;
typedef iqtcam::CSnapImageGuiComp SnapImageGui;
typedef iqtcam::CExposureParamsGuiComp ExposureParamsGui;
typedef iqtcam::CBitmapSupplierGuiComp SnapBitmapSupplierGui;
typedef iqtcam::CMultiBitmapViewComp MultiBitmapView;


} // namespace QtCameraPck


#endif // !QtCameraPck_included


