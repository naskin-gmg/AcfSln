#include "icomp/export.h"

#include "iqt/CDefaultServicesProvider.h"

#include "QwtPck.h"


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
			"Qwt package",
			"Qwt Qt 3D View" IM_TAG("Qt 3D") IM_PROJECT("ACF-Solutions") IM_COMPANY("ImagingTools") IM_AUTHOR("\"Witold Gantzke\" \"Kirill Lepskiy\""));

I_EXPORT_COMPONENT(
			SurfaceView,
			"3D-View component",
			"Model Observer Parameter GUI Qt 3D Surface" IM_TAG("Image"));


} // namespace QwtPck


