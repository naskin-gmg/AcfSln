// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
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


