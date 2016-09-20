#include "InspectionPck.h"


#include <icomp/export.h>


namespace InspectionPck
{


I_EXPORT_PACKAGE(
			"AcfSln/InspectionPck",
			"General, system-independent inspection package",
			"Standard Base Imaging Tools" IM_PROJECT("ACF-Solutions") IM_TAG("\"Image Processing\"") IM_COMPANY("ImagingTools"));

I_EXPORT_COMPONENT(
			InspectionTask,
			"Groups set of suppliers into single inspection task",
			"Task Model Supplier Product Process Cascade" IM_CATEGORY(I_DATA_MODEL) IM_TAG("Inspection Model") IM_AUTHOR("\"Witold Gantzke\""));

I_EXPORT_COMPONENT(
			InformationProviderCache,
			"General information provider cache object used to manage threading barrier for object supplier chain",
			"Provider Cache Results" IM_CATEGORY(I_DATA_MODEL) IM_TAG("Inspection Model") IM_AUTHOR("\"Witold Gantzke\""));

I_EXPORT_COMPONENT(
			IdProviderCache,
			"ID provider cache object used to manage threading barrier for object supplier chain",
			"Provider Cache ID" IM_CATEGORY(I_DATA_MODEL) IM_TAG("Inspection Model") IM_AUTHOR("\"Witold Gantzke\""));

I_EXPORT_COMPONENT(
			ComposedInformationProvider,
			"Provider of the combined information state based on the list of underlaying information providers",
			"Provider Results Composition State Information" IM_CATEGORY(I_DATA_MODEL) IM_TAG("Inspection Model") IM_AUTHOR("\"Kirill Lepskiy\""));


} // namespace InspectionPck


