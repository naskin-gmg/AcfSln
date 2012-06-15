#include "PropPck.h"


#include "icomp/export.h"


I_REGISTER_QT_RESOURCE(iqtgui);


namespace PropPck
{


I_EXPORT_PACKAGE(
			"AcfSln/PropPck",
			"Property package",
			IM_TAG("Property") IM_PROJECT("ACF-Solutions") IM_COMPANY("ImagingTools") IM_AUTHOR("\"Witold Gantzke\" \"Kirill Lepskiy\""));

I_EXPORT_COMPONENT(
			PropertiesEditor,
			"Editor for object properties",
			"Editor Object" IM_TAG("Property"));


} // namespace PropPck


