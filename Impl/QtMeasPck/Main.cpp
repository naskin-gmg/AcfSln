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

I_EXPORT_PACKAGE("AcfSln/Measure/GUI", "Qt dependent package for sampling, measurement acquisition and related thema", "Sampling Measurement Acquisition Qt GUI");

I_EXPORT_COMPONENT(SamplesSequenceView, "Display sample sequence", "Diagram Polyline Samples Sequence Measurement GUI");


} // namespace QtMeasPck


