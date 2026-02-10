// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#include "CalibrationGuiPck.h"


#include <icomp/export.h>


namespace CalibrationGuiPck
{


I_EXPORT_PACKAGE(
			"AcfSln/Qt/Gui/Calibration",
			"GUI for calibration package",
			IM_PROJECT("ACF-Solutions") IM_TAG("Qt Viewport Calibration") IM_COMPANY("ImagingTools") IM_AUTHOR("\"Witold Gantzke\" \"Kirill Lepskiy\""));

I_EXPORT_COMPONENT(
			NoneCalibrationShape,
			"Displays uncalibrated grid on view console",
			"Uncalibrated None Calibration 2D Grid Shape");

I_EXPORT_COMPONENT(
			AffiniteCalibrationShape,
			"Shows affine calibration grid on calibrated view",
			"Calibration Affine Display Observer 2D Grid Shape");

I_EXPORT_COMPONENT(
			PerspectiveCalibrationShape,
			"Shows perpective calibration grid on calibrated view",
			"Calibration Perspective Display Observer 2D Grid Shape");

I_EXPORT_COMPONENT(
			GeneralCalibrationShape,
			"Shows general purpose calibration grid on calibrated view",
			"Calibration General Common Display Observer 2D Grid Shape");

I_EXPORT_COMPONENT(
			PerspectiveCalibEditor,
			"Editor of perspective calibration parameters",
			"Perspective Calibration Transformation Matrix Translation" IM_TAG("GUI Editor 2D"));

I_EXPORT_COMPONENT(
			SimpleLensCorrectionEditor,
			"Editor of lens correction parameters",
			"Lens Correction Calibration Transformation Matrix Translation" IM_TAG("GUI Editor 2D"));


} // namespace CalibrationGuiPck


