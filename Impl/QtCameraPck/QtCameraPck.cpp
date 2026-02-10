// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#include "QtCameraPck.h"


#include <icomp/export.h>


namespace QtCameraPck
{


I_EXPORT_PACKAGE(
			"AcfSln/QtCamera",
			"Qt camera package",
			"Qt Camera GUI" IM_PROJECT("ACF-Solutions") IM_TAG("\"Image Processing\" Camera") IM_COMPANY("ImagingTools") IM_AUTHOR("\"Witold Gantzke\" \"Kirill Lepskiy\""));

I_EXPORT_COMPONENT(
			FileAcquisition,
			"Acquise bitmap from file, load and save bitmaps in binary format as file loader",
			"Image Bitmap Acquisition Binary Loader Load Save JPG JPEG PNG BMP");

I_EXPORT_COMPONENT(
			SnapImageGui,
			"* Deprecated * use SnapImageParamsEditor insead",
			"View Image Bitmap Camera Snap Parameters Deprecated");

I_EXPORT_COMPONENT(
			SnapImageParamsEditor,
			"Delegator of snap parameters with snap GUI functionality",
			"View Image Bitmap Camera Snap Parameters");

I_EXPORT_COMPONENT(
			ExposureParamsGui,
			"Gui for exposure parameters",
			"Image Bitmap Camera Snap Acquisition Exposure Parameters");

I_EXPORT_COMPONENT(
			SnapBitmapSupplierGui,
			"Gui getting image from camera and connecting it to display console", 
			"Image Bitmap Camera Snap Acquisition Supplier Parameters");

I_EXPORT_COMPONENT(
			SnapMultiBitmapSupplierGui,
			"Gui getting a set of images from camera and connecting one of them to display console", 
			"Multi Image Bitmap Camera Snap Acquisition Supplier Parameters GUI Qt");

I_EXPORT_COMPONENT(
			ScaleCalibrationEditor,
			"Editor for scale calibration parameters",
			"Scale Calibration Numeric Values Filter Ranges Parameter Editor");

I_EXPORT_COMPONENT(
			CameraInfoGui,
			"Gui showing selected camera information",
			"GUI Camera Information");

} // namespace QtCameraPck
