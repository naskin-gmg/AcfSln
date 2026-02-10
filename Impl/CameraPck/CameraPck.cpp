// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#include "CameraPck.h"


#include <icomp/export.h>


namespace CameraPck
{


I_EXPORT_PACKAGE(
			"AcfSln/CameraPck",
			"General, system- and hardware-independent camera package",
			"Standard Base Imaging Camera" IM_PROJECT("ACF-Solutions") IM_TAG("\"Image Processing\"") IM_COMPANY("ImagingTools") IM_AUTHOR("\"Witold Gantzke\" \"Kirill Lepskiy\""));


I_EXPORT_COMPONENT(
			ExposureParams,
			"Parameters for exposure parameter control",
			"Exposure Shutter Delay Time EEN Image Bitmap Acquisition Snap Parameters");

I_EXPORT_COMPONENT(
			MemoryAcquisition,
			"Image acquisition from the image itself",
			"Image Bitmap Camera Snap Acquisition");

I_EXPORT_COMPONENT(
			AcquisitionConstraints,
			"Static acquisition constraints for some device",
			"Image Bitmap Camera Snap Acquisition Constraints");

I_EXPORT_COMPONENT(
			SelectionParamCamera,
			"Camera delegator enhancing parameter set using some selection and parameter manager",
			"Image Bitmap Camera Snap Acquisition Parameter Manager Delegator");

I_EXPORT_COMPONENT(
			CameraDelegator, 
			"Simple camera delegator",
			"Image Bitmap Camera Snap Acquisition Manager Delegator");

I_EXPORT_COMPONENT(
			MultiCameraBitmapSupplier,
			"Multi camera bitmap provider", 
			"Image Bitmap Camera Snap Acquisition Supplier Multi Manager");

I_EXPORT_COMPONENT(
			SelectableBitmapSupplier,
			"Single bitmap supplier from a mutli bitmap provider", 
			"Image Bitmap Camera Snap Acquisition Supplier Multi Selection Single Manager");

I_EXPORT_COMPONENT(
			SnapMultiPageBitmapSupplier, 
			"Multi-page bitmap supplier getting the image from a camera", 
			"Image Bitmap Camera Snap Acquisition Supplier Multi-Page");

I_EXPORT_COMPONENT(
			SnapBitmapSupplier,
			"Implementation of factory-based bitmap supplier getting image from camera",
			"Image Bitmap Acquisition Snap Supplier");

I_EXPORT_COMPONENT(
			StandardQtBitmapSnapSupplier,
			"Implementation of Qt-bitmap based supplier getting image from camera",
			"Image Bitmap QImage Snap Supplier");

I_EXPORT_COMPONENT(
			StandardBitmapSnapSupplier,
			"Implementation of the bitmap supplier getting image from camera using standard bitmap implementation",
			"Image Bitmap Snap Supplier");

I_EXPORT_COMPONENT(
			CalibratedCamera,
			"Camera wrapper providing bitmaps with embedded resolution information",
			"Camera Calibration Resolution Image Bitmap Snap");

I_EXPORT_COMPONENT(
			MultiSourceSnapBitmapSupplier,
			"Bitmap snap supplier based on set of registered cameras",
			"Camera Calibration Supplier Image Bitmap Snap");

I_EXPORT_COMPONENT(
			BitmapSupplierMultiplexer,
			"Bitmap supplier based on selection of the underlayed bitmap provider",
			"Calibration Supplier Image Bitmap Multi");

I_EXPORT_COMPONENT(
			CameraMultiplexer,
			"Camera mutiplexer",
			"Camera Acquisition Multiplexer");

I_EXPORT_COMPONENT(
			MultiCameraJoiner,
			"Performs snap from the slave cameras and joins the images together",
			"Multi Join Camera" IM_TAG("Camera Image"));

I_EXPORT_COMPONENT(
			ProcessedCamera,
			"Performs pre- and post-processing for the snapped image",
			"Processor" IM_TAG("Camera Image"));


} // namespace CameraPck


