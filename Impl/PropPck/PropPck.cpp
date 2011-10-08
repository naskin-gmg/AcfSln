#include "icomp/export.h"

#include "iqt/CDefaultServicesProvider.h"

#include "PropPck.h"


namespace PropPck
{


static struct StaticInit
{
	StaticInit()
	{
		Q_INIT_RESOURCE(iqtgui);
	}

} instance;


I_EXPORT_SERVICES_PROVIDER(iqt::CDefaultServicesProvider);

I_EXPORT_PACKAGE(
			"PropPck",
			"Property package",
			IM_TAG("Property") IM_PROJECT("ACF-Solutions") IM_COMPANY("ImagingTools") IM_AUTHOR("\"Witold Gantzke\" \"Kirill Lepskiy\""));

I_EXPORT_COMPONENT(
			PropertiesEditor,
			"Editor for object properties",
			"Editor Object" IM_TAG("Property"));


} // namespace PropPck


