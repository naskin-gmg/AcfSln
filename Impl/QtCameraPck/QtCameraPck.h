#ifndef QtCameraPck_included
#define QtCameraPck_included


#include <iqtcam/CFileAcquisitionComp.h>
#include <iqtcam/CSnapImageGuiComp.h>
#include <iqtcam/CExposureParamsGuiComp.h>
#include <iqtcam/CBitmapSupplierGuiComp.h>
#include <iqtcam/CMultiBitmapSupplierGuiComp.h>
#include <iqtcam/CScaleCalibrationEditorComp.h>
#include <iqtcam/CCameraInfoGuiComp.h>


/**
	Qt camera package.
*/
namespace QtCameraPck
{


typedef iqtcam::CFileAcquisitionComp FileAcquisition;
typedef iqtcam::CSnapImageGuiComp SnapImageGui;
typedef iqtcam::CExposureParamsGuiComp ExposureParamsGui;
typedef iqtcam::CBitmapSupplierGuiComp SnapBitmapSupplierGui;
typedef iqtcam::CMultiBitmapSupplierGuiComp SnapMultiBitmapSupplierGui;
typedef iqtcam::CScaleCalibrationEditorComp ScaleCalibrationEditor;
typedef iqtcam::CCameraInfoGuiComp CameraInfoGui;


} // namespace QtCameraPck


#endif // !QtCameraPck_included


