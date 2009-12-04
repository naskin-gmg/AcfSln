#include "icomp/export.h"

#include "iqt/CDefaultServicesProvider.h"

#include "QwtPck.h"


static struct StaticInit
{
	StaticInit()
	{
		Q_INIT_RESOURCE(iqtgui);
	}

} instance;


namespace QwtPck
{


I_EXPORT_SERVICES_PROVIDER(iqt::CDefaultServicesProvider);

I_EXPORT_PACKAGE("QwtPck", "Qwt package", "Qwt Qt 3D View");

I_EXPORT_COMPONENT(SurfaceView, "3D-View component", "Model Observer Parameter GUI Qt 3D Surface");


} // namespace QwtPck


