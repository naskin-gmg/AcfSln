#include "icomp/export.h"

#include "SignalPck.h"


namespace SignalPck
{


I_EXPORT_DEFAULT_SERVICES

I_EXPORT_PACKAGE(
			"SignalPck",
			"General, system-independent image processing package",
			"Standard Base Imaging Tools" IM_PROJECT("ACF-Solutions") IM_TAG("\"Image Processing\"") IM_COMPANY("ImagingTools") IM_AUTHOR("\"Witold Gantzke\" \"Kirill Lepskiy\""));


I_EXPORT_COMPONENT(
			SamplingParams,
			"Sampling params based on fixed boundary values",
			"Sample Sampling Acquisition Model Parameters");

I_EXPORT_COMPONENT(
			TriggerParams,
			"Trigger params for cameras",
			"Camera Acquisition Trigger Parameters Snap");


} // namespace SignalPck


