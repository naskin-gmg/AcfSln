// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#include "ProcessingPck.h"


#include <icomp/export.h>


namespace ProcessingPck
{


I_EXPORT_PACKAGE(
			"AcfSln/Processing",
			"Base system-undependent general package",
			IM_PROJECT("ACF-Solutions") IM_TAG("Processing") IM_COMPANY("ImagingTools") IM_AUTHOR("\"Witold Gantzke\" \"Kirill Lepskiy\""));

I_EXPORT_COMPONENT(
			CascadedProcessor,
			"Sequence processing using slave processors",
			"Processor Sequence Buffer Cascade" IM_CATEGORY(I_DATA_PROCESSING) IM_TAG("Processing Composition"));

I_EXPORT_COMPONENT(
			IterativeProcessor,
			"Iterative Processor",
			"Iterative Loop Sequence Processor" IM_CATEGORY(I_DATA_PROCESSING) IM_TAG("Processing"));

I_EXPORT_COMPONENT(
			ModelBasedProcessingTrigger,
			"Model-based processing trigger",
			"Processor Model Trigger" IM_CATEGORY(I_DATA_PROCESSING) IM_TAG("Processing"));


} // namespace ProcessingPck


