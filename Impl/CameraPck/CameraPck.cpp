#include "CameraPck.h"


#include "icomp/export.h"


namespace CameraPck
{


I_EXPORT_DEFAULT_SERVICES

I_EXPORT_PACKAGE(
			"CameraPck",
			"General, system-independent image processing package",
			"Standard Base Imaging Tools" IM_PROJECT("ACF-Solutions") IM_TAG("\"Image Processing\"") IM_COMPANY("ImagingTools") IM_AUTHOR("\"Witold Gantzke\" \"Kirill Lepskiy\""));


I_EXPORT_COMPONENT(
			ExposureParams,
			"Parameters for exposure parameter control",
			"Exposure Shutter Delay Time EEN Image Bitmap Acquisition Snap Parameters");

I_EXPORT_COMPONENT(
			SnapBitmapSupplier,
			"Implementation of bitmap supplier getting image from camera",
			"Image Bitmap Acquisition Snap Supplier Parameters");

I_EXPORT_COMPONENT(
			MemoryAcquisition,
			"Image acquisition from the image itself",
			"Image Bitmap Camera Snap Acquisition");

I_EXPORT_COMPONENT(
			AcquisitionConstraints,
			"Static acquisition constraints for some device",
			"Image Bitmap Camera Snap Acquisition Constraints");


} // namespace CameraPck


