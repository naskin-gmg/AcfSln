#include "EdgePck.h"


#include "icomp/export.h"


namespace EdgePck
{


I_EXPORT_PACKAGE(
			"AcfSln/EdgePck",
			"Edge extraction and processing processing package",
			"Edge Outline Contour Imaging Tools" IM_PROJECT("ACF-Solutions") IM_TAG("\"Image Edge Processing\"") IM_COMPANY("ImagingTools"));

I_EXPORT_COMPONENT(
			EdgesReductor,
			"Processor reducing complexity of edge lines",
			"Processor Lines Reduction Correction Simplicity" IM_CATEGORY(I_DATA_PROCESSING) IM_TAG("Processor") IM_AUTHOR("\"Witold Gantzke\""));

I_EXPORT_COMPONENT(
			FastEdgesExtractor,
			"Extract edge lines from image",
			"Image Bitmap Picture Extraction Processing" IM_CATEGORY(I_DATA_PROCESSING) IM_TAG("Processor") IM_AUTHOR("\"Witold Gantzke\""));


} // namespace EdgePck


