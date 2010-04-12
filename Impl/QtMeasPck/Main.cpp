#include "icomp/export.h"

#include "QtMeasPck.h"


#include "iqtgui/iqtgui.h"


static struct StaticInit
{
	StaticInit()
	{
		Q_INIT_RESOURCE(iqtgui);
	}

} instance;


namespace QtMeasPck
{


I_EXPORT_DEFAULT_SERVICES;

I_EXPORT_PACKAGE(
			"AcfSln/Measure/GUI",
			"Qt dependent package for sampling, measurement acquisition and related thema",
			"Measurement Acquisition GUI" IM_TAG("Qt Sampling") IM_PROJECT("ACF-Solutions") IM_COMPANY("ImagingTools") IM_AUTHOR("\"Witold Gantzke\" \"Kirill Lepskiy\""));

I_EXPORT_COMPONENT(
			DataSequenceView,
			"Display sample sequence",
			"Diagram Polyline Samples Sequence Measurement GUI" IM_TAG("2D GUI"));


} // namespace QtMeasPck


