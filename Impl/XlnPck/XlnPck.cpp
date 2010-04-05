#include "icomp/export.h"

#include "XlnPck.h"


namespace XlnPck
{


I_EXPORT_DEFAULT_SERVICES;

I_EXPORT_PACKAGE(
			"Xln",
			"Xml processing package based on XERCES and XALAN",
			"XERCES XALAN XSLT Transformation Parser DOM" IM_TAG("XML") IM_PROJECT("ACF-Solutions") IM_COMPANY("ImagingTools") IM_AUTHOR("\"Witold Gantzke\" \"Kirill Lepskiy\""));

I_EXPORT_COMPONENT(
			XmlFileSerializer,
			"XERCES based file loader serializing objects using XML format",
			"XERCES Load Save Serializer Archive Format" IM_TAG("File Loader"));


} // namespace XlnPck


