// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#include "QtAuthorizationPck.h"


// Qt includes
#include <QtCore/QDir>

// ACF includes
#include <icomp/export.h>


I_REGISTER_QT_RESOURCE(iqtgui);


namespace QtAuthorizationPck
{


I_EXPORT_PACKAGE(
			"AcfSln/Gui/AuthorizationPck",
			"GUI components for authorization",
			"GUI Qt" IM_PROJECT("ACF-Solutions") IM_TAG("Authorization") IM_COMPANY("ImagingTools") IM_AUTHOR("\"Witold Gantzke\" \"Kirill Lepskiy\""));


I_EXPORT_COMPONENT(
			LoginBarGui,
			"Login GUI",
			"Login");

I_EXPORT_COMPONENT(
			UserGui,
			"GUI displaying list of users",
			"User Rights");


} // namespace QtAuthorizationPck


