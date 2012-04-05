#include "QtCameraPck.h"


#include "icomp/export.h"


namespace QtCameraPck
{


I_EXPORT_PACKAGE(
			"AcfSln/QtCamera",
			"Qt camera package",
			"Qt Camera" IM_PROJECT("ACF-Solutions") IM_TAG("\"Image Processing\"") IM_COMPANY("ImagingTools") IM_AUTHOR("\"Witold Gantzke\" \"Kirill Lepskiy\""));

I_EXPORT_COMPONENT(
			FileAcquisition,
			"Acquise bitmap from file, load and save bitmaps in binary format as file loader",
			"Image Bitmap Acquisition Binary Loader Load Save JPG JPEG PNG BMP Qt");

I_EXPORT_COMPONENT(
			SnapImageGui,
			"Show snapped image and allow to change snap parameters",
			"View Image Bitmap Camera Snap Parameters GUI Qt");

I_EXPORT_COMPONENT(
			ExposureParamsGui,
			"Gui for exposure parameters",
			"Image Bitmap Camera Snap Acquisition Exposure Parameters GUI Qt");

I_EXPORT_COMPONENT(
			SnapBitmapSupplierGui,
			"Gui getting image from camera and connecting it to display console", 
			"Image Bitmap Camera Snap Acquisition Supplier Parameters GUI Qt");


} // namespace QtCameraPck
