// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#include "ProcessingGuiPck.h"


#include <icomp/export.h>


I_REGISTER_QT_RESOURCE(iqtgui);


namespace ProcessingGuiPck
{


I_EXPORT_PACKAGE(
			"AcfSln/ProcessingGui",
			"Processing GUI package",
			IM_PROJECT("ACF-Solutions") IM_TAG("Qt GUI Processing") IM_COMPANY("ImagingTools") IM_AUTHOR("\"Witold Gantzke\" \"Kirill Lepskiy\""));

I_EXPORT_COMPONENT(
			ProcessorControlGui,
			"Processor acquisition GUI",
			"Acquisition Processor Parameter View" IM_TAG("Processing"));

I_EXPORT_COMPONENT(
			DocumentProcessingManager,
			"Document processing manager",
			"Processing Document" IM_TAG("Processing"));

I_EXPORT_COMPONENT(
			DocumentProcessingCommand,
			"Document processing command",
			"Processing Document Command" IM_TAG("Processing"));

I_EXPORT_COMPONENT(
			ProcessorCommand,
			"Binding of any processor to menu command",
			"Menu Processor Command Action");


} // namespace ProcessingGuiPck


