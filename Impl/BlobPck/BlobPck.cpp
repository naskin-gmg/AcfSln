#include "BlobPck.h"


#include <icomp/export.h>


namespace BlobPck
{


I_EXPORT_PACKAGE(
			"AcfSln/BlobPck",
			"Blob extraction and processing package",
			"Blob Search Imaging Tools" IM_PROJECT("ACF-Solutions") IM_TAG("\"Image Blob Processing\"") IM_COMPANY("ImagingTools"));

I_EXPORT_COMPONENT(
			BlobFilterParams,
			"Parameters for the blob filtering",
			"Blob Filter Parameters Search" IM_CATEGORY(I_DATA_MODEL));

I_EXPORT_COMPONENT(
			PreciseBlobProcessor,
			"Precise blob processor",
			"Blob Filter Search Area Threshold Segmantation" IM_CATEGORY(I_DATA_MODEL) IM_AUTHOR("\"Witold Gantzke\""));

I_EXPORT_COMPONENT(
			BlobFilterParamsGui,
			"Editor for the blob filter parameters",
			"Blob Filter Parameters Editor UI GUI");

I_EXPORT_COMPONENT(
			BlobFeaturesSupplierGui,
			"UI for blob-related feature supplier",
			"Features Blob Search"
);


} // namespace BlobPck


