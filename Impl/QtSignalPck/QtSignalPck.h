// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#ifndef QtSignalPck_included
#define QtSignalPck_included


// ACF includes
#include <icomp/TModelCompWrap.h>
#include <icomp/TMakeComponentWrap.h>

#include <iqtsig/CSamplingParamsGuiComp.h>
#include <iqtsig/CPulseParamsGuiComp.h>
#include <iqtsig/CTriggerParamsGuiComp.h>
#include <iqtsig/CSampleAcquisitionGuiComp.h>


/**
	Standard Qt package.
*/
namespace QtSignalPck
{


typedef iqtsig::CSamplingParamsGuiComp SamplingParamsGui;
typedef iqtsig::CPulseParamsGuiComp PulseParamsGui;
typedef iqtsig::CTriggerParamsGuiComp TriggerParamsGui;
typedef iqtsig::CSampleAcquisitionGuiComp SampleAcquisitionGui;


} // namespace QtSignalPck


#endif // !QtSignalPck_included


