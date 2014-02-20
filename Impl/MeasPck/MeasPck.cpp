#include "MeasPck.h"


#include "icomp/export.h"


namespace MeasPck
{


I_EXPORT_PACKAGE(
			"AcfSln/Measure",
			"System-undependent package for sampling, measurement acquisition and related thema",
			"Measurement Acquisition Undependend" IM_TAG("Sampling") IM_PROJECT("ACF-Solutions") IM_COMPANY("ImagingTools"));

I_EXPORT_COMPONENT(
			GeneralBitmap,
			"General, system undependent bitmap implementation",
			"Bitmap Picture Acquisition Simple General" IM_TAG("Image") IM_AUTHOR("\"Witold Gantzke\" \"Kirill Lepskiy\""));

I_EXPORT_COMPONENT(
			GeneralDataSequence,
			"General, system undependent sample sequence implementation",
			"Samples Sequence Acquisition" IM_AUTHOR("\"Witold Gantzke\""));

I_EXPORT_COMPONENT(
			SimpleSamplesSequence8,
			"Simple sample sequence storing 8-bit samples, system undependent",
			"Samples Sequence Measurement" IM_AUTHOR("\"Witold Gantzke\" \"Kirill Lepskiy\""));

I_EXPORT_COMPONENT(
			SimpleSamplesSequence16,
			"Simple sample sequence storing 16-bit samples, system undependent",
			"Samples Sequence Measurement" IM_AUTHOR("\"Witold Gantzke\" \"Kirill Lepskiy\""));

I_EXPORT_COMPONENT(
			SimpleSamplesSequence32,
			"Simple sample sequence storing 32-bit samples, system undependent",
			"Samples Sequence Measurement" IM_AUTHOR("\"Witold Gantzke\" \"Kirill Lepskiy\""));

I_EXPORT_COMPONENT(
			WavSamplesLoader,
			"Loader for loading and saving sample sequence as Audio WAV file",
			"Audio WAV Loder Serializer Persistence Samples Sequence Measurement" IM_TAG("Loader") IM_AUTHOR("\"Witold Gantzke\""));

I_EXPORT_COMPONENT(
			DataStatistics,
			"Basic statistics of a numerical data",
			"Data" IM_TAG("Statistics") IM_AUTHOR("\"Kirill Lepskiy\""));

I_EXPORT_COMPONENT(
			DataSequenceStatistics,
			"Data sequence statistics",
			"Sequence Data" IM_TAG("Statistics") IM_AUTHOR("\"Kirill Lepskiy\""));

I_EXPORT_COMPONENT(
			HistogramStatisticsProcessor,
			"Processor for calculation of statistics",
			"Histogram Statistics" IM_TAG("Processing") IM_AUTHOR("\"Kirill Lepskiy\""));

I_EXPORT_COMPONENT(
			NumericParams,
			"Set of numeric values used for filter values, channel values etc.",
			"Numeric Values Parameter" IM_AUTHOR("\"Witold Gantzke\""));

I_EXPORT_COMPONENT(
			NumericConstraints,
			"Defines simple constraints for numeric parameters with the same units and ranges for all values",
			"Numeric Values Parameter Constraints" IM_AUTHOR("\"Witold Gantzke\""));

I_EXPORT_COMPONENT(
			LinearAdjustParams,
			"Parameter with scale and offset for simple linear transformations",
			"Parameter Scale Offset Shift Contrast Brightness Linear Power" IM_CATEGORY(I_DATA_MODEL) IM_TAG("Model Parameter") IM_AUTHOR("\"Witold Gantzke\""));

I_EXPORT_COMPONENT(
			NumericValuesJoiner,
			"Joins several numeric values under the single interface",
			"Numeric Value Join" IM_AUTHOR("\"Arseniy Masyuk\""));

I_EXPORT_COMPONENT(
			CrossCorrelationProcessor,
			"Calculate the cross correlation of two data sequences",
			"Correlation Processor Comparsion" IM_AUTHOR("\"Kirill Lepskiy\""));

I_EXPORT_COMPONENT(
			ProcessedDataSequenceSupplier,
			"Data sequence supplier providing processed data sequence from another one",
			"Supplier Data Sequence Processing Processor" IM_AUTHOR("\"Alexander Duda\""));

I_EXPORT_COMPONENT(
			DataSequenceJoinerSupplier,
			"Data sequence joiner from a (multi-)datasequence-provider",
			"Data Sequence Supplier Provider Join" IM_AUTHOR("\"Alexander Duda\""));


} // namespace MeasPck


