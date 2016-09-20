#include "DocumentProcessingPck.h"


// ACF includes
#include <icomp/export.h>


namespace DocumentProcessingPck
{


I_EXPORT_PACKAGE(
			"AcfSln/DocumentProcessing",
			"Document processing package",
			"Qt Processing Document" IM_PROJECT("ACF-Solutions") IM_TAG("\"Document Processing\"") IM_COMPANY("ImagingTools") IM_AUTHOR("\"Witold Gantzke\" \"Kirill Lepskiy\""));

I_EXPORT_COMPONENT(
			RenderedDocumentPreviewGenerator,
			"Generator of the rendered document preview",
			"Preview Rendering" IM_TAG("Document"));


} // namespace DocumentProcessingPck


