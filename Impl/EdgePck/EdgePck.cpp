#include "EdgePck.h"


#include <icomp/export.h>


namespace EdgePck
{


I_EXPORT_PACKAGE(
			"AcfSln/EdgePck",
			"Edge extraction and processing package",
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
			"Contours Edges Borders Lines Polylines Supplier Delegated Converted Transformed" IM_CATEGORY(I_DATA_PROCESSING) IM_TAG("Supplier") IM_AUTHOR("\"Witold Gantzke\""));

I_EXPORT_COMPONENT(
			EdgeBasedFeaturesSupplier,
			"Features supplier (search) using edge lines and external processor",
			"Contours Edges Borders Lines Supplier Search Position Orientation" IM_CATEGORY(I_DATA_PROCESSING) IM_TAG("Supplier") IM_AUTHOR("\"Witold Gantzke\""));

I_EXPORT_COMPONENT(
			EdgeLinesSupplierEditor,
			"Editor for supplier of edge lines",
			"Edges Lines Contour Polyline GUI Supplier" IM_CATEGORY(I_DATA_VISUALIZATION) IM_TAG("Supplier Editor") IM_AUTHOR("\"Witold Gantzke\""));


} // namespace EdgePck


