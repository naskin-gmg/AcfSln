// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#include "WizardPck.h"


#include <icomp/export.h>


namespace WizardPck
{


I_EXPORT_PACKAGE(
			"AcfSln/Wizard",
			"Wizard data model and controller package",
			IM_PROJECT("ACF") IM_TAG("Wizard") IM_COMPANY("ImagingTools") IM_AUTHOR("\"Witold Gantzke\" \"Kirill Lepskiy\""));

I_EXPORT_COMPONENT(
			WizardDocument,
			"Wizard controller",
			"Wizard Parameter" IM_CATEGORY(I_DATA_MODEL));

I_EXPORT_COMPONENT(
			ComposedStateController,
			"State controller composing other state controller implementations to provide single state",
			"State Parameter Composition Composed Graph" IM_CATEGORY(I_DATA_MODEL));

I_EXPORT_COMPONENT(
			SelectionConsistencyController,
			"State controller calculating its state using check of external selection parameters",
			"State Controller Selection Parameters Dependency" IM_CATEGORY(I_DATA_MODEL))


} // namespace WizardPck


