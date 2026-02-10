// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#include "BarcodePck.h"


#include <icomp/export.h>


namespace BarcodePck
{


I_EXPORT_PACKAGE(
			"AcfSln/BarcodePck",
			"Barcode-related processing package",
			"Barcode Search" IM_PROJECT("ACF-Solutions") IM_TAG("\"Image Barcode Processing\"") IM_COMPANY("ImagingTools"));

I_EXPORT_COMPONENT(
			GeneralBarcodeSearchParams,
			"Parameters for barcode search",
			"Barcode Parameters Search" IM_CATEGORY(I_DATA_MODEL));

} // namespace BarcodePck


