// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#include "SignalPck.h"


#include <icomp/export.h>


namespace SignalPck
{


I_EXPORT_PACKAGE(
			"AcfSln/Signal",
			"General, system-independent image processing package",
			"Standard Base Imaging Tools" IM_PROJECT("ACF-Solutions") IM_TAG("\"Image Processing\"") IM_COMPANY("ImagingTools") IM_AUTHOR("\"Witold Gantzke\" \"Kirill Lepskiy\""));


I_EXPORT_COMPONENT(
			SamplingParams,
			"Sampling params based on fixed boundary values",
			"Sample Sampling Acquisition Model Parameters");

I_EXPORT_COMPONENT(
			TriggerParams,
			"Trigger params for cameras",
			"Trigger Parameters Snap Edge Falling Rising Signal");

I_EXPORT_COMPONENT(
			PulseParams,
			"Pulse control parameters",
			"Trigger Pulse Signal Parameters Delay Duration Time Period");


} // namespace SignalPck


