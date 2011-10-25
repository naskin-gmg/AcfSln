#include "ZlibPck.h"


#include "icomp/export.h"


namespace ZlibPck
{


I_EXPORT_DEFAULT_SERVICES;

I_EXPORT_PACKAGE(
			"Zlib",
			"Package based on compression library ZLIB",
			"ZLIB Compression File Archive Load Save" IM_PROJECT("ACF-Solutions") IM_COMPANY("ImagingTools") IM_AUTHOR("\"Witold Gantzke\" \"Kirill Lepskiy\""));

I_EXPORT_COMPONENT(
			GzXmlFileSerializer,
			"Compressed XML file serializer",
			"Compression Compressed Serializer Archive Load Save GZ ZLIB ZIP" IM_TAG("XML File Loader"));


} // namespace ZlibPck


