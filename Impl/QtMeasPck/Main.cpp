#include "QtMeasPck.h"


#include "icomp/export.h"

#include "iqtgui/iqtgui.h"


namespace QtMeasPck
{


I_REGISTER_QT_RESOURCE(iqtgui);

I_EXPORT_PACKAGE(
			"AcfSln/Measure/GUI",
			"Qt dependent package for sampling, measurement acquisition and related thema",
			"Measurement Acquisition GUI" IM_TAG("Qt Sampling") IM_PROJECT("ACF-Solutions") IM_COMPANY("ImagingTools") IM_AUTHOR("\"Witold Gantzke\" \"Kirill Lepskiy\""));

I_EXPORT_COMPONENT(
			DataSequenceView,
			"Display sample sequence",
			"Diagram Polyline Samples Sequence Measurement GUI" IM_TAG("2D GUI"));

I_EXPORT_COMPONENT(
			DataStatisticsGui,
			"Data statistics GUI",
			"Statistics Histogram Data" IM_TAG("GUI"));

I_EXPORT_COMPONENT(
			NumericParamsGui,
			"Editor for numeric parameters",
			"Numeric Values Filter Ranges Parameter Editor");


} // namespace QtMeasPck


