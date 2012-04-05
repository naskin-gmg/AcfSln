#include "InspectionPck.h"


#include "icomp/export.h"


namespace InspectionPck
{


I_EXPORT_PACKAGE(
			"AcfSln/InspectionPck",
			"General, system-independent image processing package",
			"Standard Base Imaging Tools" IM_PROJECT("ACF-Solutions") IM_TAG("\"Image Processing\"") IM_COMPANY("ImagingTools") IM_AUTHOR("\"Witold Gantzke\" \"Kirill Lepskiy\""));


I_EXPORT_COMPONENT(
			InspectionTask,
			"Group set of suppliers into single inspection task",
			"Task Model Supplier Product Process Cascade" IM_CATEGORY(I_DATA_MODEL) IM_TAG("Inspection Model"));

I_EXPORT_COMPONENT(
			Inspection,
			"Group set of inspection tasks into single one",
			"Task Model Supplier Product Composite Cascade" IM_CATEGORY(I_DATA_MODEL) IM_TAG("Inspection Model"));

I_EXPORT_COMPONENT(
			GeneralResultCache,
			"General inspection provider cache object",
			"Provider Cache Results" IM_CATEGORY(I_DATA_MODEL) IM_TAG("Inspection Model"));


I_EXPORT_COMPONENT(
			IdCache,
			"ID provider cache object",
			"Provider Cache ID" IM_CATEGORY(I_DATA_MODEL) IM_TAG("Inspection Model"));


} // namespace InspectionPck


