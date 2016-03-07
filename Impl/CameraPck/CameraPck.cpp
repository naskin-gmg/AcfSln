#include "CameraPck.h"


#include "icomp/export.h"


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
			"Image Standard Bitmap Snap Supplier");

I_EXPORT_COMPONENT(
			CalibratedCamera,
			"Camera wrapper providing bitmaps with embedded resolution information",
			"Camera Calibration Resolution Image Bitmap Snap");


} // namespace CameraPck


