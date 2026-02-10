// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#include "ProductionPck.h"


#include <icomp/export.h>


namespace ProductionPck
{


I_EXPORT_PACKAGE(
			"AcfSln/Production",
			"Production-related component package",
			IM_PROJECT("ACF-Solutions") IM_TAG("Production") IM_COMPANY("ImagingTools") IM_AUTHOR("\"Kirill Lepskiy\""));

I_EXPORT_COMPONENT(
			ProductionHistory,
			"Container of the production data",
			"Production Review History Quality" IM_CATEGORY(I_DATA_PROCESSING));


} // namespace ProductionPck


