// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#include "QtSignalPck.h"


// ACF includes
#include <icomp/export.h>


I_REGISTER_QT_RESOURCE(iqtgui);


namespace QtSignalPck
{


I_EXPORT_PACKAGE(
			"AcfSln/Signal/Gui",
			"Imaging tools Qt-based general package",
			"Imaging Tools Qt Standard" IM_PROJECT("ACF-Solutions") IM_TAG("\"Image Processing\"") IM_COMPANY("ImagingTools"));


I_EXPORT_COMPONENT(
			SamplingParamsGui,
			"GUI editor for sampling parameters",
			"Interval Sample Sampling Interval Observer Editor Parameters GUI Qt" IM_AUTHOR("\"Witold Gantzke\""));

I_EXPORT_COMPONENT(
			PulseParamsGui,
			"GUI editor for pulse parameters",
			"Pulse Delay Relaxing Interval Observer Editor Parameters GUI Qt" IM_AUTHOR("\"Witold Gantzke\""));

I_EXPORT_COMPONENT(
			TriggerParamsGui,
			"GUI editor for trigger parameters",
			"Trigger Mode Edge Observer Editor Parameters GUI Qt" IM_AUTHOR("\"Witold Gantzke\""));

I_EXPORT_COMPONENT(
			SampleAcquisitionGui,
			"Sample acquisition GUI",
			"Sample Sampling Acquisition GUI" IM_AUTHOR("\"Witold Gantzke\""));


} // namespace QtSignalPck


