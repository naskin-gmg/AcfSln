// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#include "WizardGuiPck.h"


#include <icomp/export.h>


I_REGISTER_QT_RESOURCE(iqtgui);


namespace WizardGuiPck
{


I_EXPORT_PACKAGE(
			"AcfSln/Gui/Wizard",
			"Wizard UI package",
			IM_PROJECT("ACF") IM_TAG("Qt GUI Wizard") IM_COMPANY("ImagingTools") IM_AUTHOR("\"Witold Gantzke\" \"Kirill Lepskiy\""));


I_EXPORT_COMPONENT(
			WizardOptionsListGui,
			"GUI for the list of wizard options",
			"Wizard");


} // namespace WizardGuiPck


