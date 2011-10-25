#include "QwtPck.h"


#include "icomp/export.h"

#include "iqt/CDefaultServicesProvider.h"


static struct StaticInit
{
	StaticInit()
	{
		Q_INIT_RESOURCE(iqtgui);
		Q_INIT_RESOURCE(Qwt3d);
	}

} instance;


namespace QwtPck
{


I_EXPORT_SERVICES_PROVIDER(iqt::CDefaultServicesProvider);

I_EXPORT_PACKAGE(
			"QwtPck",
			"QWT and QWT3D based package",
			"Qwt Qt 2D 3D View" IM_TAG("Qt") IM_PROJECT("ACF-Solutions") IM_COMPANY("ImagingTools") IM_AUTHOR("\"Witold Gantzke\" \"Kirill Lepskiy\""));

I_EXPORT_COMPONENT(
			ProgressHistoryGui,
			"Progress history shown as 2D diagram",
			"Progress History Diagram Activity Curve Line Statistics" IM_TAG("2D"));

I_EXPORT_COMPONENT(
			HistogramView,
			"Histogram view",
			"Histogram Statistics" IM_TAG("GUI"));

I_EXPORT_COMPONENT(
			SurfaceView,
			"3D-View component",
			"Model Observer Parameter GUI Qt 3D Surface" IM_TAG("Image 3D"));


} // namespace QwtPck


