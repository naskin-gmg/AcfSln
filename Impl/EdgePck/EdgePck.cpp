#include "EdgePck.h"


#include "icomp/export.h"


namespace EdgePck
{


I_EXPORT_PACKAGE(
			"AcfSln/EdgePck",
			"Edge extraction and processing processing package",
			"Edge Outline Contour Imaging Tools" IM_PROJECT("ACF-Solutions") IM_TAG("\"Image Edge Processing\"") IM_COMPANY("ImagingTools"));

I_EXPORT_COMPONENT(
			EdgeLinesContainer,
			"Container of edge lines",
			"Edges Lines Container Set Multiple" IM_CATEGORY(I_DATA_MODEL) IM_TAG("Model 2D") IM_AUTHOR("\"Witold Gantzke\""));

I_EXPORT_COMPONENT(
			EdgesReductor,
			"Processor reducing complexity of edge lines",
			"Processor Lines Reduction Correction Simplicity" IM_CATEGORY(I_DATA_PROCESSING) IM_TAG("Processor") IM_AUTHOR("\"Witold Gantzke\""));

I_EXPORT_COMPONENT(
			FastEdgesExtractor,
			"Extract edge lines from image",
			"Image Bitmap Picture Extraction Processing" IM_CATEGORY(I_DATA_PROCESSING) IM_TAG("Processor") IM_AUTHOR("\"Witold Gantzke\""));

I_EXPORT_COMPONENT(
			ExtractedEdgeLinesSupplier,
			"Provide edge lines extracted from image",
			"Extraction Processing Provider Contours Edges Borders Lines Polylines" IM_CATEGORY(I_DATA_PROCESSING) IM_TAG("Supplier") IM_AUTHOR("\"Witold Gantzke\""));

I_EXPORT_COMPONENT(
			ProcessedEdgeLinesSupplier,
			"Edge line supplier provide edges from another edge supplier",
			"Contours Edges Borders Lines Polylines Supplier Delegated Converted Transformed" IM_CATEGORY(I_GUI) IM_TAG("Supplier") IM_AUTHOR("\"Witold Gantzke\""));

I_EXPORT_COMPONENT(
			EdgeLinesSupplierGui,
			"GUI for edge line supplier",
			"Contours Edges Borders Lines Polylines Supplier GUI" IM_CATEGORY(I_GUI) IM_TAG("Supplier GUI") IM_AUTHOR("\"Krzysztof Srodon\" \"Ryszard Mroczka\""));


} // namespace EdgePck


