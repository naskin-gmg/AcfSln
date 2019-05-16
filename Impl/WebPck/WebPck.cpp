#include "WebPck.h"


#include <icomp/export.h>


namespace WebPck
{


I_EXPORT_PACKAGE(
			"AcfSln/Web",
			"Web service component",
			IM_PROJECT("ACF") IM_TAG("Web") IM_COMPANY("ImagingTools"));

I_EXPORT_COMPONENT(
			WebServerStatus,
			"Status of a web-server",
			"Server HTTP Web" IM_CATEGORY(I_DATA_MODEL));


} // namespace WebPck


