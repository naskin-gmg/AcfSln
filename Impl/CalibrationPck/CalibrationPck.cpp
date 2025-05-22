#include "CalibrationPck.h"


#include <icomp/export.h>


namespace CalibrationPck
{


I_EXPORT_PACKAGE(
			"AcfSln/Calibration",
			"Calibration related package",
			IM_PROJECT("ACF-Solutions") IM_TAG("Calibration") IM_COMPANY("ImagingTools") IM_AUTHOR("\"Witold Gantzke\" \"Kirill Lepskiy\""));

I_EXPORT_COMPONENT(
			AffineCalibration2d,
			"Affine calibration",
			"Affine Calibration Tranformation" IM_CATEGORY(I_DATA_MODEL) IM_TAG("2D Model"));

I_EXPORT_COMPONENT(
			PerspectiveCalibration2d,
			"Perspectivic calibration",
			"Perspective Calibration Tranformation" IM_CATEGORY(I_DATA_MODEL) IM_TAG("2D Model"));

I_EXPORT_COMPONENT(
			SimpleLensCorrection,
			"Lens correction calibration based on single distortion factor",
			"Lens Correction Distortion Calibration Tranformation" IM_CATEGORY(I_DATA_MODEL) IM_TAG("2D Model"));

I_EXPORT_COMPONENT(
			StaticCalibrationProvider,
			"Provider of a static calibration object",
			"2D Tranformation Calibration" IM_CATEGORY(I_DATA_MODEL) IM_TAG("2D"));

I_EXPORT_COMPONENT(
			Calibration2dProxy,
			"Proxy of a 2D calibration using calibration provider",
			"2D Tranformation Calibration Proxy Provider" IM_CATEGORY(I_DATA_MODEL) IM_TAG("2D"));

I_EXPORT_COMPONENT(
			AffineCalibrationProvider,
			"Provides of affine transformation based on rotation, translation and scale parameters",
			"2D Affine Tranformation Calibration" IM_CATEGORY(I_DATA_MODEL) IM_TAG("2D"));

I_EXPORT_COMPONENT(
			LUTCalibration2d,
			"Lookuptable 2d calibration",
			"LUT Calibration Tranformation" IM_CATEGORY(I_DATA_MODEL) IM_TAG("2D Model"));

} // namespace CalibrationPck


