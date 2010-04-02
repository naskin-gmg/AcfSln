#include "icomp/export.h"

#include "MeasPck.h"


namespace MeasPck
{


I_EXPORT_DEFAULT_SERVICES;

I_EXPORT_PACKAGE("AcfSln/Measure", "System-undependent package for sampling, measurement acquisition and related thema", "Sampling Measurement Acquisition Undependend");

I_EXPORT_COMPONENT(
			GeneralBitmap,
			"General, system undependent bitmap implementation",
			"Bitmap Image Picture Acquisition Simple General");

I_EXPORT_COMPONENT(
			GeneralSamplesSequence,
			"General, system undependent sample sequence implementation",
			"Samples Sequence Acquisition");

I_EXPORT_COMPONENT(
			SimpleSamplesSequence8,
			"Simple sample sequence storing 8-bit samples, system undependent",
			"Samples Sequence Measurement");

I_EXPORT_COMPONENT(
			SimpleSamplesSequence16,
			"Simple sample sequence storing 16-bit samples, system undependent",
			"Samples Sequence Measurement");

I_EXPORT_COMPONENT(
			SimpleSamplesSequence32,
			"Simple sample sequence storing 32-bit samples, system undependent",
			"Samples Sequence Measurement");

I_EXPORT_COMPONENT(
			WavSamplesLoader,
			"Loader for loading and saving sample sequence as Audio WAV file",
			"Audio WAV Loder Serializer Persistence Samples Sequence Measurement");


} // namespace MeasPck


